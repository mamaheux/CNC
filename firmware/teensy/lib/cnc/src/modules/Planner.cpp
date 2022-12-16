#include <cnc/modules/Planner.h>
#include <cnc/modules/ModuleKernel.h>
#include <cnc/space.h>

#include <cstring>
#include <cmath>

#undef round
#undef abs

constexpr const char* X_STEP_COUNT_PER_MM_KEY = "planner.x.step_count_per_mm";
constexpr const char* Y_STEP_COUNT_PER_MM_KEY = "planner.y.step_count_per_mm";
constexpr const char* Z_STEP_COUNT_PER_MM_KEY = "planner.z.step_count_per_mm";

constexpr const char* MIN_FEED_RATE_IN_MM_PER_S_KEY = "planner.min_feed_rate_in_mm_per_s";
constexpr const char* MAX_FEED_RATE_IN_MM_PER_S_KEY = "planner.max_feed_rate_in_mm_per_s";
constexpr const char* ACCELERATION_IN_MM_PER_SS_KEY = "planner.acceleration_in_mm_per_ss";
constexpr const char* JUNCTION_DEVIATION_KEY = "planner.junction_deviation";

constexpr const char* PENDING_LINE_MAX_DELAY_MS_KEY = "planner.pending_line_max_delay_ms";

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
    float maxFeedRateAccelerationAndDeceleration = std::sqrt(da2 + squaredEntryFeedRateInMmPerS + squaredExitFeedRateInMmPerS) / 1.41421356237f;

    float distanceOnlyAcceleration = (squaredFeedRateInMmPerS - squaredEntryFeedRateInMmPerS) / (2.f * accelerationInMmPerSS);
    float distanceOnlyDeceleration = (squaredFeedRateInMmPerS - squaredExitFeedRateInMmPerS) / (2.f * accelerationInMmPerSS);

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

    int32_t startXInStep = static_cast<int32_t>(std::round(startPointDouble.x * xStepCountPerMmDouble));
    int32_t startYInStep = static_cast<int32_t>(std::round(startPointDouble.y * yStepCountPerMmDouble));
    int32_t startZInStep = static_cast<int32_t>(std::round(startPointDouble.z * zStepCountPerMmDouble));
    int32_t endXInStep = static_cast<int32_t>(std::round(endPointDouble.x * xStepCountPerMmDouble));
    int32_t endYInStep = static_cast<int32_t>(std::round(endPointDouble.y * yStepCountPerMmDouble));
    int32_t endZInStep = static_cast<int32_t>(std::round(endPointDouble.z * zStepCountPerMmDouble));
    block.totalStepCount[AXIS_X_INDEX] = std::abs(startXInStep - endXInStep);
    block.totalStepCount[AXIS_Y_INDEX] = std::abs(startYInStep - endYInStep);
    block.totalStepCount[AXIS_Z_INDEX] = std::abs(startZInStep - endZInStep);


    // Set acceleration
    double entryFeedRateInMmPerSDouble = static_cast<double>(m_entryFeedRateInMmPerS);
    double feedRateInMmPerSDouble = static_cast<double>(m_feedRateInMmPerS);
    double exitFeedRateInMmPerSDouble = static_cast<double>(m_exitFeedRateInMmPerS);

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
    block.durationUs = static_cast<uint32_t>(static_cast<double>(block.decelerationUntilTick) / tickFrequencyDouble * 1e6);
    block.spindleRpm = m_spindleRpm;

    return block;
}

FLASHMEM Planner::Planner(CoordinateTransformer* coordinateTransformer, ArcConverter* arcConverter)
    : m_coordinateTransformer(coordinateTransformer),
      m_arcConverter(arcConverter),
      m_machineRange(Vector3<float>(), Vector3<float>()),
      m_speedFactor(1.f),
      m_g0FeedRateInMmPerS(0.f)
{
}

FLASHMEM void Planner::configure(const ConfigItem& item)
{
    if (strcmp(item.getKey(), X_STEP_COUNT_PER_MM_KEY) == 0)
    {
        m_xStepCountPerMm = item.getValueFloat();
    }
    else if (strcmp(item.getKey(), Y_STEP_COUNT_PER_MM_KEY) == 0)
    {
        m_yStepCountPerMm = item.getValueFloat();
    }
    else if (strcmp(item.getKey(), Z_STEP_COUNT_PER_MM_KEY) == 0)
    {
        m_zStepCountPerMm = item.getValueFloat();
    }
    else if (strcmp(item.getKey(), MIN_FEED_RATE_IN_MM_PER_S_KEY) == 0)
    {
        m_minFeedRateInMmPerS = item.getValueFloat();
    }
    else if (strcmp(item.getKey(), MAX_FEED_RATE_IN_MM_PER_S_KEY) == 0)
    {
        m_maxFeedRateInMmPerS = item.getValueFloat();
    }
    else if (strcmp(item.getKey(), ACCELERATION_IN_MM_PER_SS_KEY) == 0)
    {
        m_accelerationInMmPerSS = item.getValueFloat();
    }
    else if (strcmp(item.getKey(), JUNCTION_DEVIATION_KEY) == 0)
    {
        m_junctionDeviation = item.getValueFloat();
    }
    else if (strcmp(item.getKey(), PENDING_LINE_MAX_DELAY_MS_KEY) == 0)
    {
        m_pendingLineMaxDelayMs = item.getValueInt();
    }
}

