#include <cnc/math/FixedPoint.h>
#include <unity.h>

#define TEST_ASSERT_EQUAL_FIXED_POINT(actual, expect) TEST_ASSERT((actual) == (expect))

void test_BitMask()
{
    auto r0 = BitMask<uint8_t, 1, 0>::Value;
    auto r1 = BitMask<uint8_t, 2, 0>::Value;
    auto r2 = BitMask<uint8_t, 3, 0>::Value;
    auto r3 = BitMask<uint8_t, 4, 0>::Value;
    auto r4 = BitMask<uint8_t, 5, 0>::Value;
    auto r5 = BitMask<uint8_t, 6, 0>::Value;
    auto r6 = BitMask<uint8_t, 7, 0>::Value;
    auto r7 = BitMask<uint8_t, 8, 0>::Value;

    TEST_ASSERT_EQUAL(0b0000'0001, r0);
    TEST_ASSERT_EQUAL(0b0000'0011, r1);
    TEST_ASSERT_EQUAL(0b0000'0111, r2);
    TEST_ASSERT_EQUAL(0b0000'1111, r3);
    TEST_ASSERT_EQUAL(0b0001'1111, r4);
    TEST_ASSERT_EQUAL(0b0011'1111, r5);
    TEST_ASSERT_EQUAL(0b0111'1111, r6);
    TEST_ASSERT_EQUAL(0b1111'1111, r7);

    r0 = BitMask<uint8_t, 2, 0>::Value;
    r1 = BitMask<uint8_t, 2, 1>::Value;
    r2 = BitMask<uint8_t, 2, 2>::Value;
    r3 = BitMask<uint8_t, 2, 3>::Value;
    r4 = BitMask<uint8_t, 2, 4>::Value;
    r5 = BitMask<uint8_t, 2, 5>::Value;
    r6 = BitMask<uint8_t, 2, 6>::Value;
    r7 = BitMask<uint8_t, 2, 7>::Value;
    auto r8 = BitMask<uint8_t, 2, 8>::Value;

    TEST_ASSERT_EQUAL(0b0000'0011, r0);
    TEST_ASSERT_EQUAL(0b0000'0110, r1);
    TEST_ASSERT_EQUAL(0b0000'1100, r2);
    TEST_ASSERT_EQUAL(0b0001'1000, r3);
    TEST_ASSERT_EQUAL(0b0011'0000, r4);
    TEST_ASSERT_EQUAL(0b0110'0000, r5);
    TEST_ASSERT_EQUAL(0b1100'0000, r6);
    TEST_ASSERT_EQUAL(0b1000'0000, r7);
    TEST_ASSERT_EQUAL(0b0000'0000, r8);
}

void test_zero_shouldBeZero()
{
    FixedPointQ3_4 testee(1.0);

    TEST_ASSERT_EQUAL_FIXED_POINT(testee + FixedPointQ3_4::ZERO, 1.0);
}

void test_one_shouldBeOne()
{
    FixedPointQ3_4 testee(1.0);

    TEST_ASSERT_EQUAL_FIXED_POINT(testee + FixedPointQ3_4::ONE, 2.0);
}

void test_FixedPointQ3_4_multiplyAccumulate()
{
    FixedPointQ3_4 testee1(1.0);
    testee1.multiplyAccumulate(0.5, 2.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(testee1, 2.0);

    FixedPointQ3_4 testee2(-1.0);
    testee2.multiplyAccumulate(0.5, 2.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(testee2, 0.0);

    FixedPointQ3_4 testee3(1.0);
    testee3.multiplyAccumulate(-0.5, 2.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(testee3, 0.0);

    FixedPointQ3_4 testee4(1.0);
    testee4.multiplyAccumulate(0.5, -1.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(testee4, 0.5);
}

void test_FixedPointQ3_4_multiplySubtract()
{
    FixedPointQ3_4 testee1(1.0);
    testee1.multiplySubtract(0.5, 2.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(testee1, 0.0);

    FixedPointQ3_4 testee2(-1.0);
    testee2.multiplySubtract(0.5, 2.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(testee2, -2.0);

    FixedPointQ3_4 testee3(1.0);
    testee3.multiplySubtract(-0.5, 2.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(testee3, 2.0);

    FixedPointQ3_4 testee4(1.0);
    testee4.multiplySubtract(0.5, -1.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(testee4, 1.5);
}

void test_FixedPointQ3_4_operatorAddAssignation()
{
    FixedPointQ3_4 testee1(1.0);
    testee1 += 1.25;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee1, 2.25);

    FixedPointQ3_4 testee2(-1.0);
    testee2 += 1.75;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee2, 0.75);

    FixedPointQ3_4 testee3(1.0);
    testee3 += -1.5;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee3, -0.5);

    FixedPointQ3_4 testee4(-1.0);
    testee4 += -1.25;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee4, -2.25);
}

void test_FixedPointQ3_4_operatorSubtractAssignation()
{
    FixedPointQ3_4 testee1(1.0);
    testee1 -= 1.25;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee1, -0.25);

    FixedPointQ3_4 testee2(-1.0);
    testee2 -= 1.75;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee2, -2.75);

    FixedPointQ3_4 testee3(1.0);
    testee3 -= -1.5;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee3, 2.5);

    FixedPointQ3_4 testee4(-1.0);
    testee4 -= -1.5;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee4, 0.5);
}

void test_FixedPointQ3_4_operatorMultiplyAssignation()
{
    FixedPointQ3_4 testee1(1.0);
    testee1 *= 1.25;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee1, 1.25);

    FixedPointQ3_4 testee2(-1.0);
    testee2 *= 1.75;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee2, -1.75);

    FixedPointQ3_4 testee3(1.0);
    testee3 *= -1.5;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee3, -1.5);

    FixedPointQ3_4 testee4(-1.0);
    testee4 *= -1.625;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee4, 1.625);
}

void test_FixedPointQ3_4_operatorDivideAssignation()
{
    FixedPointQ3_4 testee1(1.0);
    testee1 /= 0.25;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee1, 4.0);

    FixedPointQ3_4 testee2(-1.0);
    testee2 /= 0.25;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee2, -4.0);

    FixedPointQ3_4 testee3(1.0);
    testee3 /= -0.5;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee3, -2.0);

    FixedPointQ3_4 testee4(-1.0);
    testee4 /= -4.0;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee4, 0.25);
}

void test_FixedPointQ3_4_operatorIncrement()
{
    FixedPointQ3_4 testee(1.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(++testee, 2.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(testee++, 2.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(testee, 3.0);
}

void test_FixedPointQ3_4_operatorDecrement()
{
    FixedPointQ3_4 testee(2.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(--testee, 1.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(testee--, 1.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(testee, 0.0);
}

void test_FixedPointQ3_4_operatorUnaryMinus()
{
    TEST_ASSERT_EQUAL_FIXED_POINT(-FixedPointQ3_4(1.0), -1.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(-FixedPointQ3_4(0.0), 0.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(-FixedPointQ3_4(-1.0), 1.0);
}

void test_FixedPointQ3_4_operatorAdd()
{
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ3_4(1.0) + 1.25, 2.25);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ3_4(-1.0) + 1.75, 0.75);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ3_4(1.0) + -1.5, -0.5);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ3_4(-1.0) + -1.5, -2.5);
}

void test_FixedPointQ3_4_operatorSubtract()
{
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ3_4(1.0) - 1.25, -0.25);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ3_4(-1.0) - 1.75, -2.75);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ3_4(1.0) - -1.5, 2.5);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ3_4(-1.0) - -1.5, 0.5);
}

void test_FixedPointQ3_4_operatorMultiply()
{
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ3_4(1.0) * 1.25, 1.25);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ3_4(-1.0) * 1.75, -1.75);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ3_4(1.0) * -1.5, -1.5);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ3_4(-1.0) * -1.25, 1.25);
}

void test_FixedPointQ3_4_operatorDivide()
{
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ3_4(1.0) / 0.25, 4.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ3_4(-1.0) / 0.5, -2.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ3_4(1.0) / -0.5, -2.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ3_4(-1.0) / -0.25, 4.0);
}

void test_FixedPointQ3_4_operatorEqualTo()
{
    TEST_ASSERT_TRUE(FixedPointQ3_4(10.0) == 10.0);
    TEST_ASSERT_FALSE(FixedPointQ3_4(10.0) == 10.1);
}

void test_FixedPointQ3_4_operatorNotEqualTo()
{
    TEST_ASSERT_FALSE(FixedPointQ3_4(10.0) != 10.0);
    TEST_ASSERT_TRUE(FixedPointQ3_4(10.0) != 10.1);
}

void test_FixedPointQ3_4_operatorLessThan()
{
    TEST_ASSERT_TRUE(FixedPointQ3_4(10.0) < 11.0);
    TEST_ASSERT_FALSE(FixedPointQ3_4(10.0) < 9.0);
}

void test_FixedPointQ3_4_operatorLessThanOrEqualTo()
{
    TEST_ASSERT_TRUE(FixedPointQ3_4(10.0) <= 11.0);
    TEST_ASSERT_TRUE(FixedPointQ3_4(10.0) <= 10.0);
    TEST_ASSERT_FALSE(FixedPointQ3_4(10.0) < 9.0);
}

void test_FixedPointQ3_4_operatorGreaterThan()
{
    TEST_ASSERT_TRUE(FixedPointQ3_4(11.0) > 10.0);
    TEST_ASSERT_FALSE(FixedPointQ3_4(9.0) > 10.0);
}

void test_FixedPointQ3_4_operatorGreaterThanOrEqualTo()
{
    TEST_ASSERT_TRUE(FixedPointQ3_4(11.0) >= 10.0);
    TEST_ASSERT_TRUE(FixedPointQ3_4(10.0) >= 10.0);
    TEST_ASSERT_FALSE(FixedPointQ3_4(9.0) >= 10.0);
}


void test_FixedPointQ7_8_multiplyAccumulate()
{
    FixedPointQ7_8 testee1(10.0);
    testee1.multiplyAccumulate(0.5, 20.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(testee1, 20.0);

    FixedPointQ7_8 testee2(-10.0);
    testee2.multiplyAccumulate(0.5, 20.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(testee2, 0.0);

    FixedPointQ7_8 testee3(10.0);
    testee3.multiplyAccumulate(-0.5, 20.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(testee3, 0.0);

    FixedPointQ7_8 testee4(10.0);
    testee4.multiplyAccumulate(0.5, -10.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(testee4, 5.0);
}

void test_FixedPointQ7_8_multiplySubtract()
{
    FixedPointQ7_8 testee1(1.0);
    testee1.multiplySubtract(0.5, 2.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(testee1, 0.0);

    FixedPointQ7_8 testee2(-1.0);
    testee2.multiplySubtract(0.5, 2.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(testee2, -2.0);

    FixedPointQ7_8 testee3(1.0);
    testee3.multiplySubtract(-0.5, 2.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(testee3, 2.0);

    FixedPointQ7_8 testee4(1.0);
    testee4.multiplySubtract(0.5, -1.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(testee4, 1.5);
}

void test_FixedPointQ7_8_operatorAddAssignation()
{
    FixedPointQ7_8 testee1(10.0);
    testee1 += 1.25;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee1, 11.25);

    FixedPointQ7_8 testee2(-10.0);
    testee2 += 1.75;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee2, -8.25);

    FixedPointQ7_8 testee3(10.0);
    testee3 += -1.5;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee3, 8.5);

    FixedPointQ7_8 testee4(-10.0);
    testee4 += -1.625;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee4, -11.625);
}

void test_FixedPointQ7_8_operatorSubtractAssignation()
{
    FixedPointQ7_8 testee1(10.0);
    testee1 -= 1.25;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee1, 8.75);

    FixedPointQ7_8 testee2(-10.0);
    testee2 -= 1.75;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee2, -11.75);

    FixedPointQ7_8 testee3(10.0);
    testee3 -= -1.5;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee3, 11.5);

    FixedPointQ7_8 testee4(-10.0);
    testee4 -= -1.625;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee4, -8.375);
}

void test_FixedPointQ7_8_operatorMultiplyAssignation()
{
    FixedPointQ7_8 testee1(10.0);
    testee1 *= 1.25;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee1, 12.5);

    FixedPointQ7_8 testee2(-10.0);
    testee2 *= 1.75;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee2, -17.5);

    FixedPointQ7_8 testee3(10.0);
    testee3 *= -1.5;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee3, -15.0);

    FixedPointQ7_8 testee4(-10.0);
    testee4 *= -1.625;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee4, 16.25);
}

