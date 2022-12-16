#include <cnc/modules/Planner.h>
#include <unity.h>

constexpr float MAX_DELTA = 1e-5;

constexpr float MAX_ACCELERATION_IN_MM_PER_SS = 200;
constexpr float MAX_DEVIATION_IN_MM = 0.005;

void test_junctionFeedRateInMmPerS_notValidLines()
{
    PlannerLine currentLine{{0.f, 0.f, 0.f}, {1.f, 1.f, 0.f}, 10, tl::nullopt};
    PlannerLine nextLine{{1.1f, 1.1f, 0.f}, {2.f, 2.f, 0.f}, 10, tl::nullopt};

    auto junctionFeedRate =
        calculateJunctionFeedRateInMmPerS(currentLine, nextLine, MAX_ACCELERATION_IN_MM_PER_SS, MAX_DEVIATION_IN_MM);

    TEST_ASSERT(tl::nullopt == junctionFeedRate);
}

void test_junctionFeedRateInMmPerS_0Deg()
{
    PlannerLine currentLine{{0.f, 0.f, 0.f}, {1.f, 1.f, 0.f}, 10, tl::nullopt};
    PlannerLine nextLine{{1.f, 1.f, 0.f}, {2.f, 2.f, 0.f}, 5, tl::nullopt};

    auto junctionFeedRate =
        calculateJunctionFeedRateInMmPerS(currentLine, nextLine, MAX_ACCELERATION_IN_MM_PER_SS, MAX_DEVIATION_IN_MM);

    TEST_ASSERT_TRUE(junctionFeedRate.has_value());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 5.f, *junctionFeedRate);
}

void test_junctionFeedRateInMmPerS_45Deg()
{
    PlannerLine currentLine{{0.f, 0.f, 0.f}, {1.f, 1.f, 0.f}, 10, tl::nullopt};
    PlannerLine nextLine{{1.f, 1.f, 0.f}, {2.f, 1.f, 0.f}, 5, tl::nullopt};

    auto junctionFeedRate =
        calculateJunctionFeedRateInMmPerS(currentLine, nextLine, MAX_ACCELERATION_IN_MM_PER_SS, MAX_DEVIATION_IN_MM);

    TEST_ASSERT_TRUE(junctionFeedRate.has_value());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 3.483818, *junctionFeedRate);
}

void test_junctionFeedRateInMmPerS_90Deg()
{
    PlannerLine currentLine{{0.f, 0.f, 0.f}, {-1.f, 0.f, 0.f}, 10, tl::nullopt};
    PlannerLine nextLine{{-1.f, 0.f, 0.f}, {-1.f, 1.f, 0.f}, 5, tl::nullopt};

    auto junctionFeedRate =
        calculateJunctionFeedRateInMmPerS(currentLine, nextLine, MAX_ACCELERATION_IN_MM_PER_SS, MAX_DEVIATION_IN_MM);

    TEST_ASSERT_TRUE(junctionFeedRate.has_value());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.553774, *junctionFeedRate);
}

void test_junctionFeedRateInMmPerS_135Deg()
{
    PlannerLine currentLine{{1.f, 1.f, 0.f}, {0.f, 0.f, 0.f}, 10, tl::nullopt};
    PlannerLine nextLine{{0.f, 0.f, 0.f}, {0.f, 1.f, 0.f}, 5, tl::nullopt};

    auto junctionFeedRate =
        calculateJunctionFeedRateInMmPerS(currentLine, nextLine, MAX_ACCELERATION_IN_MM_PER_SS, MAX_DEVIATION_IN_MM);

    TEST_ASSERT_TRUE(junctionFeedRate.has_value());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.7873485, *junctionFeedRate);
}

void test_junctionFeedRateInMmPerS_180Deg()
{
    PlannerLine currentLine{{0.f, 1.f, 1.f}, {0.f, 0.f, 0.f}, 10, tl::nullopt};
    PlannerLine nextLine{{0.f, 0.f, 0.f}, {0.f, 1.f, 1.f}, 5, tl::nullopt};

    auto junctionFeedRate =
        calculateJunctionFeedRateInMmPerS(currentLine, nextLine, MAX_ACCELERATION_IN_MM_PER_SS, MAX_DEVIATION_IN_MM);

    TEST_ASSERT_TRUE(junctionFeedRate.has_value());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.03337785, *junctionFeedRate);
}

void test_PlannerBlockFromLine_invalid()
{
    constexpr float VALID_ENTRY_FEED_RATE_IN_MM_PER_S = 1.5f;
    constexpr float INVALID_ENTRY_FEED_RATE_IN_MM_PER_S = 2.1f;
    constexpr float FEED_RATE_IN_MM_PER_S = 2.f;
    constexpr float VALID_EXIT_FEED_RATE_IN_MM_PER_S = 1.75f;
    constexpr float INVALID_EXIT_FEED_RATE_IN_MM_PER_S = 2.2f;
    constexpr float ACCELERATION_IN_MM_PER_SS = 200.f;

    PlannerLine line{{0.f, 0.f, 0.f}, {10.f, 10.f, 0.f}, FEED_RATE_IN_MM_PER_S, tl::nullopt};
    TEST_ASSERT_FALSE(PlannerBlock::fromLine(line, INVALID_ENTRY_FEED_RATE_IN_MM_PER_S, VALID_EXIT_FEED_RATE_IN_MM_PER_S, ACCELERATION_IN_MM_PER_SS).has_value());
    TEST_ASSERT_FALSE(PlannerBlock::fromLine(line, VALID_ENTRY_FEED_RATE_IN_MM_PER_S, INVALID_EXIT_FEED_RATE_IN_MM_PER_S, ACCELERATION_IN_MM_PER_SS).has_value());
}

