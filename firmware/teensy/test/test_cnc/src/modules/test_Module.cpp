#include <cnc/modules/Module.h>
#include <unity.h>

void test_CommandResult_ok() {
  CommandResult result = CommandResult::ok();

  TEST_ASSERT(result.type() == CommandResultType::OK);
  TEST_ASSERT_EQUAL_STRING("", result.errorMessage());
}

void test_CommandResult_okResponseSent() {
  CommandResult result = CommandResult::okResponseSent();

  TEST_ASSERT(result.type() == CommandResultType::OK_RESPONSE_SENT);
  TEST_ASSERT_EQUAL_STRING("", result.errorMessage());
}

void test_CommandResult_pending() {
  CommandResult result = CommandResult::pending();

  TEST_ASSERT(result.type() == CommandResultType::PENDING);
  TEST_ASSERT_EQUAL_STRING("", result.errorMessage());
}

void test_CommandResult_error() {
  CommandResult result = CommandResult::error("e");

  TEST_ASSERT(result.type() == CommandResultType::ERROR);
  TEST_ASSERT_EQUAL_STRING("e", result.errorMessage());
}

void test_CommandResult_notHandled() {
  CommandResult result = CommandResult::notHandled();

  TEST_ASSERT(result.type() == CommandResultType::NOT_HANDLED);
  TEST_ASSERT_EQUAL_STRING("", result.errorMessage());
}

void test_CmmandResult_agregate() {
  CommandResult result = CommandResult::notHandled();

  result = result.agregate(CommandResult::notHandled());
  TEST_ASSERT(result.type() == CommandResultType::NOT_HANDLED);
  TEST_ASSERT_EQUAL_STRING("", result.errorMessage());

  result = result.agregate(CommandResult::ok());
  TEST_ASSERT(result.type() == CommandResultType::OK);
  TEST_ASSERT_EQUAL_STRING("", result.errorMessage());

  result = result.agregate(CommandResult::pending());
  TEST_ASSERT(result.type() == CommandResultType::PENDING);
  TEST_ASSERT_EQUAL_STRING("", result.errorMessage());

  result = result.agregate(CommandResult::okResponseSent());
  TEST_ASSERT(result.type() == CommandResultType::OK_RESPONSE_SENT);
  TEST_ASSERT_EQUAL_STRING("", result.errorMessage());

  result = result.agregate(CommandResult::error("e"));
  TEST_ASSERT(result.type() == CommandResultType::ERROR);
  TEST_ASSERT_EQUAL_STRING("e", result.errorMessage());

  result = CommandResult::notHandled().agregate(CommandResult::notHandled());
  TEST_ASSERT(result.type() == CommandResultType::NOT_HANDLED);
  TEST_ASSERT_EQUAL_STRING("", result.errorMessage());

  result = CommandResult::ok().agregate(CommandResult::notHandled());
  TEST_ASSERT(result.type() == CommandResultType::OK);
  TEST_ASSERT_EQUAL_STRING("", result.errorMessage());

  result = CommandResult::pending().agregate(CommandResult::notHandled());
  TEST_ASSERT(result.type() == CommandResultType::PENDING);
  TEST_ASSERT_EQUAL_STRING("", result.errorMessage());

  result = CommandResult::okResponseSent().agregate(CommandResult::notHandled());
  TEST_ASSERT(result.type() == CommandResultType::OK_RESPONSE_SENT);
  TEST_ASSERT_EQUAL_STRING("", result.errorMessage());

  result = CommandResult::error("e").agregate(CommandResult::notHandled());
  TEST_ASSERT(result.type() == CommandResultType::ERROR);
  TEST_ASSERT_EQUAL_STRING("e", result.errorMessage());
}