void test_FixedPointQ7_8_operatorDivideAssignation()
{
    FixedPointQ7_8 testee1(10.0);
    testee1 /= 1.25;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee1, 8.0);

    FixedPointQ7_8 testee2(-10.0);
    testee2 /= 2.5;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee2, -4.0);

    FixedPointQ7_8 testee3(10.0);
    testee3 /= -5.0;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee3, -2.0);

    FixedPointQ7_8 testee4(-10.0);
    testee4 /= -8.0;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee4, 1.25);
}

void test_FixedPointQ7_8_operatorIncrement()
{
    FixedPointQ7_8 testee(10.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(++testee, 11.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(testee++, 11.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(testee, 12.0);
}

void test_FixedPointQ7_8_operatorDecrement()
{
    FixedPointQ7_8 testee(10.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(--testee, 9.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(testee--, 9.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(testee, 8.0);
}

void test_FixedPointQ7_8_operatorUnaryMinus()
{
    TEST_ASSERT_EQUAL_FIXED_POINT(-FixedPointQ7_8(10.0), -10.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(-FixedPointQ7_8(0.0), 0.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(-FixedPointQ7_8(-10.0), 10.0);
}

void test_FixedPointQ7_8_operatorAdd()
{
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ7_8(10.0) + 1.25, 11.25);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ7_8(-10.0) + 1.75, -8.25);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ7_8(10.0) + -1.5, 8.5);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ7_8(-10.0) + -1.625, -11.625);
}

void test_FixedPointQ7_8_operatorSubtract()
{
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ7_8(10.0) - 1.25, 8.75);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ7_8(-10.0) - 1.75, -11.75);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ7_8(10.0) - -1.5, 11.5);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ7_8(-10.0) - -1.625, -8.375);
}

