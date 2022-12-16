#ifndef CNC_LINEAR_BLOCK_H
#define CNC_LINEAR_BLOCK_H

#include <cnc/math/FixedPoint.h>

#include <tl/optional.hpp>

enum class Direction : bool
{
    FORWARD = 0,
    BACKWARD = 1
};

enum class Axis
{
    X = 0,
    Y = 1,
    Z = 2
};

constexpr size_t AXIS_X_INDEX = static_cast<size_t>(Axis::X);
constexpr size_t AXIS_Y_INDEX = static_cast<size_t>(Axis::Y);
constexpr size_t AXIS_Z_INDEX = static_cast<size_t>(Axis::Z);
constexpr size_t AXIS_COUNT = 3;

typedef FixedPoint<1, 62> LinearBlockFixedPoint;

struct LinearBlock
{
    Direction directions[AXIS_COUNT];

    uint64_t currentTick;
    uint64_t accelerationUntilTick;
    uint64_t plateauUntilTick;
    uint64_t decelerationUntilTick;

    int32_t currentStepCount[AXIS_COUNT];
    int32_t totalStepCount[AXIS_COUNT];

    LinearBlockFixedPoint accelerationPerTick[AXIS_COUNT];
    LinearBlockFixedPoint decelerationPerTick[AXIS_COUNT];
    LinearBlockFixedPoint stepPerTick[AXIS_COUNT];
    LinearBlockFixedPoint stepCounter[AXIS_COUNT];

    LinearBlockFixedPoint minStepPerTick[AXIS_COUNT];

    uint32_t durationUs;
    tl::optional<float> spindleRpm;
};

#endif
