#include "mcu/modules/Planner.h"
#include "mcu/utils/StringPrint.h"
#include "mcu/criticalError.h"

#include <cnc/modules/ModuleKernel.h>

constexpr const char* X_STEP_COUNT_PER_MM_KEY = "planner.x.step_count_per_mm";
constexpr const char* Y_STEP_COUNT_PER_MM_KEY = "planner.y.step_count_per_mm";
constexpr const char* Z_STEP_COUNT_PER_MM_KEY = "planner.z.step_count_per_mm";

constexpr const char* MIN_FEED_RATE_IN_MM_PER_S_KEY = "planner.min_feed_rate_in_mm_per_s";
constexpr const char* MAX_FEED_RATE_IN_MM_PER_S_KEY = "planner.max_feed_rate_in_mm_per_s";
constexpr const char* ACCELERATION_IN_MM_PER_SS_KEY = "planner.acceleration_in_mm_per_ss";
constexpr const char* JUNCTION_DEVIATION_KEY = "planner.junction_deviation";

constexpr const char* PENDING_LINE_DELAY_US_KEY = "planner.pending_line_delay_us";
constexpr const char* QUEUE_MIN_DURATION_US_KEY = "planner.queue_min_duration_us";
constexpr const char* QUEUE_MIN_SIZE_KEY = "planner.queue_min_size";

constexpr const char* OUT_OF_RANGE_COMMAND_RESPONSE = "error The position is out of range.";

FLASHMEM Planner::Planner(CoordinateTransformer* coordinateTransformer, ArcConverter* arcConverter)
    : m_coordinateTransformer(coordinateTransformer),
      m_arcConverter(arcConverter),
      m_linearBlockExecutor(nullptr),
      m_machineRange(Vector3<float>(), Vector3<float>()),
      m_speedFactor(1.f),
      m_feedRateScale(1.f),
      m_g0FeedRateInMmPerS(0.f),
      m_g1g2g3FeedRateInMmPerS(0.f),
      m_lastExitFeedRateInMmPerS(0.f),
      m_lastPendingLineTimeUs(0),
      m_lastPushTimeUs(0),
      m_lastQueueDurationUs(0),
      m_lastQueueSize(0)
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
    else if (strcmp(item.getKey(), PENDING_LINE_DELAY_US_KEY) == 0)
    {
        m_pendingLineDelayUs = item.getValueInt();
    }
    else if (strcmp(item.getKey(), QUEUE_MIN_DURATION_US_KEY) == 0)
    {
        m_queueMinDurationUs = item.getValueInt();
    }
    else if (strcmp(item.getKey(), QUEUE_MIN_SIZE_KEY) == 0)
    {
        m_queueMinSize = item.getValueInt();
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

    CHECK_CONFIG_ERROR(onMissingConfigItem, m_pendingLineDelayUs.has_value(), PENDING_LINE_DELAY_US_KEY);
    CHECK_CONFIG_ERROR(onMissingConfigItem, m_queueMinDurationUs.has_value(), QUEUE_MIN_DURATION_US_KEY);
    CHECK_CONFIG_ERROR(onMissingConfigItem, m_queueMinSize.has_value(), QUEUE_MIN_SIZE_KEY);
}

FLASHMEM void Planner::begin()
{
    m_g0FeedRateInMmPerS = *m_maxFeedRateInMmPerS;
    m_g1g2g3FeedRateInMmPerS = *m_minFeedRateInMmPerS;
    m_lastExitFeedRateInMmPerS = *m_minFeedRateInMmPerS;

    m_kernel->registerToEvent(ModuleEventType::GCODE_COMMAND, this);
    m_kernel->registerToEvent(ModuleEventType::MCODE_COMMAND, this);
}

