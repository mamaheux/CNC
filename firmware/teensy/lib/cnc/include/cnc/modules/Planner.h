#ifndef CNC_MODULES_PLANNER_H
#define CNC_MODULES_PLANNER_H

#include <cnc/LinearBlock.h>
#include <cnc/collections/BoundedQueue.h>
#include <cnc/math/Range.h>
#include <cnc/modules/Module.h>
#include <cnc/modules/CoordinateTransformer.h>
#include <cnc/modules/ArcConverter.h>

#include <tl/optional.hpp>

struct PlannerLine
{
    Vector3<float> startPoint;
    Vector3<float> endPoint;

    float m_feedRateInMmPerS;
    tl::optional<float> spindleRpm;
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

    tl::optional<float> m_xStepCountPerMm;
    tl::optional<float> m_yStepCountPerMm;
    tl::optional<float> m_zStepCountPerMm;
    tl::optional<float> m_minFeedRateInMmPerS;
    tl::optional<float> m_maxFeedRateInMmPerS;
    tl::optional<float> m_accelerationInMmPerSS;
    tl::optional<float> m_junctionDeviation;
    tl::optional<uint32_t> m_pendingLineDelayMs;

    InclusiveRange3<float> m_machineRange;  // TODO use
    Vector3<float> m_lastTargetPosition;
    tl::optional<Vector3<float>> m_g28TargetPosition;

    float m_speedFactor;  // TODO use
    float m_g0FeedRateInMmPerS;  // TODO use
    tl::optional<float> m_g1g2g3FeedRateInMmPerS;  // TODO use

    tl::optional<PlannerLine> m_pendingLine;
    tl::optional<PendingGCode> m_pendingGCode;

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

    bool hasPendingMotionCommands() override;

    void reset(const Vector3<float>& machinePosition, const InclusiveRange3<float>& range);
    float xStepCountPerMm() const;
    float yStepCountPerMm() const;
    float zStepCountPerMm() const;

private:
    void sendLastTargetPositionInSelectedCoordinateSystem(CommandSource source, uint32_t commandId);
    void sendLastTargetPositionInMachineCoordinateSystem(CommandSource source, uint32_t commandId);

    void updateMaxFeedRateInMmPerS(const MCode& mcode);
    void updateAccelerationInMmPerSS(const MCode& mcode);
    void updateSpeedFactor(const MCode& mcode);

protected:
    virtual void sendPosition(CommandSource source, uint32_t commandId, const Vector3<float>& position) = 0;
    virtual void sendOkWithS(CommandSource source, uint32_t commandId, float sValue) = 0;
    virtual void sendOkWithP(CommandSource source, uint32_t commandId, float pValue) = 0;
};

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
