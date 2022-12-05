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

// TODO remove comments
struct LinearBlock
{
    Direction directions[AXIS_COUNT]; // Handle

    uint64_t currentTick; // Handle
    uint64_t accelerationUntilTick; // Handle
    uint64_t plateauUntilTick; // Handle
    uint64_t decelerationUntilTick; // Handle

    uint32_t currentStepCount[AXIS_COUNT];
    uint32_t totalStepCount[AXIS_COUNT];

    LinearBlockFixedPoint accelerationPerTick[AXIS_COUNT]; // Handle
    LinearBlockFixedPoint decelerationPerTick[AXIS_COUNT]; // Handle
    LinearBlockFixedPoint stepPerTick[AXIS_COUNT];
    LinearBlockFixedPoint stepCounter[AXIS_COUNT];

    LinearBlockFixedPoint minStepPerTick; // Handle

    uint32_t durationUs;  // Handle
    tl::optional<float> spindleRpm;  // Handle
};

#endif
