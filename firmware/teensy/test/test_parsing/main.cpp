#include <unity.h>

#include "test_properties.h"
#include "test_trim.h"

int main(int argc, char **argv) {
  UNITY_BEGIN();

  RUN_TEST(test_parsePropertiesLine_comment);
  RUN_TEST(test_parsePropertiesLine_empty);
  RUN_TEST(test_parsePropertiesLine_valueTrailingSpaceAndComment);
  RUN_TEST(test_parsePropertiesLine_keyTrailingSpace);
  RUN_TEST(test_parsePropertiesLine_valueTrailingSpace);
  RUN_TEST(test_parsePropertiesLine_commentedValue);
  RUN_TEST(test_parsePropertiesLine_manySpaces);
  RUN_TEST(test_parsePropertiesLine_emptyKey);
  RUN_TEST(test_parsePropertiesLine_emptyKeyAndLeadingSpace);
  RUN_TEST(test_parsePropertiesLine_emptyValue);
  RUN_TEST(test_parsePropertiesLine_emptyValueAndTrailingSpace);
  RUN_TEST(test_parsePropertiesLine_newLine);

  RUN_TEST(test_ltrim_oneSpace);
  RUN_TEST(test_ltrim_twoSpace);
  RUN_TEST(test_ltrim_allSpaceCharacters);
  RUN_TEST(test_ltrim_noSpace);
  RUN_TEST(test_ltrim_empty);

  RUN_TEST(test_rtrim_oneSpace);
  RUN_TEST(test_rtrim_twoSpace);
  RUN_TEST(test_rtrim_allSpaceCharacters);
  RUN_TEST(test_rtrim_noSpace);
  RUN_TEST(test_rtrim_empty);

  RUN_TEST(test_trim_oneSpace);
  RUN_TEST(test_trim_twoSpace);
  RUN_TEST(test_trim_allSpaceCharacters);
  RUN_TEST(test_trim_noSpace);
  RUN_TEST(test_trim_empty);

  UNITY_END();

  return 0;
}