void test_FixedPointQ7_8_operatorMultiply()
{
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ7_8(10.0) * 1.25, 12.5);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ7_8(-10.0) * 1.75, -17.5);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ7_8(10.0) * -1.5, -15.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ7_8(-10.0) * -1.625, 16.25);
}

void test_FixedPointQ7_8_operatorDivide()
{
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ7_8(10.0) / 1.25, 8.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ7_8(-10.0) / 2.5, -4.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ7_8(10.0) / -5.0, -2.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ7_8(-10.0) / -8.0, 1.25);
}

void test_FixedPointQ7_8_operatorEqualTo()
{
    TEST_ASSERT_TRUE(FixedPointQ7_8(10.0) == 10.0);
    TEST_ASSERT_FALSE(FixedPointQ7_8(10.0) == 10.1);
}

void test_FixedPointQ7_8_operatorNotEqualTo()
{
    TEST_ASSERT_FALSE(FixedPointQ7_8(10.0) != 10.0);
    TEST_ASSERT_TRUE(FixedPointQ7_8(10.0) != 10.1);
}

void test_FixedPointQ7_8_operatorLessThan()
{
    TEST_ASSERT_TRUE(FixedPointQ7_8(10.0) < 11.0);
    TEST_ASSERT_FALSE(FixedPointQ7_8(10.0) < 9.0);
}

