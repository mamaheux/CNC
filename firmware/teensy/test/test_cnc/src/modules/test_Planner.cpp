#include <cnc/modules/Planner.h>
#include <unity.h>

constexpr float MAX_DELTA = 1e-5;

constexpr float MAX_ACCELERATION_IN_MM_PER_SS = 200;
constexpr float MAX_DEVIATION_IN_MM = 0.005;

void test_junctionFeedRateInMmPerS_notValidLines()
{
    PlannerLine currentLine{{0.f, 0.f, 0.f}, {1.f, 1.f, 0.f}, 10, tl::nullopt};
    PlannerLine nextLine{{1.1f, 1.1f, 0.f}, {2.f, 2.f, 0.f}, 10, tl::nullopt};

    auto junctionFeedRate = calculateJunctionFeedRateInMmPerS(currentLine, nextLine, MAX_ACCELERATION_IN_MM_PER_SS, MAX_DEVIATION_IN_MM);

    TEST_ASSERT(tl::nullopt == junctionFeedRate);
}

void test_junctionFeedRateInMmPerS_0Deg()
{
    PlannerLine currentLine{{0.f, 0.f, 0.f}, {1.f, 1.f, 0.f}, 10, tl::nullopt};
    PlannerLine nextLine{{1.f, 1.f, 0.f}, {2.f, 2.f, 0.f}, 5, tl::nullopt};

    auto junctionFeedRate = calculateJunctionFeedRateInMmPerS(currentLine, nextLine, MAX_ACCELERATION_IN_MM_PER_SS, MAX_DEVIATION_IN_MM);

    TEST_ASSERT_TRUE(junctionFeedRate.has_value());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 5.f, *junctionFeedRate);
}

void test_junctionFeedRateInMmPerS_45Deg()
{
    PlannerLine currentLine{{0.f, 0.f, 0.f}, {1.f, 1.f, 0.f}, 10, tl::nullopt};
    PlannerLine nextLine{{1.f, 1.f, 0.f}, {2.f, 1.f, 0.f}, 5, tl::nullopt};

    auto junctionFeedRate = calculateJunctionFeedRateInMmPerS(currentLine, nextLine, MAX_ACCELERATION_IN_MM_PER_SS, MAX_DEVIATION_IN_MM);

    TEST_ASSERT_TRUE(junctionFeedRate.has_value());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 3.483818, *junctionFeedRate);
}

void test_junctionFeedRateInMmPerS_90Deg()
{
    PlannerLine currentLine{{0.f, 0.f, 0.f}, {-1.f, 0.f, 0.f}, 10, tl::nullopt};
    PlannerLine nextLine{{-1.f, 0.f, 0.f}, {-1.f, 1.f, 0.f}, 5, tl::nullopt};

    auto junctionFeedRate = calculateJunctionFeedRateInMmPerS(currentLine, nextLine, MAX_ACCELERATION_IN_MM_PER_SS, MAX_DEVIATION_IN_MM);

    TEST_ASSERT_TRUE(junctionFeedRate.has_value());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.553774, *junctionFeedRate);
}

// TODO next tests
void test_junctionFeedRateInMmPerS_135Deg()
{
    PlannerLine currentLine{{1.f, 1.f, 0.f}, {0.f, 0.f, 0.f}, 10, tl::nullopt};
    PlannerLine nextLine{{0.f, 0.f, 0.f}, {0.f, 1.f, 0.f}, 5, tl::nullopt};

    auto junctionFeedRate = calculateJunctionFeedRateInMmPerS(currentLine, nextLine, MAX_ACCELERATION_IN_MM_PER_SS, MAX_DEVIATION_IN_MM);

    TEST_ASSERT_TRUE(junctionFeedRate.has_value());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.7873485, *junctionFeedRate);
}

void test_junctionFeedRateInMmPerS_180Deg()
{
    PlannerLine currentLine{{0.f, 1.f, 1.f}, {0.f, 0.f, 0.f}, 10, tl::nullopt};
    PlannerLine nextLine{{0.f, 0.f, 0.f}, {0.f, 1.f, 1.f}, 5, tl::nullopt};

    auto junctionFeedRate = calculateJunctionFeedRateInMmPerS(currentLine, nextLine, MAX_ACCELERATION_IN_MM_PER_SS, MAX_DEVIATION_IN_MM);

    TEST_ASSERT_TRUE(junctionFeedRate.has_value());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.03337785, *junctionFeedRate);
}
