#ifndef MCU_MODULES_PLANNER_H
#define MCU_MODULES_PLANNER_H

#include "mcu/modules/LinearBlockExecutor.h"

#include <cnc/collections/BoundedQueue.h>
#include <cnc/math/Range.h>
#include <cnc/modules/Module.h>
#include <cnc/modules/Planner.h>
#include <cnc/modules/CoordinateTransformer.h>
#include <cnc/modules/ArcConverter.h>

constexpr size_t MAX_PLANNER_RESPONSE_SIZE = 128;

struct PendingLinearBlock
{
    LinearBlock block;
    tl::optional<CommandSource> source;
    tl::optional<uint32_t> commandId;
};

struct PendingGCode
{
    GCode gcode;
    CommandSource source;
    uint32_t commandId;
};


class Planner : public Module
{
    CoordinateTransformer* m_coordinateTransformer;
    ArcConverter* m_arcConverter;
    LinearBlockExecutor* m_linearBlockExecutor;

    char m_response[MAX_PLANNER_RESPONSE_SIZE];

    // Configuration
    tl::optional<float> m_xStepCountPerMm;
    tl::optional<float> m_yStepCountPerMm;
    tl::optional<float> m_zStepCountPerMm;

    tl::optional<float> m_minFeedRateInMmPerS;
    tl::optional<float> m_maxFeedRateInMmPerS;
    tl::optional<float> m_accelerationInMmPerSS;
    tl::optional<float> m_junctionDeviation;

    tl::optional<uint32_t> m_pendingLineDelayUs;
    tl::optional<uint32_t> m_queueMinDurationUs;
    tl::optional<uint32_t> m_queueMinSize;

    // Modal parameters
    InclusiveRange3<float> m_machineRange;
    Vector3<float> m_lastTargetPosition;

    float m_speedFactor;
    float m_g0FeedRateInMmPerS;
    float m_g1g2g3FeedRateInMmPerS;
    float m_lastExitFeedRateInMmPerS;

    // Planning attricutes
    tl::optional<PendingGCode> m_pendingGCode;

    uint32_t m_lastPendingLineTimeUs;
    tl::optional<PlannerLine> m_pendingLine;

    uint32_t m_lastPushTimeUs;
    uint32_t m_lastQueueDurationUs;
    size_t m_lastQueueSize;
    tl::optional<PendingLinearBlock> m_pendingLinearBlock;

public:
    Planner(CoordinateTransformer* coordinateTransformer, ArcConverter* arcConverter);
    ~Planner() override = default;

    DECLARE_NOT_COPYABLE(Planner);
    DECLARE_NOT_MOVABLE(Planner);

    void configure(const ConfigItem& item) override;
    void checkConfigErrors(const MissingConfigCallback& onMissingConfigItem) override;
    void begin() override;

    CommandResult onGCodeCommandReceived(const GCode& gcode, CommandSource source, uint32_t commandId) override;
    CommandResult onMCodeCommandReceived(const MCode& mcode, CommandSource source, uint32_t commandId) override;

    void update() override;

    bool isCncMoving() override;
    bool hasPendingMotionCommands() override;

    void reset(const Vector3<float>& machinePosition, const InclusiveRange3<float>& range);

    void setLinearBlockExecutor(LinearBlockExecutor* linearBlockExecutor);

    float xStepCountPerMm() const;
    float yStepCountPerMm() const;
    float zStepCountPerMm() const;

private:
    void sendLastTargetPositionInSelectedCoordinateSystem(CommandSource source, uint32_t commandId);
    void sendLastTargetPositionInMachineCoordinateSystem(CommandSource source, uint32_t commandId);

    void updateMaxFeedRateInMmPerS(const MCode& mcode);
    void updateAccelerationInMmPerSS(const MCode& mcode);
    void updateSpeedFactor(const MCode& mcode);

    void handlePendingLinearBlock();

    void handlePendingGCode();
    void handlePendingG0G1(float& modalFeedRateInMmPerS);
    void handlePendingG2G3();
    void handleNotFinishedArc();

    void handlePendingLine();

    Vector3<float> calculateEndPoint(const GCode& g0g1);
    float toLineFeedRate(float feedRate);

    void pushLine(const PlannerLine& line, CommandSource source, uint32_t commandId);
    bool pushLinearBlock(const LinearBlock& block);

    void sendOkForG0G1G2G3(CommandSource source, uint32_t commandId);

    void sendPosition(CommandSource source, uint32_t commandId, const Vector3<float>& position);
    void sendOkWithS(CommandSource source, uint32_t commandId, float sValue);
    void sendOkWithP(CommandSource source, uint32_t commandId, float pValue);
};

inline void Planner::setLinearBlockExecutor(LinearBlockExecutor* linearBlockExecutor)
{
    m_linearBlockExecutor = linearBlockExecutor;
}

inline float Planner::xStepCountPerMm() const
{
    return *m_xStepCountPerMm;
}

inline float Planner::yStepCountPerMm() const
{
    return *m_yStepCountPerMm;
}

inline float Planner::zStepCountPerMm() const
{
    return *m_zStepCountPerMm;
}

#endif