void test_FixedPointQ7_8_operatorLessThanOrEqualTo()
{
    TEST_ASSERT_TRUE(FixedPointQ7_8(10.0) <= 11.0);
    TEST_ASSERT_TRUE(FixedPointQ7_8(10.0) <= 10.0);
    TEST_ASSERT_FALSE(FixedPointQ7_8(10.0) < 9.0);
}

void test_FixedPointQ7_8_operatorGreaterThan()
{
    TEST_ASSERT_TRUE(FixedPointQ7_8(11.0) > 10.0);
    TEST_ASSERT_FALSE(FixedPointQ7_8(9.0) > 10.0);
}

void test_FixedPointQ7_8_operatorGreaterThanOrEqualTo()
{
    TEST_ASSERT_TRUE(FixedPointQ7_8(11.0) >= 10.0);
    TEST_ASSERT_TRUE(FixedPointQ7_8(10.0) >= 10.0);
    TEST_ASSERT_FALSE(FixedPointQ7_8(9.0) >= 10.0);
}


void test_FixedPointQ15_16_multiplyAccumulate()
{
    FixedPointQ15_16 testee1(10.0);
    testee1.multiplyAccumulate(0.5, 20.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(testee1, 20.0);

    FixedPointQ15_16 testee2(-10.0);
    testee2.multiplyAccumulate(0.5, 20.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(testee2, 0.0);

    FixedPointQ15_16 testee3(10.0);
    testee3.multiplyAccumulate(-0.5, 20.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(testee3, 0.0);

    FixedPointQ15_16 testee4(10.0);
    testee4.multiplyAccumulate(0.5, -10.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(testee4, 5.0);
}


void test_FixedPointQ15_16_multiplySubtract()
{
    FixedPointQ15_16 testee1(1.0);
    testee1.multiplySubtract(0.5, 2.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(testee1, 0.0);

    FixedPointQ15_16 testee2(-1.0);
    testee2.multiplySubtract(0.5, 2.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(testee2, -2.0);

    FixedPointQ15_16 testee3(1.0);
    testee3.multiplySubtract(-0.5, 2.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(testee3, 2.0);

    FixedPointQ15_16 testee4(1.0);
    testee4.multiplySubtract(0.5, -1.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(testee4, 1.5);
}

void test_FixedPointQ15_16_operatorAddAssignation()
{
    FixedPointQ15_16 testee1(10.0);
    testee1 += 1.25;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee1, 11.25);

    FixedPointQ15_16 testee2(-10.0);
    testee2 += 1.75;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee2, -8.25);

    FixedPointQ15_16 testee3(10.0);
    testee3 += -1.5;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee3, 8.5);

    FixedPointQ15_16 testee4(-10.0);
    testee4 += -1.625;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee4, -11.625);
}

void test_FixedPointQ15_16_operatorSubtractAssignation()
{
    FixedPointQ15_16 testee1(10.0);
    testee1 -= 1.25;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee1, 8.75);

    FixedPointQ15_16 testee2(-10.0);
    testee2 -= 1.75;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee2, -11.75);

    FixedPointQ15_16 testee3(10.0);
    testee3 -= -1.5;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee3, 11.5);

    FixedPointQ15_16 testee4(-10.0);
    testee4 -= -1.625;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee4, -8.375);
}