CommandResult Planner::onGCodeCommandReceived(const GCode& gcode, CommandSource source, uint32_t commandId)
{
    if (gcode.code() == 0 || gcode.code() == 1 || gcode.code() == 2 || gcode.code() == 3)
    {
        m_pendingGCode = {gcode, source, commandId};
        return CommandResult::pending();
    }
    else if (gcode.code() == 20 && gcode.subcode() == tl::nullopt)
    {
        m_feedRateScale = INCH_TO_MM_SCALE;
    }
    else if (gcode.code() == 21 && gcode.subcode() == tl::nullopt)
    {
        m_feedRateScale = 1.f;
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
    if (m_pendingLinearBlock.has_value())
    {
        handlePendingLinearBlock();
    }
    else if (!m_arcConverter->isFinished())
    {
        handleNotFinishedArc();
    }
    else if (m_pendingGCode.has_value())
    {
        handlePendingGCode();
    }
    else if (m_pendingLine.has_value())
    {
        handlePendingLine();
    }
}

bool Planner::isCncMoving()
{
    return hasPendingMotionCommands();
}

bool Planner::hasPendingMotionCommands()
{
    return !m_arcConverter->isFinished() || m_pendingLinearBlock.has_value() || m_pendingGCode.has_value() ||
           m_pendingLine.has_value();
}

void Planner::reset(const Vector3<float>& machinePosition, const InclusiveRange3<float>& range)
{
    m_machineRange = range;
    m_lastTargetPosition = machinePosition;
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

void Planner::handlePendingLinearBlock()
{
    if ((micros() - m_lastPushTimeUs) < (m_lastQueueDurationUs / 4))
    {
        return;
    }

    if (pushLinearBlock(m_pendingLinearBlock->block))
    {
        m_pendingLinearBlock = tl::nullopt;

        if (m_pendingLinearBlock->source.has_value() && m_pendingLinearBlock->commandId.has_value() &&
            m_arcConverter->isFinished())
        {
            sendOkForG0G1G2G3(*m_pendingLinearBlock->source, *m_pendingLinearBlock->commandId);
            m_pendingGCode = tl::nullopt;
        }
    }
}

void Planner::handlePendingGCode()
{
    switch (m_pendingGCode->gcode.code())
    {
        case 0:
            handlePendingG0G1(m_g0FeedRateInMmPerS);
            break;

        case 1:
            handlePendingG0G1(m_g1g2g3FeedRateInMmPerS);
            break;

        case 2:
        case 3:
            handlePendingG2G3();
            break;
    }
}

void Planner::handlePendingG0G1(float& modalFeedRateInMmPerS)
{
    auto endPoint = calculateEndPoint(m_pendingGCode->gcode);
    if (!m_machineRange.contains(endPoint))
    {
        m_kernel->sendCommandResponse(OUT_OF_RANGE_COMMAND_RESPONSE, m_pendingGCode->source, m_pendingGCode->commandId);
        m_pendingGCode = tl::nullopt;
        return;
    }

    float feedRateInMmPerS;
    if (m_pendingGCode->gcode.f().has_value())
    {
        feedRateInMmPerS = std::max(*m_pendingGCode->gcode.f() * m_feedRateScale / 60.f, *m_minFeedRateInMmPerS);
        modalFeedRateInMmPerS = feedRateInMmPerS;
    }

    PlannerLine line{m_lastTargetPosition, endPoint, toLineFeedRate(modalFeedRateInMmPerS), m_pendingGCode->gcode.s()};
    pushLine(line, m_pendingGCode->source, m_pendingGCode->commandId);

    m_lastTargetPosition = endPoint;
    m_kernel->dispatchTargetPosition(m_lastTargetPosition);

    if (m_pendingLinearBlock == tl::nullopt)
    {
        sendOkForG0G1G2G3(m_pendingGCode->source, m_pendingGCode->commandId);
        m_pendingGCode = tl::nullopt;
    }
}

void Planner::handlePendingG2G3()
{
    CommandResult result = m_arcConverter->setArc(m_pendingGCode->gcode);
    if (result.type() == CommandResultType::ERROR)
    {
        m_kernel->sendCommandResponse("error ", m_pendingGCode->source, m_pendingGCode->commandId, false);
        m_kernel->sendCommandResponse(result.errorMessage(), m_pendingGCode->source, m_pendingGCode->commandId);
    }

    if (m_pendingGCode->gcode.f().has_value())
    {
        m_g1g2g3FeedRateInMmPerS = std::max(*m_pendingGCode->gcode.f() / 60.f, *m_minFeedRateInMmPerS);
    }
    handleNotFinishedArc();
}

void Planner::handleNotFinishedArc()
{
    GCode lineGCode;
    while (m_arcConverter->getNextSegment(lineGCode) && m_pendingLinearBlock == tl::nullopt)
    {
        auto endPoint = calculateEndPoint(lineGCode);
        if (!m_machineRange.contains(endPoint))
        {
            m_kernel->sendCommandResponse(
                OUT_OF_RANGE_COMMAND_RESPONSE,
                m_pendingGCode->source,
                m_pendingGCode->commandId);
            m_pendingGCode = tl::nullopt;
            m_arcConverter->clear();
            return;
        }

        PlannerLine line{
            m_lastTargetPosition,
            endPoint,
            toLineFeedRate(m_g1g2g3FeedRateInMmPerS),
            m_pendingGCode->gcode.s()};
        pushLine(line, m_pendingGCode->source, m_pendingGCode->commandId);

        m_lastTargetPosition = endPoint;
        m_kernel->dispatchTargetPosition(m_lastTargetPosition);
    }

    if (m_arcConverter->isFinished() && m_pendingLinearBlock == tl::nullopt)
    {
        sendOkForG0G1G2G3(m_pendingGCode->source, m_pendingGCode->commandId);
        m_pendingGCode = tl::nullopt;
    }
}

void Planner::handlePendingLine()
{
    int64_t pendingLineDelayUs = std::max(
        static_cast<int64_t>(m_lastQueueDurationUs) - static_cast<int64_t>(*m_queueMinDurationUs),
        static_cast<int64_t>(*m_pendingLineDelayUs));

    if (static_cast<int64_t>(micros() - m_lastPendingLineTimeUs) < pendingLineDelayUs)
    {
        return;
    }

    auto plannerBlock = PlannerBlock::fromLine(
        *m_pendingLine,
        m_lastExitFeedRateInMmPerS,
        *m_minFeedRateInMmPerS,
        *m_accelerationInMmPerSS);
    CRITICAL_ERROR_CHECK(plannerBlock.has_value(), "PlannerBlock::fromLine failed");
    m_lastExitFeedRateInMmPerS = *m_minFeedRateInMmPerS;

    auto linearBlock = plannerBlock->toLinearBlock(
        *m_xStepCountPerMm,
        *m_yStepCountPerMm,
        *m_zStepCountPerMm,
        *m_minFeedRateInMmPerS,
        m_linearBlockExecutor->tickFrequency());

    if (!pushLinearBlock(linearBlock))
    {
        m_pendingLinearBlock = PendingLinearBlock{linearBlock, tl::nullopt, tl::nullopt};
    }

    m_pendingLine = tl::nullopt;
}

Vector3<float> Planner::calculateEndPoint(const GCode& g0g1)
{
    if (g0g1.isMachineCoordinateSystem())
    {
        float x = *g0g1.x().or_else([this]() { return m_lastTargetPosition.x; });
        float y = *g0g1.y().or_else([this]() { return m_lastTargetPosition.y; });
        float z = *g0g1.z().or_else([this]() { return m_lastTargetPosition.z; });
        return Vector3<float>(x, y, z);
    }
    else
    {
        auto gcodeStartPoint = m_coordinateTransformer->machineCoordinateToGcode(m_lastTargetPosition);
        float x = *g0g1.x().or_else([gcodeStartPoint]() { return gcodeStartPoint.x; });
        float y = *g0g1.y().or_else([gcodeStartPoint]() { return gcodeStartPoint.y; });
        float z = *g0g1.z().or_else([gcodeStartPoint]() { return gcodeStartPoint.z; });
        return m_coordinateTransformer->gcodeCoordinateToMachineCoordinate(Vector3<float>(x, y, z));
    }
}

float Planner::toLineFeedRate(float feedRate)
{
    float lineFeedRate = feedRate * m_speedFactor;
    return std::max(*m_minFeedRateInMmPerS, std::min(lineFeedRate, *m_maxFeedRateInMmPerS));
}

void Planner::pushLine(const PlannerLine& line, CommandSource source, uint32_t commandId)
{
    if (m_pendingLine == tl::nullopt)
    {
        m_lastPendingLineTimeUs = micros();
        m_pendingLine = line;
        return;
    }

    auto exitFeedRateInMmPerS =
        calculateJunctionFeedRateInMmPerS(*m_pendingLine, line, *m_accelerationInMmPerSS, *m_junctionDeviation);
    CRITICAL_ERROR_CHECK(exitFeedRateInMmPerS.has_value(), "calculateJunctionFeedRateInMmPerS failed");

    auto plannerBlock = PlannerBlock::fromLine(
        *m_pendingLine,
        m_lastExitFeedRateInMmPerS,
        *exitFeedRateInMmPerS,
        *m_accelerationInMmPerSS);
    CRITICAL_ERROR_CHECK(plannerBlock.has_value(), "PlannerBlock::fromLine failed");
    m_lastExitFeedRateInMmPerS = *exitFeedRateInMmPerS;

    auto linearBlock = plannerBlock->toLinearBlock(
        *m_xStepCountPerMm,
        *m_yStepCountPerMm,
        *m_zStepCountPerMm,
        *m_minFeedRateInMmPerS,
        m_linearBlockExecutor->tickFrequency());
    if (!pushLinearBlock(linearBlock))
    {
        m_pendingLinearBlock = PendingLinearBlock{linearBlock, source, commandId};
    }

    m_lastPendingLineTimeUs = micros();
    m_pendingLine = line;
}

bool Planner::pushLinearBlock(const LinearBlock& block)
{
    m_lastPushTimeUs = micros();
    return m_kernel->dispatchLinearBlock(block, m_lastQueueDurationUs, m_lastQueueSize);
}

void Planner::sendOkForG0G1G2G3(CommandSource source, uint32_t commandId)
{
    float sleepDurationMs = 0.f;
    if (m_lastQueueSize > *m_queueMinSize && m_lastQueueDurationUs > (*m_queueMinDurationUs + *m_pendingLineDelayUs))
    {
        sleepDurationMs = (m_lastQueueDurationUs - *m_queueMinDurationUs - *m_pendingLineDelayUs) / 1000.f;
    }

    sendOkWithP(source, commandId, sleepDurationMs);
}

void Planner::sendPosition(CommandSource source, uint32_t commandId, const Vector3<float>& position)
{
    StringPrint stringPrint(m_response, MAX_PLANNER_RESPONSE_SIZE);
    stringPrint.print(OK_COMMAND_RESPONSE);
    stringPrint.print(" X");
    stringPrint.print(position.x);
    stringPrint.print(" Y");
    stringPrint.print(position.y);
    stringPrint.print(" Z");
    stringPrint.print(position.z);
    stringPrint.finish();

    m_kernel->sendCommandResponse(m_response, source, commandId);
}

void Planner::sendOkWithS(CommandSource source, uint32_t commandId, float sValue)
{
    StringPrint stringPrint(m_response, MAX_PLANNER_RESPONSE_SIZE);
    stringPrint.print(OK_COMMAND_RESPONSE);
    stringPrint.print(" S");
    stringPrint.print(sValue);
    stringPrint.finish();

    m_kernel->sendCommandResponse(m_response, source, commandId);
}

void Planner::sendOkWithP(CommandSource source, uint32_t commandId, float pValue)
{
    StringPrint stringPrint(m_response, MAX_PLANNER_RESPONSE_SIZE);
    stringPrint.print(OK_COMMAND_RESPONSE);
    stringPrint.print(" P");
    stringPrint.print(pValue);
    stringPrint.finish();

    m_kernel->sendCommandResponse(m_response, source, commandId);
}
