#include <cnc/modules/Planner.h>
#include <cnc/space.h>

#include <cstring>
#include <cmath>
#include <cstdlib>

#undef round
#undef abs

inline float pow2(float x)
{
    return x * x;
}

// Inspired by https://onehossshay.wordpress.com/2011/09/24/improving_grbl_cornering_algorithm/
tl::optional<float> calculateJunctionFeedRateInMmPerS(
    const PlannerLine& currentLine,
    const PlannerLine& nextLine,
    float maxAccelerationInMmPerSS,
    float deviationInMm)
{
    constexpr float MAX_DISTANCE = 0.01f;
    constexpr float EPS = 1e-6;
    if ((currentLine.endPoint - nextLine.startPoint).norm() > MAX_DISTANCE)
    {
        return tl::nullopt;
    }

    Vector3<float> entry = currentLine.endPoint - currentLine.startPoint;
    Vector3<float> exit = nextLine.endPoint - nextLine.startPoint;

    float cosTheta = -entry.dot(exit) / (entry.norm() * exit.norm() + EPS);
    float sinHalfTheta = std::sqrt((1.f - cosTheta) / 2.f + EPS);
    float radius = deviationInMm * sinHalfTheta / (1.f - sinHalfTheta + EPS);

    float feedRateJunctionInMmPerS = std::sqrt(maxAccelerationInMmPerSS * radius);

    return std::min(feedRateJunctionInMmPerS, std::min(currentLine.feedRateInMmPerS, nextLine.feedRateInMmPerS));
}

tl::optional<PlannerBlock> PlannerBlock::fromLine(
    const PlannerLine& line,
    float entryFeedRateInMmPerS,
    float exitFeedRateInMmPerS,
    float accelerationInMmPerSS)
{
    if (entryFeedRateInMmPerS > line.feedRateInMmPerS || exitFeedRateInMmPerS > line.feedRateInMmPerS)
    {
        return tl::nullopt;
    }

    PlannerBlock block;
    block.m_startPoint = line.startPoint;
    block.m_endPoint = line.endPoint;
    block.m_distance = (line.endPoint - line.startPoint).norm();

    block.setDirectionFromLine(line);
    block.setAccelerationAndFeedRates(
        entryFeedRateInMmPerS,
        line.feedRateInMmPerS,
        exitFeedRateInMmPerS,
        accelerationInMmPerSS);
    block.calculateTrapezoid(accelerationInMmPerSS);

    block.m_spindleRpm = line.spindleRpm;

    return block;
}

void PlannerBlock::setDirectionFromLine(const PlannerLine& line)
{
    if (line.endPoint.x >= line.startPoint.x)
    {
        m_directions[AXIS_X_INDEX] = Direction::FORWARD;
    }
    else
    {
        m_directions[AXIS_X_INDEX] = Direction::BACKWARD;
    }

    if (line.endPoint.y >= line.startPoint.y)
    {
        m_directions[AXIS_Y_INDEX] = Direction::FORWARD;
    }
    else
    {
        m_directions[AXIS_Y_INDEX] = Direction::BACKWARD;
    }

    if (line.endPoint.z >= line.startPoint.z)
    {
        m_directions[AXIS_Z_INDEX] = Direction::FORWARD;
    }
    else
    {
        m_directions[AXIS_Z_INDEX] = Direction::BACKWARD;
    }
}

void PlannerBlock::setAccelerationAndFeedRates(
    float entryFeedRateInMmPerS,
    float feedRateInMmPerS,
    float exitFeedRateInMmPerS,
    float accelerationInMmPerSS)
{
    m_entryFeedRateInMmPerS = entryFeedRateInMmPerS;

    float da2 = 2.f * m_distance * accelerationInMmPerSS;
    float squaredEntryFeedRateInMmPerS = pow2(entryFeedRateInMmPerS);
    float squaredFeedRateInMmPerS = pow2(feedRateInMmPerS);
    float squaredExitFeedRateInMmPerS = pow2(exitFeedRateInMmPerS);

    float maxFeedRateOnlyAcceleration = std::sqrt(da2 + squaredEntryFeedRateInMmPerS);
    float minFeedRateOnlyDeceleration = std::sqrt(squaredEntryFeedRateInMmPerS - da2);
    float maxFeedRateAccelerationAndDeceleration =
        std::sqrt(da2 + squaredEntryFeedRateInMmPerS + squaredExitFeedRateInMmPerS) / 1.41421356237f;

    float distanceOnlyAcceleration =
        (squaredFeedRateInMmPerS - squaredEntryFeedRateInMmPerS) / (2.f * accelerationInMmPerSS);
    float distanceOnlyDeceleration =
        (squaredFeedRateInMmPerS - squaredExitFeedRateInMmPerS) / (2.f * accelerationInMmPerSS);

    if (entryFeedRateInMmPerS < exitFeedRateInMmPerS && maxFeedRateOnlyAcceleration < feedRateInMmPerS)
    {
        m_feedRateInMmPerS = maxFeedRateOnlyAcceleration;
        m_exitFeedRateInMmPerS = maxFeedRateOnlyAcceleration;
    }
    else if (minFeedRateOnlyDeceleration > exitFeedRateInMmPerS)
    {
        m_feedRateInMmPerS = m_entryFeedRateInMmPerS;
        m_exitFeedRateInMmPerS = minFeedRateOnlyDeceleration;
    }
    else if ((distanceOnlyAcceleration + distanceOnlyDeceleration) < m_distance)
    {
        m_feedRateInMmPerS = feedRateInMmPerS;
        m_exitFeedRateInMmPerS = exitFeedRateInMmPerS;
    }
    else
    {
        m_feedRateInMmPerS = maxFeedRateAccelerationAndDeceleration;
        m_exitFeedRateInMmPerS = exitFeedRateInMmPerS;
    }
}