void test_PlannerBlockFromLine_directions()
{
    constexpr float ENTRY_FEED_RATE_IN_MM_PER_S = 1.f;
    constexpr float FEED_RATE_IN_MM_PER_S = 2.f;
    constexpr float EXIT_FEED_RATE_IN_MM_PER_S = 1.5f;
    constexpr float ACCELERATION_IN_MM_PER_SS = 200.f;

    PlannerLine line1{{0.f, 0.f, 0.f}, {0.f, 0.f, 0.f}, FEED_RATE_IN_MM_PER_S, tl::nullopt};
    auto block1 = PlannerBlock::fromLine(line1, ENTRY_FEED_RATE_IN_MM_PER_S, EXIT_FEED_RATE_IN_MM_PER_S, ACCELERATION_IN_MM_PER_SS);
    TEST_ASSERT_TRUE(block1.has_value());
    TEST_ASSERT_EQUAL(Direction::FORWARD, block1->directions(Axis::X));
    TEST_ASSERT_EQUAL(Direction::FORWARD, block1->directions(Axis::Y));
    TEST_ASSERT_EQUAL(Direction::FORWARD, block1->directions(Axis::Z));

    PlannerLine line2{{1.f, 2.f, 3.f}, {0.f, 3.f, 4.f}, FEED_RATE_IN_MM_PER_S, tl::nullopt};
    auto block2 = PlannerBlock::fromLine(line2, ENTRY_FEED_RATE_IN_MM_PER_S, EXIT_FEED_RATE_IN_MM_PER_S, ACCELERATION_IN_MM_PER_SS);
    TEST_ASSERT_TRUE(block2.has_value());
    TEST_ASSERT_EQUAL(Direction::BACKWARD, block2->directions(Axis::X));
    TEST_ASSERT_EQUAL(Direction::FORWARD, block2->directions(Axis::Y));
    TEST_ASSERT_EQUAL(Direction::FORWARD, block2->directions(Axis::Z));

    PlannerLine line3{{1.f, 2.f, 3.f}, {2.f, 0.f, 4.f}, FEED_RATE_IN_MM_PER_S, tl::nullopt};
    auto block3 = PlannerBlock::fromLine(line3, ENTRY_FEED_RATE_IN_MM_PER_S, EXIT_FEED_RATE_IN_MM_PER_S, ACCELERATION_IN_MM_PER_SS);
    TEST_ASSERT_TRUE(block3.has_value());
    TEST_ASSERT_EQUAL(Direction::FORWARD, block3->directions(Axis::X));
    TEST_ASSERT_EQUAL(Direction::BACKWARD, block3->directions(Axis::Y));
    TEST_ASSERT_EQUAL(Direction::FORWARD, block3->directions(Axis::Z));

    PlannerLine line4{{1.f, 2.f, 3.f}, {2.f, 3.f, 0.f}, FEED_RATE_IN_MM_PER_S, tl::nullopt};
    auto block4 = PlannerBlock::fromLine(line4, ENTRY_FEED_RATE_IN_MM_PER_S, EXIT_FEED_RATE_IN_MM_PER_S, ACCELERATION_IN_MM_PER_SS);
    TEST_ASSERT_TRUE(block4.has_value());
    TEST_ASSERT_EQUAL(Direction::FORWARD, block4->directions(Axis::X));
    TEST_ASSERT_EQUAL(Direction::FORWARD, block4->directions(Axis::Y));
    TEST_ASSERT_EQUAL(Direction::BACKWARD, block4->directions(Axis::Z));

    PlannerLine line5{{1.f, 2.f, 3.f}, {0.f, 0.f, 0.f}, FEED_RATE_IN_MM_PER_S, tl::nullopt};
    auto block5 = PlannerBlock::fromLine(line5, ENTRY_FEED_RATE_IN_MM_PER_S, EXIT_FEED_RATE_IN_MM_PER_S, ACCELERATION_IN_MM_PER_SS);
    TEST_ASSERT_TRUE(block5.has_value());
    TEST_ASSERT_EQUAL(Direction::BACKWARD, block5->directions(Axis::X));
    TEST_ASSERT_EQUAL(Direction::BACKWARD, block5->directions(Axis::Y));
    TEST_ASSERT_EQUAL(Direction::BACKWARD, block5->directions(Axis::Z));
}

void test_PlannerBlockFromLine_spindleRpm()
{
    constexpr float ENTRY_FEED_RATE_IN_MM_PER_S = 1.f;
    constexpr float FEED_RATE_IN_MM_PER_S = 2.f;
    constexpr float EXIT_FEED_RATE_IN_MM_PER_S = 1.5f;
    constexpr float ACCELERATION_IN_MM_PER_SS = 200.f;

    PlannerLine line1{{0.f, 0.f, 0.f}, {0.f, 0.f, 0.f}, FEED_RATE_IN_MM_PER_S, tl::nullopt};
    auto block1 = PlannerBlock::fromLine(line1, ENTRY_FEED_RATE_IN_MM_PER_S, EXIT_FEED_RATE_IN_MM_PER_S, ACCELERATION_IN_MM_PER_SS);
    TEST_ASSERT_TRUE(block1.has_value());
    TEST_ASSERT(tl::nullopt == block1->spindleRpm());

    PlannerLine line2{{0.f, 0.f, 0.f}, {0.f, 0.f, 0.f}, FEED_RATE_IN_MM_PER_S, 10000.f};
    auto block2 = PlannerBlock::fromLine(line2, ENTRY_FEED_RATE_IN_MM_PER_S, EXIT_FEED_RATE_IN_MM_PER_S, ACCELERATION_IN_MM_PER_SS);
    TEST_ASSERT_TRUE(block2.has_value());
    TEST_ASSERT(10000.f == block2->spindleRpm());
}

void test_PlannerBlockFromLine_noMovement()
{
    constexpr float ENTRY_FEED_RATE_IN_MM_PER_S = 1.f;
    constexpr float FEED_RATE_IN_MM_PER_S = 2.f;
    constexpr float EXIT_FEED_RATE_IN_MM_PER_S = 1.5f;
    constexpr float ACCELERATION_IN_MM_PER_SS = 200.f;

    PlannerLine line{{0.f, 0.f, 0.f}, {0.f, 0.f, 0.f}, FEED_RATE_IN_MM_PER_S, tl::nullopt};
    auto block = PlannerBlock::fromLine(line, ENTRY_FEED_RATE_IN_MM_PER_S, EXIT_FEED_RATE_IN_MM_PER_S, ACCELERATION_IN_MM_PER_SS);
    TEST_ASSERT_TRUE(block.has_value());

    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.f, block->distance());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, block->entryFeedRateInMmPerS());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, block->feedRateInMmPerS());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, block->exitFeedRateInMmPerS());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.f, block->accelerationDurationS());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.f, block->plateauDurationS());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.f, block->decelerationDurationS());
}

void test_PlannerBlockFromLine_accelerationOnly()
{
    constexpr float ENTRY_FEED_RATE_IN_MM_PER_S = 1.f;
    constexpr float FEED_RATE_IN_MM_PER_S = 10.f;
    constexpr float EXIT_FEED_RATE_IN_MM_PER_S = 1.5f;
    constexpr float ACCELERATION_IN_MM_PER_SS = 2.f;

    PlannerLine line{{0.f, 0.f, 0.f}, {10.f, 0.f, 0.f}, FEED_RATE_IN_MM_PER_S, tl::nullopt};
    auto block = PlannerBlock::fromLine(line, ENTRY_FEED_RATE_IN_MM_PER_S, EXIT_FEED_RATE_IN_MM_PER_S, ACCELERATION_IN_MM_PER_SS);
    TEST_ASSERT_TRUE(block.has_value());

    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 10.f, block->distance());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, block->entryFeedRateInMmPerS());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 6.40312f, block->feedRateInMmPerS());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 6.40312f, block->exitFeedRateInMmPerS());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.70156f, block->accelerationDurationS());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.f, block->plateauDurationS());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.f, block->decelerationDurationS());
}

void test_PlannerBlockFromLine_decelerationOnly()
{
    constexpr float ENTRY_FEED_RATE_IN_MM_PER_S = 10.f;
    constexpr float FEED_RATE_IN_MM_PER_S = 20.f;
    constexpr float EXIT_FEED_RATE_IN_MM_PER_S = 1.0f;
    constexpr float ACCELERATION_IN_MM_PER_SS = 2.f;

    PlannerLine line{{-3.f, -4.f, 0.f}, {0.f, 0.f, 0.f}, FEED_RATE_IN_MM_PER_S, tl::nullopt};
    auto block = PlannerBlock::fromLine(line, ENTRY_FEED_RATE_IN_MM_PER_S, EXIT_FEED_RATE_IN_MM_PER_S, ACCELERATION_IN_MM_PER_SS);
    TEST_ASSERT_TRUE(block.has_value());

    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 5.f, block->distance());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 10.f, block->entryFeedRateInMmPerS());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 10.f, block->feedRateInMmPerS());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 8.94427f, block->exitFeedRateInMmPerS());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.f, block->accelerationDurationS());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.f, block->plateauDurationS());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.527864f, block->decelerationDurationS());
}

void test_PlannerBlockFromLine_accelerationPlateau()
{
    constexpr float ENTRY_FEED_RATE_IN_MM_PER_S = 1.f;
    constexpr float FEED_RATE_IN_MM_PER_S = 20.f;
    constexpr float EXIT_FEED_RATE_IN_MM_PER_S = 20.f;
    constexpr float ACCELERATION_IN_MM_PER_SS = 8.f;

    PlannerLine line{{0.f, 40.f, 30.f}, {0.f, 0.f, 0.f}, FEED_RATE_IN_MM_PER_S, tl::nullopt};
    auto block = PlannerBlock::fromLine(line, ENTRY_FEED_RATE_IN_MM_PER_S, EXIT_FEED_RATE_IN_MM_PER_S, ACCELERATION_IN_MM_PER_SS);
    TEST_ASSERT_TRUE(block.has_value());

    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 50.f, block->distance());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, block->entryFeedRateInMmPerS());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 20.f, block->feedRateInMmPerS());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 20.f, block->exitFeedRateInMmPerS());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.375f, block->accelerationDurationS());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.25312f, block->plateauDurationS());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.f, block->decelerationDurationS());
}

