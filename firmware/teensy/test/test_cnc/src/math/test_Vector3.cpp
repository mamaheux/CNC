#include <cnc/math/Vector3.h>
#include <unity.h>

constexpr float MAX_DELTA = 1e-7;

void test_Vector3_constructor() {
  Vector3<float> a;
  Vector3<float> b(1.f, 2.f, 3.f);

  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.f, a.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.f, a.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.f, a.z);

  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, b.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, b.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 3.f, b.z);
}

void test_Vector3_dot() {
  Vector3<float> a(3.f, 4.f, 5.f);
  Vector3<float> b(1.f, 2.f, 3.f);

  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 26.f, a.dot(b));
}

void test_Vector3_add() {
  Vector3<float> a(3.f, -4.f, 5.f);
  Vector3<float> b(-1.f, 2.f, 4.f);
  Vector3<float> r = a + b;

  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, -2.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 9.f, r.z);
}

void test_Vector3_addAssign() {
  Vector3<float> a(3.f, -4.f, 5.f);
  Vector3<float> b(-1.f, 2.f, 4.f);
  a += b;

  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, a.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, -2.f, a.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 9.f, a.z);
}

void test_Vector3_substract() {
  Vector3<float> a(3.f, -4.f, 5.f);
  Vector3<float> b(-1.f, 2.f, 4.f);
  Vector3<float> r = a - b;

  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 4.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, -6.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, r.z);
}

void test_Vector3_substractAssign() {
  Vector3<float> a(3.f, -4.f, 5.f);
  Vector3<float> b(-1.f, 2.f, 4.f);
  a -= b;

  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 4.f, a.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, -6.f, a.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, a.z);
}

void test_Vector3_multiply() {
  Vector3<float> a(1.f, 2.f, 3.f);
  Vector3<float> r1 = a * 2;
  Vector3<float> r2 = 2 * a;

  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r1.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 4.f, r1.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 6.f, r1.z);

  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r2.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 4.f, r2.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 6.f, r2.z);
}

void test_Vector3_multiplyAssign() {
  Vector3<float> a(1.f, 2.f, 3.f);
  a *= 2;

  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, a.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 4.f, a.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 6.f, a.z);
}

void test_Vector3_divide() {
  Vector3<float> a(2.f, 4.f, 8.f);
  Vector3<float> r1 = a / 2;
  Vector3<float> r2 = 1 / a;

  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, r1.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r1.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 4.f, r1.z);

  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.5f, r2.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.25f, r2.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.125f, r2.z);
}

void test_Vector3_divideAssign() {
  Vector3<float> a(2.f, 4.f, 8.f);
  a /= 2;

  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, a.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, a.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 4.f, a.z);
}