void test_FixedPointQ15_16_operatorMultiplyAssignation()
{
    FixedPointQ15_16 testee1(10.0);
    testee1 *= 1.25;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee1, 12.5);

    FixedPointQ15_16 testee2(-10.0);
    testee2 *= 1.75;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee2, -17.5);

    FixedPointQ15_16 testee3(10.0);
    testee3 *= -1.5;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee3, -15.0);

    FixedPointQ15_16 testee4(-10.0);
    testee4 *= -1.625;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee4, 16.25);
}

void test_FixedPointQ15_16_operatorDivideAssignation()
{
    FixedPointQ15_16 testee1(10.0);
    testee1 /= 1.25;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee1, 8.0);

    FixedPointQ15_16 testee2(-10.0);
    testee2 /= 2.5;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee2, -4.0);

    FixedPointQ15_16 testee3(10.0);
    testee3 /= -5.0;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee3, -2.0);

    FixedPointQ15_16 testee4(-10.0);
    testee4 /= -8.0;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee4, 1.25);
}

void test_FixedPointQ15_16_operatorIncrement()
{
    FixedPointQ15_16 testee(10.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(++testee, 11.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(testee++, 11.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(testee, 12.0);
}

void test_FixedPointQ15_16_operatorDecrement()
{
    FixedPointQ15_16 testee(10.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(--testee, 9.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(testee--, 9.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(testee, 8.0);
}

void test_FixedPointQ15_16_operatorUnaryMinus()
{
    TEST_ASSERT_EQUAL_FIXED_POINT(-FixedPointQ15_16(10.0), -10.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(-FixedPointQ15_16(0.0), 0.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(-FixedPointQ15_16(-10.0), 10.0);
}

void test_FixedPointQ15_16_operatorAdd()
{
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ15_16(10.0) + 1.25, 11.25);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ15_16(-10.0) + 1.75, -8.25);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ15_16(10.0) + -1.5, 8.5);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ15_16(-10.0) + -1.625, -11.625);
}

void test_FixedPointQ15_16_operatorSubtract()
{
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ15_16(10.0) - 1.25, 8.75);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ15_16(-10.0) - 1.75, -11.75);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ15_16(10.0) - -1.5, 11.5);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ15_16(-10.0) - -1.625, -8.375);
}