void test_PlannerBlockFromLine_plateauDeceleration()
{
    constexpr float ENTRY_FEED_RATE_IN_MM_PER_S = 20.f;
    constexpr float FEED_RATE_IN_MM_PER_S = 20.f;
    constexpr float EXIT_FEED_RATE_IN_MM_PER_S = 1.f;
    constexpr float ACCELERATION_IN_MM_PER_SS = 8.f;

    PlannerLine line{{0.f, 0.f, 0.f}, {30.f, 0.f, 40.f}, FEED_RATE_IN_MM_PER_S, tl::nullopt};
    auto block = PlannerBlock::fromLine(line, ENTRY_FEED_RATE_IN_MM_PER_S, EXIT_FEED_RATE_IN_MM_PER_S, ACCELERATION_IN_MM_PER_SS);
    TEST_ASSERT_TRUE(block.has_value());

    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 50.f, block->distance());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 20.f, block->entryFeedRateInMmPerS());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 20.f, block->feedRateInMmPerS());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, block->exitFeedRateInMmPerS());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.f, block->accelerationDurationS());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.25312f, block->plateauDurationS());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.375f, block->decelerationDurationS());
}

void test_PlannerBlockFromLine_plateau()
{
    constexpr float ENTRY_FEED_RATE_IN_MM_PER_S = 10.f;
    constexpr float FEED_RATE_IN_MM_PER_S = 10.f;
    constexpr float EXIT_FEED_RATE_IN_MM_PER_S = 10.f;
    constexpr float ACCELERATION_IN_MM_PER_SS = 100.f;

    PlannerLine line{{0.f, 0.f, 0.f}, {100.f, 0.f, 0.f}, FEED_RATE_IN_MM_PER_S, tl::nullopt};
    auto block = PlannerBlock::fromLine(line, ENTRY_FEED_RATE_IN_MM_PER_S, EXIT_FEED_RATE_IN_MM_PER_S, ACCELERATION_IN_MM_PER_SS);
    TEST_ASSERT_TRUE(block.has_value());

    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 100.f, block->distance());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 10.f, block->entryFeedRateInMmPerS());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 10.f, block->feedRateInMmPerS());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 10.f, block->exitFeedRateInMmPerS());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.f, block->accelerationDurationS());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 10.f, block->plateauDurationS());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.f, block->decelerationDurationS());
}

void test_PlannerBlockFromLine_accelerationPlateauDeceleration()
{
    constexpr float ENTRY_FEED_RATE_IN_MM_PER_S = 5.f;
    constexpr float FEED_RATE_IN_MM_PER_S = 10.f;
    constexpr float EXIT_FEED_RATE_IN_MM_PER_S = 2.f;
    constexpr float ACCELERATION_IN_MM_PER_SS = 3.f;

    PlannerLine line{{0.f, 0.f, 0.f}, {100.f, 0.f, 0.f}, FEED_RATE_IN_MM_PER_S, tl::nullopt};
    auto block = PlannerBlock::fromLine(line, ENTRY_FEED_RATE_IN_MM_PER_S, EXIT_FEED_RATE_IN_MM_PER_S, ACCELERATION_IN_MM_PER_SS);
    TEST_ASSERT_TRUE(block.has_value());

    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 100.f, block->distance());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 5.f, block->entryFeedRateInMmPerS());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 10.f, block->feedRateInMmPerS());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, block->exitFeedRateInMmPerS());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.66667f, block->accelerationDurationS());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 7.15f, block->plateauDurationS());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.66667f, block->decelerationDurationS());
}

void test_PlannerBlockFromLine_accelerationDeceleration()
{
    constexpr float ENTRY_FEED_RATE_IN_MM_PER_S = 1.f;
    constexpr float FEED_RATE_IN_MM_PER_S = 10.f;
    constexpr float EXIT_FEED_RATE_IN_MM_PER_S = 6.f;
    constexpr float ACCELERATION_IN_MM_PER_SS = 3.f;

    PlannerLine line{{0.f, 0.f, 0.f}, {20.f, 0.f, 0.f}, FEED_RATE_IN_MM_PER_S, tl::nullopt};
    auto block = PlannerBlock::fromLine(line, ENTRY_FEED_RATE_IN_MM_PER_S, EXIT_FEED_RATE_IN_MM_PER_S, ACCELERATION_IN_MM_PER_SS);
    TEST_ASSERT_TRUE(block.has_value());

    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 20.f, block->distance());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, block->entryFeedRateInMmPerS());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 8.86002f, block->feedRateInMmPerS());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 6.f, block->exitFeedRateInMmPerS());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.62001f, block->accelerationDurationS());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.f, block->plateauDurationS());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.953341f, block->decelerationDurationS());
}

