#include <unity.h>

#include "test_properties.h"
#include "test_trim.h"
#include "test_SystemCommand.h"
#include "test_GCode.h"
#include "test_MCode.h"

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

  RUN_TEST(test_parseSystemCommand_empty);
  RUN_TEST(test_parseSystemCommand_invalid);
  RUN_TEST(test_parseSystemCommand_homing);

  RUN_TEST(test_parseGCode_empty);
  RUN_TEST(test_parseGCode_tooBig);
  RUN_TEST(test_parseGCode_noModalMove);
  RUN_TEST(test_parseGCode_G0);
  RUN_TEST(test_parseGCode_G1);
  RUN_TEST(test_parseGCode_G2);
  RUN_TEST(test_parseGCode_G3);
  RUN_TEST(test_parseGCode_G4);
  RUN_TEST(test_parseGCode_G10L2);
  RUN_TEST(test_parseGCode_G10L20);
  RUN_TEST(test_parseGCode_G17);
  RUN_TEST(test_parseGCode_G18);
  RUN_TEST(test_parseGCode_G19);
  RUN_TEST(test_parseGCode_G20);
  RUN_TEST(test_parseGCode_G21);
  RUN_TEST(test_parseGCode_G28);
  RUN_TEST(test_parseGCode_G28D1);
  RUN_TEST(test_parseGCode_G53);
  RUN_TEST(test_parseGCode_G53NextLine);
  RUN_TEST(test_parseGCode_G54);
  RUN_TEST(test_parseGCode_G55);
  RUN_TEST(test_parseGCode_G56);
  RUN_TEST(test_parseGCode_G57);
  RUN_TEST(test_parseGCode_G58);
  RUN_TEST(test_parseGCode_G59);
  RUN_TEST(test_parseGCode_G59D1);
  RUN_TEST(test_parseGCode_G59D2);
  RUN_TEST(test_parseGCode_G59D3);
  RUN_TEST(test_parseGCode_G90);
  RUN_TEST(test_parseGCode_G91);
  RUN_TEST(test_parseGCode_G92);
  RUN_TEST(test_parseGCode_G92D1);

  RUN_TEST(test_parseMCode_empty);
  RUN_TEST(test_parseMCode_tooBig);
  RUN_TEST(test_parseGCode_M3);
  RUN_TEST(test_parseGCode_M5);
  RUN_TEST(test_parseGCode_M17);
  RUN_TEST(test_parseGCode_M18);
  RUN_TEST(test_parseGCode_M84);
  RUN_TEST(test_parseGCode_M20);
  RUN_TEST(test_parseGCode_M23);
  RUN_TEST(test_parseGCode_M24);
  RUN_TEST(test_parseGCode_M25);
  RUN_TEST(test_parseGCode_M26);
  RUN_TEST(test_parseGCode_M27);
  RUN_TEST(test_parseGCode_M28);
  RUN_TEST(test_parseGCode_M29);
  RUN_TEST(test_parseGCode_M30);
  RUN_TEST(test_parseGCode_M32);
  RUN_TEST(test_parseGCode_M114);
  RUN_TEST(test_parseGCode_M114D1);
  RUN_TEST(test_parseGCode_M114D2);
  RUN_TEST(test_parseGCode_M114D3);
  RUN_TEST(test_parseGCode_M203);
  RUN_TEST(test_parseGCode_M204);
  RUN_TEST(test_parseGCode_M220);
  RUN_TEST(test_parseGCode_M957);
  RUN_TEST(test_parseGCode_M958);

  UNITY_END();

  return 0;
}