void test_FixedPointQ15_16_operatorMultiply()
{
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ15_16(10.0) * 1.25, 12.5);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ15_16(-10.0) * 1.75, -17.5);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ15_16(10.0) * -1.5, -15.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ15_16(-10.0) * -1.625, 16.25);
}

void test_FixedPointQ15_16_operatorDivide()
{
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ15_16(10.0) / 1.25, 8.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ15_16(-10.0) / 2.5, -4.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ15_16(10.0) / -5.0, -2.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ15_16(-10.0) / -8.0, 1.25);
}

void test_FixedPointQ15_16_operatorEqualTo()
{
    TEST_ASSERT_TRUE(FixedPointQ15_16(10.0) == 10.0);
    TEST_ASSERT_FALSE(FixedPointQ15_16(10.0) == 10.1);
}

void test_FixedPointQ15_16_operatorNotEqualTo()
{
    TEST_ASSERT_FALSE(FixedPointQ15_16(10.0) != 10.0);
    TEST_ASSERT_TRUE(FixedPointQ15_16(10.0) != 10.1);
}

void test_FixedPointQ15_16_operatorLessThan()
{
    TEST_ASSERT_TRUE(FixedPointQ15_16(10.0) < 11.0);
    TEST_ASSERT_FALSE(FixedPointQ15_16(10.0) < 9.0);
}

void test_FixedPointQ15_16_operatorLessThanOrEqualTo()
{
    TEST_ASSERT_TRUE(FixedPointQ15_16(10.0) <= 11.0);
    TEST_ASSERT_TRUE(FixedPointQ15_16(10.0) <= 10.0);
    TEST_ASSERT_FALSE(FixedPointQ15_16(10.0) < 9.0);
}

void test_FixedPointQ15_16_operatorGreaterThan()
{
    TEST_ASSERT_TRUE(FixedPointQ15_16(11.0) > 10.0);
    TEST_ASSERT_FALSE(FixedPointQ15_16(9.0) > 10.0);
}

void test_FixedPointQ15_16_operatorGreaterThanOrEqualTo()
{
    TEST_ASSERT_TRUE(FixedPointQ15_16(11.0) >= 10.0);
    TEST_ASSERT_TRUE(FixedPointQ15_16(10.0) >= 10.0);
    TEST_ASSERT_FALSE(FixedPointQ15_16(9.0) >= 10.0);
}

void test_FixedPointQ31_32_operatorAddAssignation()
{
    FixedPointQ31_32 testee1(10.0);
    testee1 += 1.25;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee1, 11.25);

    FixedPointQ31_32 testee2(-10.0);
    testee2 += 1.75;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee2, -8.25);

    FixedPointQ31_32 testee3(10.0);
    testee3 += -1.5;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee3, 8.5);

    FixedPointQ31_32 testee4(-10.0);
    testee4 += -1.625;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee4, -11.625);
}