void test_PlannerBlockToLinearBlock_directions()
{
    constexpr float ENTRY_FEED_RATE_IN_MM_PER_S = 1.f;
    constexpr float FEED_RATE_IN_MM_PER_S = 2.f;
    constexpr float EXIT_FEED_RATE_IN_MM_PER_S = 1.5f;
    constexpr float ACCELERATION_IN_MM_PER_SS = 200.f;

    PlannerLine line1{{0.f, 0.f, 0.f}, {0.f, 0.f, 0.f}, FEED_RATE_IN_MM_PER_S, tl::nullopt};
    auto plannerBlock1 = PlannerBlock::fromLine(line1, ENTRY_FEED_RATE_IN_MM_PER_S, EXIT_FEED_RATE_IN_MM_PER_S, ACCELERATION_IN_MM_PER_SS);
    TEST_ASSERT_TRUE(plannerBlock1.has_value());

    auto linearBlock1 = plannerBlock1->toLinearBlock(1.f, 1.f, 1.f, 1.f, 1.f);
    TEST_ASSERT_EQUAL(Direction::FORWARD, linearBlock1.directions[AXIS_X_INDEX]);
    TEST_ASSERT_EQUAL(Direction::FORWARD, linearBlock1.directions[AXIS_Y_INDEX]);
    TEST_ASSERT_EQUAL(Direction::FORWARD, linearBlock1.directions[AXIS_Z_INDEX]);


    PlannerLine line2{{1.f, 2.f, 3.f}, {0.f, 3.f, 4.f}, FEED_RATE_IN_MM_PER_S, tl::nullopt};
    auto plannerBlock2 = PlannerBlock::fromLine(line2, ENTRY_FEED_RATE_IN_MM_PER_S, EXIT_FEED_RATE_IN_MM_PER_S, ACCELERATION_IN_MM_PER_SS);
    TEST_ASSERT_TRUE(plannerBlock2.has_value());

    auto linearBlock2 = plannerBlock2->toLinearBlock(1.f, 1.f, 1.f, 1.f, 1.f);
    TEST_ASSERT_EQUAL(Direction::BACKWARD, linearBlock2.directions[AXIS_X_INDEX]);
    TEST_ASSERT_EQUAL(Direction::FORWARD, linearBlock2.directions[AXIS_Y_INDEX]);
    TEST_ASSERT_EQUAL(Direction::FORWARD, linearBlock2.directions[AXIS_Z_INDEX]);


    PlannerLine line3{{1.f, 2.f, 3.f}, {2.f, 0.f, 4.f}, FEED_RATE_IN_MM_PER_S, tl::nullopt};
    auto plannerBlock3 = PlannerBlock::fromLine(line3, ENTRY_FEED_RATE_IN_MM_PER_S, EXIT_FEED_RATE_IN_MM_PER_S, ACCELERATION_IN_MM_PER_SS);
    TEST_ASSERT_TRUE(plannerBlock3.has_value());

    auto linearBlock3 = plannerBlock3->toLinearBlock(1.f, 1.f, 1.f, 1.f, 1.f);
    TEST_ASSERT_EQUAL(Direction::FORWARD, linearBlock3.directions[AXIS_X_INDEX]);
    TEST_ASSERT_EQUAL(Direction::BACKWARD, linearBlock3.directions[AXIS_Y_INDEX]);
    TEST_ASSERT_EQUAL(Direction::FORWARD, linearBlock3.directions[AXIS_Z_INDEX]);


    PlannerLine line4{{1.f, 2.f, 3.f}, {2.f, 3.f, 0.f}, FEED_RATE_IN_MM_PER_S, tl::nullopt};
    auto plannerBlock4 = PlannerBlock::fromLine(line4, ENTRY_FEED_RATE_IN_MM_PER_S, EXIT_FEED_RATE_IN_MM_PER_S, ACCELERATION_IN_MM_PER_SS);
    TEST_ASSERT_TRUE(plannerBlock4.has_value());

    auto linearBlock4 = plannerBlock4->toLinearBlock(1.f, 1.f, 1.f, 1.f, 1.f);
    TEST_ASSERT_EQUAL(Direction::FORWARD, linearBlock4.directions[AXIS_X_INDEX]);
    TEST_ASSERT_EQUAL(Direction::FORWARD, linearBlock4.directions[AXIS_Y_INDEX]);
    TEST_ASSERT_EQUAL(Direction::BACKWARD, linearBlock4.directions[AXIS_Z_INDEX]);


    PlannerLine line5{{1.f, 2.f, 3.f}, {0.f, 0.f, 0.f}, FEED_RATE_IN_MM_PER_S, tl::nullopt};
    auto plannerBlock5 = PlannerBlock::fromLine(line5, ENTRY_FEED_RATE_IN_MM_PER_S, EXIT_FEED_RATE_IN_MM_PER_S, ACCELERATION_IN_MM_PER_SS);
    TEST_ASSERT_TRUE(plannerBlock5.has_value());

    auto linearBlock5 = plannerBlock5->toLinearBlock(1.f, 1.f, 1.f, 1.f, 1.f);
    TEST_ASSERT_EQUAL(Direction::BACKWARD, linearBlock5.directions[AXIS_X_INDEX]);
    TEST_ASSERT_EQUAL(Direction::BACKWARD, linearBlock5.directions[AXIS_Y_INDEX]);
    TEST_ASSERT_EQUAL(Direction::BACKWARD, linearBlock5.directions[AXIS_Z_INDEX]);
}

void test_PlannerBlockToLinearBlock_spindleRpm()
{
    constexpr float ENTRY_FEED_RATE_IN_MM_PER_S = 1.f;
    constexpr float FEED_RATE_IN_MM_PER_S = 2.f;
    constexpr float EXIT_FEED_RATE_IN_MM_PER_S = 1.5f;
    constexpr float ACCELERATION_IN_MM_PER_SS = 200.f;

    PlannerLine line1{{0.f, 0.f, 0.f}, {0.f, 0.f, 0.f}, FEED_RATE_IN_MM_PER_S, tl::nullopt};
    auto plannerBlock1 = PlannerBlock::fromLine(line1, ENTRY_FEED_RATE_IN_MM_PER_S, EXIT_FEED_RATE_IN_MM_PER_S, ACCELERATION_IN_MM_PER_SS);
    TEST_ASSERT_TRUE(plannerBlock1.has_value());
    auto linearBlock1 = plannerBlock1->toLinearBlock(1.f, 1.f, 1.f, 1.f, 1.f);
    TEST_ASSERT(tl::nullopt == linearBlock1.spindleRpm);

    PlannerLine line2{{0.f, 0.f, 0.f}, {0.f, 0.f, 0.f}, FEED_RATE_IN_MM_PER_S, 10000.f};
    auto plannerBlock2 = PlannerBlock::fromLine(line2, ENTRY_FEED_RATE_IN_MM_PER_S, EXIT_FEED_RATE_IN_MM_PER_S, ACCELERATION_IN_MM_PER_SS);
    TEST_ASSERT_TRUE(plannerBlock2.has_value());
    auto linearBlock2 = plannerBlock2->toLinearBlock(1.f, 1.f, 1.f, 1.f, 1.f);
    TEST_ASSERT(10000.f == linearBlock2.spindleRpm);
}

void test_PlannerBlockToLinearBlock_noMovement()
{
    constexpr float ENTRY_FEED_RATE_IN_MM_PER_S = 1.f;
    constexpr float FEED_RATE_IN_MM_PER_S = 2.f;
    constexpr float EXIT_FEED_RATE_IN_MM_PER_S = 1.5f;
    constexpr float ACCELERATION_IN_MM_PER_SS = 200.f;

    constexpr float X_STEP_COUNT_PER_MM = 10;
    constexpr float Y_STEP_COUNT_PER_MM = 20;
    constexpr float Z_STEP_COUNT_PER_MM = 30;
    constexpr float MIN_FEED_RATE_IN_MM_PER_S = 0.1;
    constexpr float TICK_FREQUENCY = 100000;

    PlannerLine line{{0.f, 0.f, 0.f}, {0.f, 0.f, 0.f}, FEED_RATE_IN_MM_PER_S, tl::nullopt};
    auto plannerBlock = PlannerBlock::fromLine(line, ENTRY_FEED_RATE_IN_MM_PER_S, EXIT_FEED_RATE_IN_MM_PER_S, ACCELERATION_IN_MM_PER_SS);
    TEST_ASSERT_TRUE(plannerBlock.has_value());

    auto linearBlock = plannerBlock->toLinearBlock(X_STEP_COUNT_PER_MM, Y_STEP_COUNT_PER_MM, Z_STEP_COUNT_PER_MM, MIN_FEED_RATE_IN_MM_PER_S, TICK_FREQUENCY);

    TEST_ASSERT_EQUAL(0, linearBlock.currentTick);
    TEST_ASSERT_EQUAL(0, linearBlock.accelerationUntilTick);
    TEST_ASSERT_EQUAL(0, linearBlock.plateauUntilTick);
    TEST_ASSERT_EQUAL(0, linearBlock.decelerationUntilTick);
    TEST_ASSERT_EQUAL(0, linearBlock.currentStepCount[AXIS_X_INDEX]);
    TEST_ASSERT_EQUAL(0, linearBlock.currentStepCount[AXIS_Y_INDEX]);
    TEST_ASSERT_EQUAL(0, linearBlock.currentStepCount[AXIS_Z_INDEX]);
    TEST_ASSERT_EQUAL(0, linearBlock.totalStepCount[AXIS_X_INDEX]);
    TEST_ASSERT_EQUAL(0, linearBlock.totalStepCount[AXIS_Y_INDEX]);
    TEST_ASSERT_EQUAL(0, linearBlock.totalStepCount[AXIS_Z_INDEX]);
    TEST_ASSERT_EQUAL(0, linearBlock.durationUs);
}