void PlannerBlock::calculateTrapezoid(float accelerationInMmPerSS)
{
    m_accelerationDurationS = (m_feedRateInMmPerS - m_entryFeedRateInMmPerS) / accelerationInMmPerSS;
    m_decelerationDurationS = (m_feedRateInMmPerS - m_exitFeedRateInMmPerS) / accelerationInMmPerSS;

    float accelerationDistance = 0.5f * (m_feedRateInMmPerS + m_entryFeedRateInMmPerS) * m_accelerationDurationS;
    float decelerationDistance = 0.5f * (m_feedRateInMmPerS + m_exitFeedRateInMmPerS) * m_decelerationDurationS;
    float plateauDistance = m_distance - accelerationDistance - decelerationDistance;

    m_plateauDurationS = plateauDistance / m_feedRateInMmPerS;
}

LinearBlock PlannerBlock::toLinearBlock(
    float xStepCountPerMm,
    float yStepCountPerMm,
    float zStepCountPerMm,
    float minFeedRateInMmPerS,
    float tickFrequency)
{
    double xStepCountPerMmDouble = xStepCountPerMm;
    double yStepCountPerMmDouble = yStepCountPerMm;
    double zStepCountPerMmDouble = zStepCountPerMm;
    double tickFrequencyDouble = tickFrequency;

    LinearBlock block;
    // Set directions
    block.directions[AXIS_X_INDEX] = m_directions[AXIS_X_INDEX];
    block.directions[AXIS_Y_INDEX] = m_directions[AXIS_Y_INDEX];
    block.directions[AXIS_Z_INDEX] = m_directions[AXIS_Z_INDEX];

    // Set section durations
    block.currentTick = 0;
    block.accelerationUntilTick =
        static_cast<uint64_t>(std::round(static_cast<double>(m_accelerationDurationS) * tickFrequencyDouble));
    block.plateauUntilTick = static_cast<uint64_t>(
        std::round(static_cast<double>(m_accelerationDurationS + m_plateauDurationS) * tickFrequencyDouble));
    block.decelerationUntilTick = static_cast<uint64_t>(std::round(
        static_cast<double>(m_accelerationDurationS + m_plateauDurationS + m_decelerationDurationS) *
        tickFrequencyDouble));


    // Set step count
    block.currentStepCount[AXIS_X_INDEX] = 0;
    block.currentStepCount[AXIS_Y_INDEX] = 0;
    block.currentStepCount[AXIS_Z_INDEX] = 0;

    Vector3<double> startPointDouble(
        static_cast<double>(m_startPoint.x),
        static_cast<double>(m_startPoint.y),
        static_cast<double>(m_startPoint.z));
    Vector3<double> endPointDouble(
        static_cast<double>(m_endPoint.x),
        static_cast<double>(m_endPoint.y),
        static_cast<double>(m_endPoint.z));

    auto startXInStep = static_cast<int32_t>(std::round(startPointDouble.x * xStepCountPerMmDouble));
    auto startYInStep = static_cast<int32_t>(std::round(startPointDouble.y * yStepCountPerMmDouble));
    auto startZInStep = static_cast<int32_t>(std::round(startPointDouble.z * zStepCountPerMmDouble));
    auto endXInStep = static_cast<int32_t>(std::round(endPointDouble.x * xStepCountPerMmDouble));
    auto endYInStep = static_cast<int32_t>(std::round(endPointDouble.y * yStepCountPerMmDouble));
    auto endZInStep = static_cast<int32_t>(std::round(endPointDouble.z * zStepCountPerMmDouble));
    block.totalStepCount[AXIS_X_INDEX] = std::labs(startXInStep - endXInStep);
    block.totalStepCount[AXIS_Y_INDEX] = std::labs(startYInStep - endYInStep);
    block.totalStepCount[AXIS_Z_INDEX] = std::labs(startZInStep - endZInStep);


    // Set acceleration
    auto entryFeedRateInMmPerSDouble = static_cast<double>(m_entryFeedRateInMmPerS);
    auto feedRateInMmPerSDouble = static_cast<double>(m_feedRateInMmPerS);
    auto exitFeedRateInMmPerSDouble = static_cast<double>(m_exitFeedRateInMmPerS);

    Vector3<double> direction = (endPointDouble - startPointDouble).normalized();
    Vector3<double> absDirection(std::abs(direction.x), std::abs(direction.y), std::abs(direction.z));
    double xMmToTickScale = absDirection.x * xStepCountPerMm / tickFrequencyDouble;
    double yMmToTickScale = absDirection.y * yStepCountPerMm / tickFrequencyDouble;
    double zMmToTickScale = absDirection.z * zStepCountPerMm / tickFrequencyDouble;

    double accelerationDurationS = static_cast<double>(block.accelerationUntilTick) / tickFrequencyDouble;
    double accelerationInMmPerSS = (feedRateInMmPerSDouble - entryFeedRateInMmPerSDouble) / accelerationDurationS;

    if (std::isnan(accelerationInMmPerSS))
    {
        block.accelerationPerTick[AXIS_X_INDEX] = LinearBlockFixedPoint::ZERO;
        block.accelerationPerTick[AXIS_Y_INDEX] = LinearBlockFixedPoint::ZERO;
        block.accelerationPerTick[AXIS_Z_INDEX] = LinearBlockFixedPoint::ZERO;
    }
    else
    {
        block.accelerationPerTick[AXIS_X_INDEX] = accelerationInMmPerSS * xMmToTickScale;
        block.accelerationPerTick[AXIS_Y_INDEX] = accelerationInMmPerSS * yMmToTickScale;
        block.accelerationPerTick[AXIS_Z_INDEX] = accelerationInMmPerSS * zMmToTickScale;
    }


    // Set deceleration
    double decelerationDurationS =
        static_cast<double>(block.decelerationUntilTick - block.plateauUntilTick) / tickFrequencyDouble;
    double decelerationInMmPerSS = (exitFeedRateInMmPerSDouble - feedRateInMmPerSDouble) / decelerationDurationS;

    if (std::isnan(decelerationInMmPerSS))
    {
        block.decelerationPerTick[AXIS_X_INDEX] = LinearBlockFixedPoint::ZERO;
        block.decelerationPerTick[AXIS_Y_INDEX] = LinearBlockFixedPoint::ZERO;
        block.decelerationPerTick[AXIS_Z_INDEX] = LinearBlockFixedPoint::ZERO;
    }
    else
    {
        block.decelerationPerTick[AXIS_X_INDEX] = decelerationInMmPerSS * xMmToTickScale;
        block.decelerationPerTick[AXIS_Y_INDEX] = decelerationInMmPerSS * yMmToTickScale;
        block.decelerationPerTick[AXIS_Z_INDEX] = decelerationInMmPerSS * zMmToTickScale;
    }

    block.stepPerTick[AXIS_X_INDEX] = entryFeedRateInMmPerSDouble * xMmToTickScale;
    block.stepPerTick[AXIS_Y_INDEX] = entryFeedRateInMmPerSDouble * yMmToTickScale;
    block.stepPerTick[AXIS_Z_INDEX] = entryFeedRateInMmPerSDouble * zMmToTickScale;

    // Set step counter
    block.stepCounter[AXIS_X_INDEX] = LinearBlockFixedPoint::ZERO;
    block.stepCounter[AXIS_Y_INDEX] = LinearBlockFixedPoint::ZERO;
    block.stepCounter[AXIS_Z_INDEX] = LinearBlockFixedPoint::ZERO;

    // Set minStepPerTick
    double minFeedRateInMmPerSDouble = minFeedRateInMmPerS;
    block.minStepPerTick[AXIS_X_INDEX] = minFeedRateInMmPerSDouble * xMmToTickScale;
    block.minStepPerTick[AXIS_Y_INDEX] = minFeedRateInMmPerSDouble * yMmToTickScale;
    block.minStepPerTick[AXIS_Z_INDEX] = minFeedRateInMmPerSDouble * zMmToTickScale;


    // Set missing attributes
    block.durationUs =
        static_cast<uint32_t>(static_cast<double>(block.decelerationUntilTick) / tickFrequencyDouble * 1e6);
    block.spindleRpm = m_spindleRpm;

    return block;
}
