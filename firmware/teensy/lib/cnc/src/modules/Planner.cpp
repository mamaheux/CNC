#include <cnc/modules/Planner.h>
#include <cnc/modules/ModuleKernel.h>
#include <cnc/space.h>

#include <cstring>

#include <iostream> // TODO remove

constexpr const char* X_STEP_COUNT_PER_MM_KEY = "planner.x.step_count_per_mm";
constexpr const char* Y_STEP_COUNT_PER_MM_KEY = "planner.y.step_count_per_mm";
constexpr const char* Z_STEP_COUNT_PER_MM_KEY = "planner.z.step_count_per_mm";

constexpr const char* MIN_FEED_RATE_IN_MM_PER_S_KEY = "planner.min_feed_rate_in_mm_per_s";
constexpr const char* MAX_FEED_RATE_IN_MM_PER_S_KEY = "planner.max_feed_rate_in_mm_per_s";
constexpr const char* ACCELERATION_IN_MM_PER_SS_KEY = "planner.x_acceleration_in_mm_per_ss";
constexpr const char* JUNCTION_DEVIATION_KEY = "planner.junction_deviation";

constexpr const char* PENDING_LINE_MAX_DELAY_MS_KEY = "planner.pending_line_max_delay_ms";



// Inspired by https://onehossshay.wordpress.com/2011/09/24/improving_grbl_cornering_algorithm/
tl::optional<float> calculateJunctionFeedRateInMmPerS(const PlannerLine& currentLine,
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

LinearBlock lineToLinearBlock(const PlannerLine& line,
    float entryFeedRateInMmPerS,
    float exitFeedRateInMmPerS,
    float tickDurationUs)
{
    LinearBlock block;
    block.directions; // TODO

    block.currentTick = 0;
    block.accelerationUntilTick; // TODO
    block.plateauUntilTick; // TODO
    block.decelerationUntilTick; // TODO

    block.currentStepCount[AXIS_X_INDEX] = 0;
    block.currentStepCount[AXIS_Y_INDEX] = 0;
    block.currentStepCount[AXIS_Z_INDEX] = 0;
    block.totalStepCount; // TODO

    block.accelerationPerTick; // TODO
    block.decelerationPerTick; // TODO
    block.stepPerTick; // TODO

    block.stepCounter[AXIS_X_INDEX] = LinearBlockFixedPoint::ZERO;
    block.stepCounter[AXIS_Y_INDEX] = LinearBlockFixedPoint::ZERO;
    block.stepCounter[AXIS_Z_INDEX] = LinearBlockFixedPoint::ZERO;

    block.minStepPerTick; // TODO
    block.durationUs; // TODO
    block.spindleRpm; // TODO
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