void test_PlannerBlockToLinearBlock_accelerationOnly()
{
    constexpr float ENTRY_FEED_RATE_IN_MM_PER_S = 1.f;
    constexpr float FEED_RATE_IN_MM_PER_S = 10.f;
    constexpr float EXIT_FEED_RATE_IN_MM_PER_S = 1.5f;
    constexpr float ACCELERATION_IN_MM_PER_SS = 2.f;

    constexpr float X_STEP_COUNT_PER_MM = 10;
    constexpr float Y_STEP_COUNT_PER_MM = 20;
    constexpr float Z_STEP_COUNT_PER_MM = 30;
    constexpr float MIN_FEED_RATE_IN_MM_PER_S = 0.1;
    constexpr float TICK_FREQUENCY = 100000;

    PlannerLine line{{0.f, 0.f, 0.f}, {-10.f, 0.f, 0.f}, FEED_RATE_IN_MM_PER_S, tl::nullopt};
    auto plannerBlock = PlannerBlock::fromLine(line, ENTRY_FEED_RATE_IN_MM_PER_S, EXIT_FEED_RATE_IN_MM_PER_S, ACCELERATION_IN_MM_PER_SS);
    TEST_ASSERT_TRUE(plannerBlock.has_value());

    auto linearBlock = plannerBlock->toLinearBlock(X_STEP_COUNT_PER_MM, Y_STEP_COUNT_PER_MM, Z_STEP_COUNT_PER_MM, MIN_FEED_RATE_IN_MM_PER_S, TICK_FREQUENCY);
    TEST_ASSERT_EQUAL(0, linearBlock.currentTick);
    TEST_ASSERT_EQUAL(270156, linearBlock.accelerationUntilTick);
    TEST_ASSERT_EQUAL(270156, linearBlock.plateauUntilTick);
    TEST_ASSERT_EQUAL(270156, linearBlock.decelerationUntilTick);
    TEST_ASSERT_EQUAL(0, linearBlock.currentStepCount[AXIS_X_INDEX]);
    TEST_ASSERT_EQUAL(0, linearBlock.currentStepCount[AXIS_Y_INDEX]);
    TEST_ASSERT_EQUAL(0, linearBlock.currentStepCount[AXIS_Z_INDEX]);
    TEST_ASSERT_EQUAL(100, linearBlock.totalStepCount[AXIS_X_INDEX]);
    TEST_ASSERT_EQUAL(0, linearBlock.totalStepCount[AXIS_Y_INDEX]);
    TEST_ASSERT_EQUAL(0, linearBlock.totalStepCount[AXIS_Z_INDEX]);
    TEST_ASSERT(LinearBlockFixedPoint(922337943250674) == linearBlock.accelerationPerTick[AXIS_X_INDEX]);
    TEST_ASSERT(0.f == linearBlock.accelerationPerTick[AXIS_Y_INDEX]);
    TEST_ASSERT(0.f == linearBlock.accelerationPerTick[AXIS_Z_INDEX]);
    TEST_ASSERT(0.f == linearBlock.decelerationPerTick[AXIS_X_INDEX]);
    TEST_ASSERT(0.f == linearBlock.decelerationPerTick[AXIS_Y_INDEX]);
    TEST_ASSERT(0.f == linearBlock.decelerationPerTick[AXIS_Z_INDEX]);
    TEST_ASSERT(LinearBlockFixedPoint(461168601842738) == linearBlock.stepPerTick[AXIS_X_INDEX]);
    TEST_ASSERT(0.f == linearBlock.stepPerTick[AXIS_Y_INDEX]);
    TEST_ASSERT(0.f == linearBlock.stepPerTick[AXIS_Z_INDEX]);
    TEST_ASSERT(0.f == linearBlock.stepCounter[AXIS_X_INDEX]);
    TEST_ASSERT(0.f == linearBlock.stepCounter[AXIS_Y_INDEX]);
    TEST_ASSERT(0.f == linearBlock.stepCounter[AXIS_Z_INDEX]);
    TEST_ASSERT(LinearBlockFixedPoint(46116860871468) == linearBlock.minStepPerTick[AXIS_X_INDEX]);
    TEST_ASSERT(0.d == linearBlock.minStepPerTick[AXIS_Y_INDEX]);
    TEST_ASSERT(0.d == linearBlock.minStepPerTick[AXIS_Z_INDEX]);
    TEST_ASSERT_EQUAL(2701560, linearBlock.durationUs);
    TEST_ASSERT(tl::nullopt == linearBlock.spindleRpm);
}

void test_PlannerBlockToLinearBlock_decelerationOnly()
{
    constexpr float ENTRY_FEED_RATE_IN_MM_PER_S = 10.f;
    constexpr float FEED_RATE_IN_MM_PER_S = 20.f;
    constexpr float EXIT_FEED_RATE_IN_MM_PER_S = 1.0f;
    constexpr float ACCELERATION_IN_MM_PER_SS = 2.f;

    constexpr float X_STEP_COUNT_PER_MM = 10;
    constexpr float Y_STEP_COUNT_PER_MM = 20;
    constexpr float Z_STEP_COUNT_PER_MM = 30;
    constexpr float MIN_FEED_RATE_IN_MM_PER_S = 0.1;
    constexpr float TICK_FREQUENCY = 100000;

    PlannerLine line{{3.f, 4.f, 0.f}, {0.f, 0.f, 0.f}, FEED_RATE_IN_MM_PER_S, tl::nullopt};
    auto plannerBlock = PlannerBlock::fromLine(line, ENTRY_FEED_RATE_IN_MM_PER_S, EXIT_FEED_RATE_IN_MM_PER_S, ACCELERATION_IN_MM_PER_SS);
    TEST_ASSERT_TRUE(plannerBlock.has_value());

    auto linearBlock = plannerBlock->toLinearBlock(X_STEP_COUNT_PER_MM, Y_STEP_COUNT_PER_MM, Z_STEP_COUNT_PER_MM, MIN_FEED_RATE_IN_MM_PER_S, TICK_FREQUENCY);
    TEST_ASSERT_EQUAL(0, linearBlock.currentTick);
    TEST_ASSERT_EQUAL(0, linearBlock.accelerationUntilTick);
    TEST_ASSERT_EQUAL(0, linearBlock.plateauUntilTick);
    TEST_ASSERT_EQUAL(52786, linearBlock.decelerationUntilTick);
    TEST_ASSERT_EQUAL(0, linearBlock.currentStepCount[AXIS_X_INDEX]);
    TEST_ASSERT_EQUAL(0, linearBlock.currentStepCount[AXIS_Y_INDEX]);
    TEST_ASSERT_EQUAL(0, linearBlock.currentStepCount[AXIS_Z_INDEX]);
    TEST_ASSERT_EQUAL(30, linearBlock.totalStepCount[AXIS_X_INDEX]);
    TEST_ASSERT_EQUAL(80, linearBlock.totalStepCount[AXIS_Y_INDEX]);
    TEST_ASSERT_EQUAL(0, linearBlock.totalStepCount[AXIS_Z_INDEX]);
    TEST_ASSERT(0.f == linearBlock.accelerationPerTick[AXIS_X_INDEX]);
    TEST_ASSERT(0.f == linearBlock.accelerationPerTick[AXIS_Y_INDEX]);
    TEST_ASSERT(0.f == linearBlock.accelerationPerTick[AXIS_Z_INDEX]);
    TEST_ASSERT(LinearBlockFixedPoint(-553406494104923) == linearBlock.decelerationPerTick[AXIS_X_INDEX]);
    TEST_ASSERT(LinearBlockFixedPoint(-1475750650946463) == linearBlock.decelerationPerTick[AXIS_Y_INDEX]);
    TEST_ASSERT(0.f == linearBlock.decelerationPerTick[AXIS_Z_INDEX]);
    TEST_ASSERT(LinearBlockFixedPoint(2767011611056433) == linearBlock.stepPerTick[AXIS_X_INDEX]);
    TEST_ASSERT(LinearBlockFixedPoint(7378697629483821) == linearBlock.stepPerTick[AXIS_Y_INDEX]);
    TEST_ASSERT(0.f == linearBlock.stepPerTick[AXIS_Z_INDEX]);
    TEST_ASSERT(0.f == linearBlock.stepCounter[AXIS_X_INDEX]);
    TEST_ASSERT(0.f == linearBlock.stepCounter[AXIS_Y_INDEX]);
    TEST_ASSERT(0.f == linearBlock.stepCounter[AXIS_Z_INDEX]);
    TEST_ASSERT(LinearBlockFixedPoint(27670116522881) == linearBlock.minStepPerTick[AXIS_X_INDEX]);
    TEST_ASSERT(LinearBlockFixedPoint(73786977394349) == linearBlock.minStepPerTick[AXIS_Y_INDEX]);
    TEST_ASSERT(0.f == linearBlock.minStepPerTick[AXIS_Z_INDEX]);
    TEST_ASSERT_EQUAL(527860, linearBlock.durationUs);
    TEST_ASSERT(tl::nullopt == linearBlock.spindleRpm);
}

