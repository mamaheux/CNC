#include <cnc/resources/MotionLock.h>
#include <unity.h>

static MCode toMCode(const char* command)
{
    MCodeParser parser;
    MCode mcode;
    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse(command, mcode));
    return mcode;
}

void test_MotionLock()
{
    MotionLock testee;

    GCode g1 = GCode::g1(Vector3<float>(1.f, 2.f, 3.f), 5.f, true);
    MCode m3 = toMCode("M3");
    MCode m114 = toMCode("M114");

    TEST_ASSERT_TRUE(testee.tryLock(SystemCommand::HOMING, CommandSource::SERIAL_SOURCE));
    TEST_ASSERT_TRUE(testee.tryLock(g1, CommandSource::SERIAL_SOURCE));
    TEST_ASSERT_TRUE(testee.tryLock(m3, CommandSource::SERIAL_SOURCE));

    TEST_ASSERT_FALSE(testee.tryLock(g1, CommandSource::FILE_SOURCE));
    TEST_ASSERT_FALSE(testee.tryLock(m3, CommandSource::FILE_SOURCE));
    TEST_ASSERT_TRUE(testee.tryLock(m114, CommandSource::FILE_SOURCE));

    testee.unlock();

    TEST_ASSERT_TRUE(testee.tryLock(g1, CommandSource::FILE_SOURCE));
    TEST_ASSERT_FALSE(testee.tryLock(SystemCommand::HOMING, CommandSource::SERIAL_SOURCE));
    TEST_ASSERT_FALSE(testee.tryLock(m3, CommandSource::SERIAL_SOURCE));
    TEST_ASSERT_TRUE(testee.tryLock(m114, CommandSource::SERIAL_SOURCE));

    testee.unlock();

    TEST_ASSERT_TRUE(testee.tryLock(m114, CommandSource::FILE_SOURCE));
    TEST_ASSERT_TRUE(testee.tryLock(m3, CommandSource::SERIAL_SOURCE));
    TEST_ASSERT_FALSE(testee.tryLock(SystemCommand::HOMING, CommandSource::FILE_SOURCE));
    TEST_ASSERT_FALSE(testee.tryLock(g1, CommandSource::FILE_SOURCE));
    TEST_ASSERT_TRUE(testee.tryLock(m114, CommandSource::FILE_SOURCE));
}