void test_FixedPointQ31_32_operatorSubtractAssignation()
{
    FixedPointQ31_32 testee1(10.0);
    testee1 -= 1.25;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee1, 8.75);

    FixedPointQ31_32 testee2(-10.0);
    testee2 -= 1.75;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee2, -11.75);

    FixedPointQ31_32 testee3(10.0);
    testee3 -= -1.5;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee3, 11.5);

    FixedPointQ31_32 testee4(-10.0);
    testee4 -= -1.625;
    TEST_ASSERT_EQUAL_FIXED_POINT(testee4, -8.375);
}

void test_FixedPointQ31_32_operatorIncrement()
{
    FixedPointQ31_32 testee(10.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(++testee, 11.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(testee++, 11.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(testee, 12.0);
}

void test_FixedPointQ31_32_operatorDecrement()
{
    FixedPointQ31_32 testee(10.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(--testee, 9.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(testee--, 9.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(testee, 8.0);
}

void test_FixedPointQ31_32_operatorUnaryMinus()
{
    TEST_ASSERT_EQUAL_FIXED_POINT(-FixedPointQ31_32(10.0), -10.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(-FixedPointQ31_32(0.0), 0.0);
    TEST_ASSERT_EQUAL_FIXED_POINT(-FixedPointQ31_32(-10.0), 10.0);
}

void test_FixedPointQ31_32_operatorAdd()
{
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ31_32(10.0) + 1.25, 11.25);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ31_32(-10.0) + 1.75, -8.25);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ31_32(10.0) + -1.5, 8.5);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ31_32(-10.0) + -1.625, -11.625);
}

void test_FixedPointQ31_32_operatorSubtract()
{
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ31_32(10.0) - 1.25, 8.75);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ31_32(-10.0) - 1.75, -11.75);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ31_32(10.0) - -1.5, 11.5);
    TEST_ASSERT_EQUAL_FIXED_POINT(FixedPointQ31_32(-10.0) - -1.625, -8.375);
}

void test_FixedPointQ31_32_operatorEqualTo()
{
    TEST_ASSERT_TRUE(FixedPointQ31_32(10.0) == 10.0);
    TEST_ASSERT_FALSE(FixedPointQ31_32(10.0) == 10.1);
}

void test_FixedPointQ31_32_operatorNotEqualTo()
{
    TEST_ASSERT_FALSE(FixedPointQ31_32(10.0) != 10.0);
    TEST_ASSERT_TRUE(FixedPointQ31_32(10.0) != 10.1);
}

void test_FixedPointQ31_32_operatorLessThan()
{
    TEST_ASSERT_TRUE(FixedPointQ31_32(10.0) < 11.0);
    TEST_ASSERT_FALSE(FixedPointQ31_32(10.0) < 9.0);
}

void test_FixedPointQ31_32_operatorLessThanOrEqualTo()
{
    TEST_ASSERT_TRUE(FixedPointQ31_32(10.0) <= 11.0);
    TEST_ASSERT_TRUE(FixedPointQ31_32(10.0) <= 10.0);
    TEST_ASSERT_FALSE(FixedPointQ31_32(10.0) < 9.0);
}

void test_FixedPointQ31_32_operatorGreaterThan()
{
    TEST_ASSERT_TRUE(FixedPointQ31_32(11.0) > 10.0);
    TEST_ASSERT_FALSE(FixedPointQ31_32(9.0) > 10.0);
}

void test_FixedPointQ31_32_operatorGreaterThanOrEqualTo()
{
    TEST_ASSERT_TRUE(FixedPointQ31_32(11.0) >= 10.0);
    TEST_ASSERT_TRUE(FixedPointQ31_32(10.0) >= 10.0);
    TEST_ASSERT_FALSE(FixedPointQ31_32(9.0) >= 10.0);
}