void test_PlannerBlockToLinearBlock_accelerationPlateau()
{
    constexpr float ENTRY_FEED_RATE_IN_MM_PER_S = 1.f;
    constexpr float FEED_RATE_IN_MM_PER_S = 20.f;
    constexpr float EXIT_FEED_RATE_IN_MM_PER_S = 20.f;
    constexpr float ACCELERATION_IN_MM_PER_SS = 8.f;

    constexpr float X_STEP_COUNT_PER_MM = 10;
    constexpr float Y_STEP_COUNT_PER_MM = 20;
    constexpr float Z_STEP_COUNT_PER_MM = 30;
    constexpr float MIN_FEED_RATE_IN_MM_PER_S = 0.1;
    constexpr float TICK_FREQUENCY = 100000.f;

    PlannerLine line{{0.f, 40.f, -30.f}, {0.f, 0.f, 0.f}, FEED_RATE_IN_MM_PER_S, tl::nullopt};
    auto plannerBlock = PlannerBlock::fromLine(line, ENTRY_FEED_RATE_IN_MM_PER_S, EXIT_FEED_RATE_IN_MM_PER_S, ACCELERATION_IN_MM_PER_SS);
    TEST_ASSERT_TRUE(plannerBlock.has_value());

    auto linearBlock = plannerBlock->toLinearBlock(X_STEP_COUNT_PER_MM, Y_STEP_COUNT_PER_MM, Z_STEP_COUNT_PER_MM, MIN_FEED_RATE_IN_MM_PER_S, TICK_FREQUENCY);
    TEST_ASSERT_EQUAL(0, linearBlock.currentTick);
    TEST_ASSERT_EQUAL(237500, linearBlock.accelerationUntilTick);
    TEST_ASSERT_EQUAL(362812, linearBlock.plateauUntilTick);
    TEST_ASSERT_EQUAL(362812, linearBlock.decelerationUntilTick);
    TEST_ASSERT_EQUAL(0, linearBlock.currentStepCount[AXIS_X_INDEX]);
    TEST_ASSERT_EQUAL(0, linearBlock.currentStepCount[AXIS_Y_INDEX]);
    TEST_ASSERT_EQUAL(0, linearBlock.currentStepCount[AXIS_Z_INDEX]);
    TEST_ASSERT_EQUAL(0, linearBlock.totalStepCount[AXIS_X_INDEX]);
    TEST_ASSERT_EQUAL(800, linearBlock.totalStepCount[AXIS_Y_INDEX]);
    TEST_ASSERT_EQUAL(900, linearBlock.totalStepCount[AXIS_Z_INDEX]);
    TEST_ASSERT(0.f == linearBlock.accelerationPerTick[AXIS_X_INDEX]);
    TEST_ASSERT(LinearBlockFixedPoint(5902958103587057) == linearBlock.accelerationPerTick[AXIS_Y_INDEX]);
    TEST_ASSERT(LinearBlockFixedPoint(6640827866535439) == linearBlock.accelerationPerTick[AXIS_Z_INDEX]);
    TEST_ASSERT(0.f == linearBlock.decelerationPerTick[AXIS_X_INDEX]);
    TEST_ASSERT(0.f == linearBlock.decelerationPerTick[AXIS_Y_INDEX]);
    TEST_ASSERT(0.f == linearBlock.decelerationPerTick[AXIS_Z_INDEX]);
    TEST_ASSERT(0.f == linearBlock.stepPerTick[AXIS_X_INDEX]);
    TEST_ASSERT(LinearBlockFixedPoint(737869762948382) == linearBlock.stepPerTick[AXIS_Y_INDEX]);
    TEST_ASSERT(LinearBlockFixedPoint(830103483316929) == linearBlock.stepPerTick[AXIS_Z_INDEX]);
    TEST_ASSERT(0.f == linearBlock.stepCounter[AXIS_X_INDEX]);
    TEST_ASSERT(0.f == linearBlock.stepCounter[AXIS_Y_INDEX]);
    TEST_ASSERT(0.f == linearBlock.stepCounter[AXIS_Z_INDEX]);
    TEST_ASSERT(0.f == linearBlock.minStepPerTick[AXIS_X_INDEX]);
    TEST_ASSERT(LinearBlockFixedPoint(73786977394349) == linearBlock.minStepPerTick[AXIS_Y_INDEX]);
    TEST_ASSERT(LinearBlockFixedPoint(83010349568643) == linearBlock.minStepPerTick[AXIS_Z_INDEX]);
    TEST_ASSERT_EQUAL(3628120, linearBlock.durationUs);
    TEST_ASSERT(tl::nullopt == linearBlock.spindleRpm);
}

void test_PlannerBlockToLinearBlock_plateauDeceleration()
{
    constexpr float ENTRY_FEED_RATE_IN_MM_PER_S = 20.f;
    constexpr float FEED_RATE_IN_MM_PER_S = 20.f;
    constexpr float EXIT_FEED_RATE_IN_MM_PER_S = 1.f;
    constexpr float ACCELERATION_IN_MM_PER_SS = 8.f;

    constexpr float X_STEP_COUNT_PER_MM = 100;
    constexpr float Y_STEP_COUNT_PER_MM = 200;
    constexpr float Z_STEP_COUNT_PER_MM = 300;
    constexpr float MIN_FEED_RATE_IN_MM_PER_S = 0.1;
    constexpr float TICK_FREQUENCY = 100000.f;

    PlannerLine line{{0.f, 0.f, 0.f}, {30.f, 0.f, 40.f}, FEED_RATE_IN_MM_PER_S, tl::nullopt};
    auto plannerBlock = PlannerBlock::fromLine(line, ENTRY_FEED_RATE_IN_MM_PER_S, EXIT_FEED_RATE_IN_MM_PER_S, ACCELERATION_IN_MM_PER_SS);
    TEST_ASSERT_TRUE(plannerBlock.has_value());

    auto linearBlock = plannerBlock->toLinearBlock(X_STEP_COUNT_PER_MM, Y_STEP_COUNT_PER_MM, Z_STEP_COUNT_PER_MM, MIN_FEED_RATE_IN_MM_PER_S, TICK_FREQUENCY);
    TEST_ASSERT_EQUAL(0, linearBlock.currentTick);
    TEST_ASSERT_EQUAL(0, linearBlock.accelerationUntilTick);
    TEST_ASSERT_EQUAL(125312, linearBlock.plateauUntilTick);
    TEST_ASSERT_EQUAL(362812, linearBlock.decelerationUntilTick);
    TEST_ASSERT_EQUAL(0, linearBlock.currentStepCount[AXIS_X_INDEX]);
    TEST_ASSERT_EQUAL(0, linearBlock.currentStepCount[AXIS_Y_INDEX]);
    TEST_ASSERT_EQUAL(0, linearBlock.currentStepCount[AXIS_Z_INDEX]);
    TEST_ASSERT_EQUAL(3000, linearBlock.totalStepCount[AXIS_X_INDEX]);
    TEST_ASSERT_EQUAL(0, linearBlock.totalStepCount[AXIS_Y_INDEX]);
    TEST_ASSERT_EQUAL(12000, linearBlock.totalStepCount[AXIS_Z_INDEX]);
    TEST_ASSERT(0.f == linearBlock.accelerationPerTick[AXIS_X_INDEX]);
    TEST_ASSERT(0.f == linearBlock.accelerationPerTick[AXIS_Y_INDEX]);
    TEST_ASSERT(0.f == linearBlock.accelerationPerTick[AXIS_Z_INDEX]);
    TEST_ASSERT(LinearBlockFixedPoint(-22136092888451460) == linearBlock.decelerationPerTick[AXIS_X_INDEX]);
    TEST_ASSERT(0.f == linearBlock.decelerationPerTick[AXIS_Y_INDEX]);
    TEST_ASSERT(LinearBlockFixedPoint(-88544371553805840) == linearBlock.decelerationPerTick[AXIS_Z_INDEX]);
    TEST_ASSERT(LinearBlockFixedPoint(55340232221128648) == linearBlock.stepPerTick[AXIS_X_INDEX]);
    TEST_ASSERT(0.f == linearBlock.stepPerTick[AXIS_Y_INDEX]);
    TEST_ASSERT(LinearBlockFixedPoint(221360928884514592) == linearBlock.stepPerTick[AXIS_Z_INDEX]);
    TEST_ASSERT(0.f == linearBlock.stepCounter[AXIS_X_INDEX]);
    TEST_ASSERT(0.f == linearBlock.stepCounter[AXIS_Y_INDEX]);
    TEST_ASSERT(0.f == linearBlock.stepCounter[AXIS_Z_INDEX]);
    TEST_ASSERT(LinearBlockFixedPoint(276701165228811) == linearBlock.minStepPerTick[AXIS_X_INDEX]);
    TEST_ASSERT(0.f == linearBlock.minStepPerTick[AXIS_Y_INDEX]);
    TEST_ASSERT(LinearBlockFixedPoint(1106804660915247) == linearBlock.minStepPerTick[AXIS_Z_INDEX]);
    TEST_ASSERT_EQUAL(3628120, linearBlock.durationUs);
    TEST_ASSERT(tl::nullopt == linearBlock.spindleRpm);
}

