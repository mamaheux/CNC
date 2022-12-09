#include <unity.h>

#include "../include/collections/test_BoundedQueue.h"

#include "../include/config/test_AnalogInputConfig.h"
#include "../include/config/test_DigitalInputConfig.h"
#include "../include/config/test_DigitalOutputConfig.h"
#include "../include/config/test_PwmOutputConfig.h"

#include "../include/math/test_FixedPoint.h"
#include "../include/math/test_Range.h"
#include "../include/math/test_Vector2.h"
#include "../include/math/test_Vector3.h"
#include "../include/math/test_ZRotation.h"

#include "../include/modules/test_Module.h"
#include "../include/modules/test_CoordinateTransformer.h"
#include "../include/modules/test_ArcConverter.h"
#include "../include/modules/test_Planner.h"

#include "../include/parsing/test_properties.h"
#include "../include/parsing/test_trim.h"
#include "../include/parsing/test_SystemCommand.h"
#include "../include/parsing/test_GCode.h"
#include "../include/parsing/test_MCode.h"

#include "../include/resources/test_MotionLock.h"
#include "../include/resources/test_ResourceLock.h"

void testCollections()
{
    RUN_TEST(test_BoundedQueue_popPush);
    RUN_TEST(test_BoundedQueue_last);
    RUN_TEST(test_BoundedQueue_isEmpty);
    RUN_TEST(test_BoundedQueue_isFull);
    RUN_TEST(test_BoundedQueue_size);
}

void testConfig()
{
    RUN_TEST(test_AnalogInputConfig_constructor);
    RUN_TEST(test_AnalogInputConfig_parse);

    RUN_TEST(test_DigitalInputConfig_constructor);
    RUN_TEST(test_DigitalInputConfig_parse);

    RUN_TEST(test_DigitalOutputConfig_constructor);
    RUN_TEST(test_DigitalOutputConfig_parse);

    RUN_TEST(test_PwmOutputConfig_constructor);
    RUN_TEST(test_PwmOutputConfig_parse);
}

