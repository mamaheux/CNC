#include <cnc/parsing/SystemCommand.h>
#include <unity.h>

void test_parseSystemCommand_empty()
{
    SystemCommand command;
    SystemCommandParser parser;

    TEST_ASSERT_EQUAL(ParsingResult::NEXT_LINE_NEEDED, parser.parse("", command));
    TEST_ASSERT_EQUAL(ParsingResult::NEXT_LINE_NEEDED, parser.parse(" ", command));
    TEST_ASSERT_EQUAL(ParsingResult::NEXT_LINE_NEEDED, parser.parse("; bob", command));
    TEST_ASSERT_EQUAL(ParsingResult::NEXT_LINE_NEEDED, parser.parse(" ; bob", command));
}

void test_parseSystemCommand_invalid()
{
    SystemCommand command;
    SystemCommandParser parser;

    TEST_ASSERT_EQUAL(ParsingResult::ERROR, parser.parse("$asd", command));
}

void test_parseSystemCommand_homing()
{
    SystemCommand command;
    SystemCommandParser parser;

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("$H", command));
    TEST_ASSERT_EQUAL(SystemCommand::HOMING, command);

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse(" $H; comment", command));
    TEST_ASSERT_EQUAL(SystemCommand::HOMING, command);

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("   $H ; comment", command));
    TEST_ASSERT_EQUAL(SystemCommand::HOMING, command);
}