void test_PlannerBlockToLinearBlock_plateau()
{
    constexpr float ENTRY_FEED_RATE_IN_MM_PER_S = 10.f;
    constexpr float FEED_RATE_IN_MM_PER_S = 10.f;
    constexpr float EXIT_FEED_RATE_IN_MM_PER_S = 10.f;
    constexpr float ACCELERATION_IN_MM_PER_SS = 100.f;

    constexpr float X_STEP_COUNT_PER_MM = 500;
    constexpr float Y_STEP_COUNT_PER_MM = 100;
    constexpr float Z_STEP_COUNT_PER_MM = 500;
    constexpr float MIN_FEED_RATE_IN_MM_PER_S = 0.52;
    constexpr float TICK_FREQUENCY = 100000.f;

    PlannerLine line{{0.f, 0.f, 0.f}, {100.f, 0.f, 0.f}, FEED_RATE_IN_MM_PER_S, tl::nullopt};
    auto plannerBlock = PlannerBlock::fromLine(line, ENTRY_FEED_RATE_IN_MM_PER_S, EXIT_FEED_RATE_IN_MM_PER_S, ACCELERATION_IN_MM_PER_SS);
    TEST_ASSERT_TRUE(plannerBlock.has_value());

    auto linearBlock = plannerBlock->toLinearBlock(X_STEP_COUNT_PER_MM, Y_STEP_COUNT_PER_MM, Z_STEP_COUNT_PER_MM, MIN_FEED_RATE_IN_MM_PER_S, TICK_FREQUENCY);
    TEST_ASSERT_EQUAL(0, linearBlock.currentTick);
    TEST_ASSERT_EQUAL(0, linearBlock.accelerationUntilTick);
    TEST_ASSERT_EQUAL(1000000, linearBlock.plateauUntilTick);
    TEST_ASSERT_EQUAL(1000000, linearBlock.decelerationUntilTick);
    TEST_ASSERT_EQUAL(0, linearBlock.currentStepCount[AXIS_X_INDEX]);
    TEST_ASSERT_EQUAL(0, linearBlock.currentStepCount[AXIS_Y_INDEX]);
    TEST_ASSERT_EQUAL(0, linearBlock.currentStepCount[AXIS_Z_INDEX]);
    TEST_ASSERT_EQUAL(50000, linearBlock.totalStepCount[AXIS_X_INDEX]);
    TEST_ASSERT_EQUAL(0, linearBlock.totalStepCount[AXIS_Y_INDEX]);
    TEST_ASSERT_EQUAL(0, linearBlock.totalStepCount[AXIS_Z_INDEX]);
    TEST_ASSERT(0.f == linearBlock.accelerationPerTick[AXIS_X_INDEX]);
    TEST_ASSERT(0.f == linearBlock.accelerationPerTick[AXIS_Y_INDEX]);
    TEST_ASSERT(0.f == linearBlock.accelerationPerTick[AXIS_Z_INDEX]);
    TEST_ASSERT(0.f == linearBlock.decelerationPerTick[AXIS_X_INDEX]);
    TEST_ASSERT(0.f == linearBlock.decelerationPerTick[AXIS_Y_INDEX]);
    TEST_ASSERT(0.f == linearBlock.decelerationPerTick[AXIS_Z_INDEX]);
    TEST_ASSERT(LinearBlockFixedPoint(230584300921369408) == linearBlock.stepPerTick[AXIS_X_INDEX]);
    TEST_ASSERT(0.f == linearBlock.stepPerTick[AXIS_Y_INDEX]);
    TEST_ASSERT(0.f == linearBlock.stepPerTick[AXIS_Z_INDEX]);
    TEST_ASSERT(0.f == linearBlock.stepCounter[AXIS_X_INDEX]);
    TEST_ASSERT(0.f == linearBlock.stepCounter[AXIS_Y_INDEX]);
    TEST_ASSERT(0.f == linearBlock.stepCounter[AXIS_Z_INDEX]);
    TEST_ASSERT(LinearBlockFixedPoint(11990383208106558) == linearBlock.minStepPerTick[AXIS_X_INDEX]);
    TEST_ASSERT(0.f == linearBlock.minStepPerTick[AXIS_Y_INDEX]);
    TEST_ASSERT(0.f == linearBlock.minStepPerTick[AXIS_Z_INDEX]);
    TEST_ASSERT_EQUAL(10000000, linearBlock.durationUs);
    TEST_ASSERT(tl::nullopt == linearBlock.spindleRpm);
}