void testMath()
{
    RUN_TEST(test_BitMask);

    RUN_TEST(test_zero_shouldBeZero);
    RUN_TEST(test_one_shouldBeOne);

    RUN_TEST(test_FixedPointQ3_4_multiplyAccumulate);
    RUN_TEST(test_FixedPointQ3_4_multiplySubtract);
    RUN_TEST(test_FixedPointQ3_4_operatorAddAssignation);
    RUN_TEST(test_FixedPointQ3_4_operatorSubtractAssignation);
    RUN_TEST(test_FixedPointQ3_4_operatorMultiplyAssignation);
    RUN_TEST(test_FixedPointQ3_4_operatorDivideAssignation);
    RUN_TEST(test_FixedPointQ3_4_operatorIncrement);
    RUN_TEST(test_FixedPointQ3_4_operatorDecrement);
    RUN_TEST(test_FixedPointQ3_4_operatorUnaryMinus);
    RUN_TEST(test_FixedPointQ3_4_operatorAdd);
    RUN_TEST(test_FixedPointQ3_4_operatorSubtract);
    RUN_TEST(test_FixedPointQ3_4_operatorMultiply);
    RUN_TEST(test_FixedPointQ3_4_operatorDivide);
    RUN_TEST(test_FixedPointQ3_4_operatorEqualTo);
    RUN_TEST(test_FixedPointQ3_4_operatorNotEqualTo);
    RUN_TEST(test_FixedPointQ3_4_operatorLessThan);
    RUN_TEST(test_FixedPointQ3_4_operatorLessThanOrEqualTo);
    RUN_TEST(test_FixedPointQ3_4_operatorGreaterThan);
    RUN_TEST(test_FixedPointQ3_4_operatorGreaterThanOrEqualTo);

    RUN_TEST(test_FixedPointQ7_8_multiplyAccumulate);
    RUN_TEST(test_FixedPointQ7_8_multiplySubtract);
    RUN_TEST(test_FixedPointQ7_8_operatorAddAssignation);
    RUN_TEST(test_FixedPointQ7_8_operatorSubtractAssignation);
    RUN_TEST(test_FixedPointQ7_8_operatorMultiplyAssignation);
    RUN_TEST(test_FixedPointQ7_8_operatorDivideAssignation);
    RUN_TEST(test_FixedPointQ7_8_operatorIncrement);
    RUN_TEST(test_FixedPointQ7_8_operatorDecrement);
    RUN_TEST(test_FixedPointQ7_8_operatorUnaryMinus);
    RUN_TEST(test_FixedPointQ7_8_operatorAdd);
    RUN_TEST(test_FixedPointQ7_8_operatorSubtract);
    RUN_TEST(test_FixedPointQ7_8_operatorMultiply);
    RUN_TEST(test_FixedPointQ7_8_operatorDivide);
    RUN_TEST(test_FixedPointQ7_8_operatorEqualTo);
    RUN_TEST(test_FixedPointQ7_8_operatorNotEqualTo);
    RUN_TEST(test_FixedPointQ7_8_operatorLessThan);
    RUN_TEST(test_FixedPointQ7_8_operatorLessThanOrEqualTo);
    RUN_TEST(test_FixedPointQ7_8_operatorGreaterThan);
    RUN_TEST(test_FixedPointQ7_8_operatorGreaterThanOrEqualTo);

    RUN_TEST(test_FixedPointQ15_16_multiplyAccumulate);
    RUN_TEST(test_FixedPointQ15_16_multiplySubtract);
    RUN_TEST(test_FixedPointQ15_16_operatorAddAssignation);
    RUN_TEST(test_FixedPointQ15_16_operatorSubtractAssignation);
    RUN_TEST(test_FixedPointQ15_16_operatorMultiplyAssignation);
    RUN_TEST(test_FixedPointQ15_16_operatorDivideAssignation);
    RUN_TEST(test_FixedPointQ15_16_operatorIncrement);
    RUN_TEST(test_FixedPointQ15_16_operatorDecrement);
    RUN_TEST(test_FixedPointQ15_16_operatorUnaryMinus);
    RUN_TEST(test_FixedPointQ15_16_operatorAdd);
    RUN_TEST(test_FixedPointQ15_16_operatorSubtract);
    RUN_TEST(test_FixedPointQ15_16_operatorMultiply);
    RUN_TEST(test_FixedPointQ15_16_operatorDivide);
    RUN_TEST(test_FixedPointQ15_16_operatorEqualTo);
    RUN_TEST(test_FixedPointQ15_16_operatorNotEqualTo);
    RUN_TEST(test_FixedPointQ15_16_operatorLessThan);
    RUN_TEST(test_FixedPointQ15_16_operatorLessThanOrEqualTo);
    RUN_TEST(test_FixedPointQ15_16_operatorGreaterThan);
    RUN_TEST(test_FixedPointQ15_16_operatorGreaterThanOrEqualTo);

    RUN_TEST(test_FixedPointQ31_32_operatorAddAssignation);
    RUN_TEST(test_FixedPointQ31_32_operatorSubtractAssignation);
    RUN_TEST(test_FixedPointQ31_32_operatorIncrement);
    RUN_TEST(test_FixedPointQ31_32_operatorDecrement);
    RUN_TEST(test_FixedPointQ31_32_operatorUnaryMinus);
    RUN_TEST(test_FixedPointQ31_32_operatorAdd);
    RUN_TEST(test_FixedPointQ31_32_operatorSubtract);
    RUN_TEST(test_FixedPointQ31_32_operatorEqualTo);
    RUN_TEST(test_FixedPointQ31_32_operatorNotEqualTo);
    RUN_TEST(test_FixedPointQ31_32_operatorLessThan);
    RUN_TEST(test_FixedPointQ31_32_operatorLessThanOrEqualTo);
    RUN_TEST(test_FixedPointQ31_32_operatorGreaterThan);
    RUN_TEST(test_FixedPointQ31_32_operatorGreaterThanOrEqualTo);

    RUN_TEST(test_InclusiveRange3);

    RUN_TEST(test_Vector2_constructor);
    RUN_TEST(test_Vector2_dot);
    RUN_TEST(test_Vector2_angle);
    RUN_TEST(test_Vector2_norm);
    RUN_TEST(test_Vector2_normalize);
    RUN_TEST(test_Vector2_normalized);
    RUN_TEST(test_Vector2_add);
    RUN_TEST(test_Vector2_addAssign);
    RUN_TEST(test_Vector2_substract);
    RUN_TEST(test_Vector2_substractAssign);
    RUN_TEST(test_Vector2_multiply);
    RUN_TEST(test_Vector2_multiplyAssign);
    RUN_TEST(test_Vector2_divide);
    RUN_TEST(test_Vector2_divideAssign);

    RUN_TEST(test_Vector3_constructor);
    RUN_TEST(test_Vector3_dot);
    RUN_TEST(test_Vector3_angle);
    RUN_TEST(test_Vector3_cross);
    RUN_TEST(test_Vector3_norm);
    RUN_TEST(test_Vector3_normalize);
    RUN_TEST(test_Vector3_normalized);
    RUN_TEST(test_Vector3_add);
    RUN_TEST(test_Vector3_addAssign);
    RUN_TEST(test_Vector3_substract);
    RUN_TEST(test_Vector3_substractAssign);
    RUN_TEST(test_Vector3_multiply);
    RUN_TEST(test_Vector3_multiplyAssign);
    RUN_TEST(test_Vector3_divide);
    RUN_TEST(test_Vector3_divideAssign);

    RUN_TEST(test_degToRad);
    RUN_TEST(test_radToDeg);
    RUN_TEST(test_ZRotation_rotate);
}