FLASHMEM void Planner::checkConfigErrors(const MissingConfigCallback& onMissingConfigItem)
{
    CHECK_CONFIG_ERROR(onMissingConfigItem, m_xStepCountPerMm.has_value(), X_STEP_COUNT_PER_MM_KEY);
    CHECK_CONFIG_ERROR(onMissingConfigItem, m_yStepCountPerMm.has_value(), Y_STEP_COUNT_PER_MM_KEY);
    CHECK_CONFIG_ERROR(onMissingConfigItem, m_zStepCountPerMm.has_value(), Z_STEP_COUNT_PER_MM_KEY);

    CHECK_CONFIG_ERROR(onMissingConfigItem, m_minFeedRateInMmPerS.has_value(), MIN_FEED_RATE_IN_MM_PER_S_KEY);
    CHECK_CONFIG_ERROR(onMissingConfigItem, m_maxFeedRateInMmPerS.has_value(), MAX_FEED_RATE_IN_MM_PER_S_KEY);
    CHECK_CONFIG_ERROR(onMissingConfigItem, m_accelerationInMmPerSS.has_value(), ACCELERATION_IN_MM_PER_SS_KEY);
    CHECK_CONFIG_ERROR(onMissingConfigItem, m_junctionDeviation.has_value(), JUNCTION_DEVIATION_KEY);

    CHECK_CONFIG_ERROR(onMissingConfigItem, m_pendingLineMaxDelayMs.has_value(), PENDING_LINE_MAX_DELAY_MS_KEY);
}

FLASHMEM void Planner::begin()
{
    m_g0FeedRateInMmPerS = *m_maxFeedRateInMmPerS;

    m_kernel->registerToEvent(ModuleEventType::GCODE_COMMAND, this);
    m_kernel->registerToEvent(ModuleEventType::MCODE_COMMAND, this);
}

CommandResult Planner::onGCodeCommandReceived(const GCode& gcode, CommandSource source, uint32_t commandId)
{
    if (gcode.code() == 0 || gcode.code() == 1 || gcode.code() == 2 || gcode.code() == 3 ||
        (gcode.code() == 28 && gcode.subcode() == tl::nullopt))
    {
        m_pendingGCode = {gcode, source, commandId};
        return CommandResult::pending();
    }
    else if (gcode.code() == 28 && gcode.subcode() == 1u)
    {
        m_g28TargetPosition = m_lastTargetPosition;
        return CommandResult::ok();
    }

    return CommandResult::notHandled();
}

CommandResult Planner::onMCodeCommandReceived(const MCode& mcode, CommandSource source, uint32_t commandId)
{
    if (mcode.code() == 114 && mcode.subcode() == tl::nullopt)
    {
        sendLastTargetPositionInSelectedCoordinateSystem(source, commandId);
    }
    else if (mcode.code() == 114 && mcode.subcode() == 2u)
    {
        sendLastTargetPositionInMachineCoordinateSystem(source, commandId);
    }
    else if (mcode.code() == 203)
    {
        updateMaxFeedRateInMmPerS(mcode);
        sendOkWithS(source, commandId, *m_maxFeedRateInMmPerS);
    }
    else if (mcode.code() == 204)
    {
        updateAccelerationInMmPerSS(mcode);
        sendOkWithS(source, commandId, *m_accelerationInMmPerSS);
    }
    else if (mcode.code() == 220)
    {
        updateSpeedFactor(mcode);
        sendOkWithS(source, commandId, m_speedFactor * 100.f);
    }
    else
    {
        return CommandResult::notHandled();
    }

    return CommandResult::okResponseSent();
}

void Planner::update()
{
    if (!m_arcConverter->isFinished())
    {
        // TODO completeArc
    }
    else if (m_pendingLine.has_value())
    {
        // TODO handle pending line
    }
    else if (m_pendingGCode.has_value())
    {
        // TODO handle gcode
    }
    // TODO handle pending gcode
    // TODO handle pending line

    // TODO handle g0 and g1 by creating a linear block
    // TODO handle g2 and g3 by creating linear blocks with ArcConverter
    // TODO hangle G28 and G28.1
    // TODO call m_kernel->dispatchTargetPosition

    // TODO max queue duration
}

bool Planner::hasPendingMotionCommands()
{
    return !m_arcConverter->isFinished() || m_pendingLine.has_value() || m_pendingGCode.has_value();
}

void Planner::reset(const Vector3<float>& machinePosition, const InclusiveRange3<float>& range)
{
    m_machineRange = range;
    m_lastTargetPosition = machinePosition;
    m_g28TargetPosition = tl::nullopt;
    m_kernel->dispatchTargetPosition(machinePosition);
}

void Planner::sendLastTargetPositionInSelectedCoordinateSystem(CommandSource source, uint32_t commandId)
{
    Vector3<float> position = m_coordinateTransformer->machineCoordinateToUserCurrentCoordinate(m_lastTargetPosition);
    sendPosition(source, commandId, position);
}

void Planner::sendLastTargetPositionInMachineCoordinateSystem(CommandSource source, uint32_t commandId)
{
    Vector3<float> position = m_coordinateTransformer->machineCoordinateToUserMachineCoordinate(m_lastTargetPosition);
    sendPosition(source, commandId, position);
}

void Planner::updateMaxFeedRateInMmPerS(const MCode& mcode)
{
    if (mcode.s().has_value())
    {
        m_maxFeedRateInMmPerS = *mcode.s();
    }
}

void Planner::updateAccelerationInMmPerSS(const MCode& mcode)
{
    if (mcode.s().has_value())
    {
        m_accelerationInMmPerSS = *mcode.s();
    }
}

void Planner::updateSpeedFactor(const MCode& mcode)
{
    if (mcode.s().has_value())
    {
        m_speedFactor = *mcode.s() / 100.f;
    }
}