void test_PlannerBlockToLinearBlock_accelerationPlateauDeceleration()
{
    constexpr float ENTRY_FEED_RATE_IN_MM_PER_S = 5.f;
    constexpr float FEED_RATE_IN_MM_PER_S = 10.f;
    constexpr float EXIT_FEED_RATE_IN_MM_PER_S = 2.f;
    constexpr float ACCELERATION_IN_MM_PER_SS = 3.f;

    constexpr float X_STEP_COUNT_PER_MM = 500;
    constexpr float Y_STEP_COUNT_PER_MM = 100;
    constexpr float Z_STEP_COUNT_PER_MM = 500;
    constexpr float MIN_FEED_RATE_IN_MM_PER_S = 0.1;
    constexpr float TICK_FREQUENCY = 100000.f;

    PlannerLine line{{0.f, 0.f, 0.f}, {100.f, 0.f, 0.f}, FEED_RATE_IN_MM_PER_S, tl::nullopt};
    auto plannerBlock = PlannerBlock::fromLine(line, ENTRY_FEED_RATE_IN_MM_PER_S, EXIT_FEED_RATE_IN_MM_PER_S, ACCELERATION_IN_MM_PER_SS);
    TEST_ASSERT_TRUE(plannerBlock.has_value());

    auto linearBlock = plannerBlock->toLinearBlock(X_STEP_COUNT_PER_MM, Y_STEP_COUNT_PER_MM, Z_STEP_COUNT_PER_MM, MIN_FEED_RATE_IN_MM_PER_S, TICK_FREQUENCY);
    TEST_ASSERT_EQUAL(0, linearBlock.currentTick);
    TEST_ASSERT_EQUAL(166667, linearBlock.accelerationUntilTick);
    TEST_ASSERT_EQUAL(881667, linearBlock.plateauUntilTick);
    TEST_ASSERT_EQUAL(1148333, linearBlock.decelerationUntilTick);
    TEST_ASSERT_EQUAL(0, linearBlock.currentStepCount[AXIS_X_INDEX]);
    TEST_ASSERT_EQUAL(0, linearBlock.currentStepCount[AXIS_Y_INDEX]);
    TEST_ASSERT_EQUAL(0, linearBlock.currentStepCount[AXIS_Z_INDEX]);
    TEST_ASSERT_EQUAL(50000, linearBlock.totalStepCount[AXIS_X_INDEX]);
    TEST_ASSERT_EQUAL(0, linearBlock.totalStepCount[AXIS_Y_INDEX]);
    TEST_ASSERT_EQUAL(0, linearBlock.totalStepCount[AXIS_Z_INDEX]);
    TEST_ASSERT(LinearBlockFixedPoint(69175151926106960) == linearBlock.accelerationPerTick[AXIS_X_INDEX]);
    TEST_ASSERT(0.f == linearBlock.accelerationPerTick[AXIS_Y_INDEX]);
    TEST_ASSERT(0.f == linearBlock.accelerationPerTick[AXIS_Z_INDEX]);
    TEST_ASSERT(LinearBlockFixedPoint(-69175463215068856) == linearBlock.decelerationPerTick[AXIS_X_INDEX]);
    TEST_ASSERT(0.f == linearBlock.decelerationPerTick[AXIS_Y_INDEX]);
    TEST_ASSERT(0.f == linearBlock.decelerationPerTick[AXIS_Z_INDEX]);
    TEST_ASSERT(LinearBlockFixedPoint(115292150460684704) == linearBlock.stepPerTick[AXIS_X_INDEX]);
    TEST_ASSERT(0.f == linearBlock.stepPerTick[AXIS_Y_INDEX]);
    TEST_ASSERT(0.f == linearBlock.stepPerTick[AXIS_Z_INDEX]);
    TEST_ASSERT(0.f == linearBlock.stepCounter[AXIS_X_INDEX]);
    TEST_ASSERT(0.f == linearBlock.stepCounter[AXIS_Y_INDEX]);
    TEST_ASSERT(0.f == linearBlock.stepCounter[AXIS_Z_INDEX]);
    TEST_ASSERT(LinearBlockFixedPoint(2305843043573432) == linearBlock.minStepPerTick[AXIS_X_INDEX]);
    TEST_ASSERT(0.f == linearBlock.minStepPerTick[AXIS_Y_INDEX]);
    TEST_ASSERT(0.f == linearBlock.minStepPerTick[AXIS_Z_INDEX]);
    TEST_ASSERT_EQUAL(11483330, linearBlock.durationUs);
    TEST_ASSERT(tl::nullopt == linearBlock.spindleRpm);
}

void test_PlannerBlockToLinearBlock_accelerationDeceleration()
{
    constexpr float ENTRY_FEED_RATE_IN_MM_PER_S = 1.f;
    constexpr float FEED_RATE_IN_MM_PER_S = 10.f;
    constexpr float EXIT_FEED_RATE_IN_MM_PER_S = 6.f;
    constexpr float ACCELERATION_IN_MM_PER_SS = 3.f;

    constexpr float X_STEP_COUNT_PER_MM = 500;
    constexpr float Y_STEP_COUNT_PER_MM = 100;
    constexpr float Z_STEP_COUNT_PER_MM = 500;
    constexpr float MIN_FEED_RATE_IN_MM_PER_S = 0.1;
    constexpr float TICK_FREQUENCY = 100000.f;

    PlannerLine line{{0.f, 0.f, 0.f}, {20.f, 0.f, 0.f}, FEED_RATE_IN_MM_PER_S, tl::nullopt};
    auto plannerBlock = PlannerBlock::fromLine(line, ENTRY_FEED_RATE_IN_MM_PER_S, EXIT_FEED_RATE_IN_MM_PER_S, ACCELERATION_IN_MM_PER_SS);
    TEST_ASSERT_TRUE(plannerBlock.has_value());

    auto linearBlock = plannerBlock->toLinearBlock(X_STEP_COUNT_PER_MM, Y_STEP_COUNT_PER_MM, Z_STEP_COUNT_PER_MM, MIN_FEED_RATE_IN_MM_PER_S, TICK_FREQUENCY);
    TEST_ASSERT_EQUAL(0, linearBlock.currentTick);
    TEST_ASSERT_EQUAL(262001, linearBlock.accelerationUntilTick);
    TEST_ASSERT_EQUAL(262001, linearBlock.plateauUntilTick);
    TEST_ASSERT_EQUAL(357335, linearBlock.decelerationUntilTick);
    TEST_ASSERT_EQUAL(0, linearBlock.currentStepCount[AXIS_X_INDEX]);
    TEST_ASSERT_EQUAL(0, linearBlock.currentStepCount[AXIS_Y_INDEX]);
    TEST_ASSERT_EQUAL(0, linearBlock.currentStepCount[AXIS_Z_INDEX]);
    TEST_ASSERT_EQUAL(10000, linearBlock.totalStepCount[AXIS_X_INDEX]);
    TEST_ASSERT_EQUAL(0, linearBlock.totalStepCount[AXIS_Y_INDEX]);
    TEST_ASSERT_EQUAL(0, linearBlock.totalStepCount[AXIS_Z_INDEX]);
    TEST_ASSERT(LinearBlockFixedPoint(69175224664521872) == linearBlock.accelerationPerTick[AXIS_X_INDEX]);
    TEST_ASSERT(0.f == linearBlock.accelerationPerTick[AXIS_Y_INDEX]);
    TEST_ASSERT(0.f == linearBlock.accelerationPerTick[AXIS_Z_INDEX]);
    TEST_ASSERT(LinearBlockFixedPoint(-69175351828947976) == linearBlock.decelerationPerTick[AXIS_X_INDEX]);
    TEST_ASSERT(0.f == linearBlock.decelerationPerTick[AXIS_Y_INDEX]);
    TEST_ASSERT(0.f == linearBlock.decelerationPerTick[AXIS_Z_INDEX]);
    TEST_ASSERT(LinearBlockFixedPoint(23058430092136940) == linearBlock.stepPerTick[AXIS_X_INDEX]);
    TEST_ASSERT(0.f == linearBlock.stepPerTick[AXIS_Y_INDEX]);
    TEST_ASSERT(0.f == linearBlock.stepPerTick[AXIS_Z_INDEX]);
    TEST_ASSERT(0.f == linearBlock.stepCounter[AXIS_X_INDEX]);
    TEST_ASSERT(0.f == linearBlock.stepCounter[AXIS_Y_INDEX]);
    TEST_ASSERT(0.f == linearBlock.stepCounter[AXIS_Z_INDEX]);
    TEST_ASSERT(LinearBlockFixedPoint(2305843043573432) == linearBlock.minStepPerTick[AXIS_X_INDEX]);
    TEST_ASSERT(0.f == linearBlock.minStepPerTick[AXIS_Y_INDEX]);
    TEST_ASSERT(0.f == linearBlock.minStepPerTick[AXIS_Z_INDEX]);
    TEST_ASSERT_EQUAL(3573350, linearBlock.durationUs);
    TEST_ASSERT(tl::nullopt == linearBlock.spindleRpm);
}