void testModules()
{
    RUN_TEST(test_CommandResult_ok);
    RUN_TEST(test_CommandResult_okResponseSent);
    RUN_TEST(test_CommandResult_pending);
    RUN_TEST(test_CommandResult_error);
    RUN_TEST(test_CommandResult_notHandled);
    RUN_TEST(test_CmmandResult_agregate);

    RUN_TEST(test_CoordinateTransformer_G10L2P10);
    RUN_TEST(test_CoordinateTransformer_G10L2);
    RUN_TEST(test_CoordinateTransformer_G10L20);
    RUN_TEST(test_CoordinateTransformer_G20G21);
    RUN_TEST(test_CoordinateTransformer_G90G91);
    RUN_TEST(test_CoordinateTransformer_G92);

    RUN_TEST(test_getCenterPointFromRadius);
    RUN_TEST(test_ArcConverter_invalidCode);
    RUN_TEST(test_ArcConverter_invalidCenterPoint);
    RUN_TEST(test_ArcConverter_invalidRadius);
    RUN_TEST(test_ArcConverter_invalidP);
    RUN_TEST(test_ArcConverter_absoluteR);
    RUN_TEST(test_ArcConverter_g2XY);
    RUN_TEST(test_ArcConverter_g3XY);
    RUN_TEST(test_ArcConverter_helixXY);
    RUN_TEST(test_ArcConverter_fullXY);
    RUN_TEST(test_ArcConverter_rXY);
    RUN_TEST(test_ArcConverter_p2XY);
    RUN_TEST(test_ArcConverter_offsetAndAbsolute);
    RUN_TEST(test_ArcConverter_g53);
    RUN_TEST(test_ArcConverter_g2XZ);
    RUN_TEST(test_ArcConverter_g3XZ);
    RUN_TEST(test_ArcConverter_g2YZ);
    RUN_TEST(test_ArcConverter_g3YZ);
    RUN_TEST(test_ArcConverter_g2Offset);
    RUN_TEST(test_ArcConverter_g2Rotation);
    RUN_TEST(test_ArcConverter_g2OffsetRotation);

    RUN_TEST(test_junctionFeedRateInMmPerS_notValidLines);
    RUN_TEST(test_junctionFeedRateInMmPerS_0Deg);
    RUN_TEST(test_junctionFeedRateInMmPerS_45Deg);
    RUN_TEST(test_junctionFeedRateInMmPerS_90Deg);
    RUN_TEST(test_junctionFeedRateInMmPerS_135Deg);
    RUN_TEST(test_junctionFeedRateInMmPerS_180Deg);
}

void testParsing()
{
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

    RUN_TEST(test_GCode_g1);
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
    RUN_TEST(test_GCode_clear);

    RUN_TEST(test_parseMCode_empty);
    RUN_TEST(test_parseMCode_tooBig);
    RUN_TEST(test_parseMCode_M3);
    RUN_TEST(test_parseMCode_M5);
    RUN_TEST(test_parseMCode_M17);
    RUN_TEST(test_parseMCode_M18);
    RUN_TEST(test_parseMCode_M84);
    RUN_TEST(test_parseMCode_M20);
    RUN_TEST(test_parseMCode_M23);
    RUN_TEST(test_parseMCode_M24);
    RUN_TEST(test_parseMCode_M25);
    RUN_TEST(test_parseMCode_M26);
    RUN_TEST(test_parseMCode_M27);
    RUN_TEST(test_parseMCode_M28);
    RUN_TEST(test_parseMCode_M29);
    RUN_TEST(test_parseMCode_M30);
    RUN_TEST(test_parseMCode_M32);
    RUN_TEST(test_parseMCode_M114);
    RUN_TEST(test_parseMCode_M114D1);
    RUN_TEST(test_parseMCode_M114D2);
    RUN_TEST(test_parseMCode_M114D3);
    RUN_TEST(test_parseMCode_M203);
    RUN_TEST(test_parseMCode_M204);
    RUN_TEST(test_parseMCode_M220);
    RUN_TEST(test_parseMCode_M957);
    RUN_TEST(test_parseMCode_M958);
    RUN_TEST(test_MCode_clear);
    RUN_TEST(test_MCode_clearM32);
}

void testResources()
{
    RUN_TEST(test_MotionLock);

    RUN_TEST(test_PinLock_isNotRecursive);
    RUN_TEST(test_PinLock_samePin);

    RUN_TEST(test_PwmLock_invalidPin);
    RUN_TEST(test_PwmLock_isNotRecursive);
    RUN_TEST(test_PwmLock_sameGroup);
}

int main(int argc, char** argv)
{
    UNITY_BEGIN();

    testCollections();
    testConfig();
    testMath();
    testModules();
    testParsing();
    testResources();

    UNITY_END();

    return 0;
}
