#ifndef CNC_MODULES_PLANNER_H
#define CNC_MODULES_PLANNER_H

#include <cnc/LinearBlock.h>
#include <cnc/math/Vector3.h>

#include <tl/optional.hpp>

struct PlannerLine
{
    Vector3<float> startPoint;
    Vector3<float> endPoint;

    float feedRateInMmPerS;
    tl::optional<float> spindleRpm;
};

class PlannerBlock
{
    Vector3<float> m_startPoint;
    Vector3<float> m_endPoint;
    float m_distance;

    Direction m_directions[AXIS_COUNT];

    float m_entryFeedRateInMmPerS;
    float m_feedRateInMmPerS;
    float m_exitFeedRateInMmPerS;

    float m_accelerationDurationS;
    float m_plateauDurationS;
    float m_decelerationDurationS;

    tl::optional<float> m_spindleRpm;

    PlannerBlock() = default;

public:
    static tl::optional<PlannerBlock> fromLine(
        const PlannerLine& line,
        float entryFeedRateInMmPerS,
        float exitFeedRateInMmPerS,
        float accelerationInMmPerSS);

    Vector3<float> startPoint() const;
    Vector3<float> endPoint() const;
    float distance() const;

    Direction directions(Axis axis) const;

    float accelerationInMmPerSS() const;
    float entryFeedRateInMmPerS() const;
    float feedRateInMmPerS() const;
    float exitFeedRateInMmPerS() const;

    float accelerationDurationS() const;
    float plateauDurationS() const;
    float decelerationDurationS() const;

    tl::optional<float> spindleRpm() const;

    LinearBlock toLinearBlock(
        float xStepCountPerMm,
        float yStepCountPerMm,
        float zStepCountPerMm,
        float minFeedRateInMmPerS,
        float tickFrequency);

private:
    void setDirectionFromLine(const PlannerLine& line);
    void setAccelerationAndFeedRates(
        float entryFeedRateInMmPerS,
        float feedRateInMmPerS,
        float exitFeedRateInMmPerS,
        float accelerationInMmPerSS);
    void calculateTrapezoid(float accelerationInMmPerSS);
};

inline Vector3<float> PlannerBlock::startPoint() const
{
    return m_startPoint;
}

inline Vector3<float> PlannerBlock::endPoint() const
{
    return m_endPoint;
}

inline float PlannerBlock::distance() const
{
    return m_distance;
}

inline Direction PlannerBlock::directions(Axis axis) const
{
    return m_directions[static_cast<size_t>(axis)];
}

inline float PlannerBlock::entryFeedRateInMmPerS() const
{
    return m_entryFeedRateInMmPerS;
}

inline float PlannerBlock::feedRateInMmPerS() const
{
    return m_feedRateInMmPerS;
}

inline float PlannerBlock::exitFeedRateInMmPerS() const
{
    return m_exitFeedRateInMmPerS;
}

inline float PlannerBlock::accelerationDurationS() const
{
    return m_accelerationDurationS;
}

inline float PlannerBlock::plateauDurationS() const
{
    return m_plateauDurationS;
}

inline float PlannerBlock::decelerationDurationS() const
{
    return m_decelerationDurationS;
}

inline tl::optional<float> PlannerBlock::spindleRpm() const
{
    return m_spindleRpm;
}


tl::optional<float> calculateJunctionFeedRateInMmPerS(
    const PlannerLine& currentLine,
    const PlannerLine& nextLine,
    float maxAccelerationInMmPerSS,
    float deviationInMm);

#endif
