#include <cnc/math/Vector2.h>
#include <unity.h>

constexpr float MAX_DELTA = 1e-7;

void test_Vector2_constructor() {
  Vector2<float> a;
  Vector2<float> b(1.f, 2.f);

  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.f, a.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.f, a.y);

  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, b.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, b.y);
}

void test_Vector2_dot() {
  Vector2<float> a(3.f, 4.f);
  Vector2<float> b(1.f, 2.f);

  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 11.f, a.dot(b));
}

void test_Vector2_angle() {
  Vector2<float> a(1.f, 0.f);
  Vector2<float> b(0.f, 1.f);

  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, M_PI / 2, a.angle(b));
}

void test_Vector2_norm() {
  Vector2<float> a(3.f, 4.f);

  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 5.f, a.norm());
}

void test_Vector2_normalize() {
  Vector2<float> a(3.f, 4.f);
  a.normalize();

  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.6f, a.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.8f, a.y);
}

void test_Vector2_normalized() {
  Vector2<float> a(3.f, 4.f);
  Vector2<float> r = a.normalized();

  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.6f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.8f, r.y);
}

void test_Vector2_add() {
  Vector2<float> a(3.f, -4.f);
  Vector2<float> b(-1.f, 2.f);
  Vector2<float> r = a + b;

  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, -2.f, r.y);
}

void test_Vector2_addAssign() {
  Vector2<float> a(3.f, -4.f);
  Vector2<float> b(-1.f, 2.f);
  a += b;

  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, a.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, -2.f, a.y);
}

void test_Vector2_substract() {
  Vector2<float> a(3.f, -4.f);
  Vector2<float> b(-1.f, 2.f);
  Vector2<float> r = a - b;

  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 4.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, -6.f, r.y);
}

void test_Vector2_substractAssign() {
  Vector2<float> a(3.f, -4.f);
  Vector2<float> b(-1.f, 2.f);
  a -= b;

  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 4.f, a.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, -6.f, a.y);
}

void test_Vector2_multiply() {
  Vector2<float> a(1.f, 2.f);
  Vector2<float> r1 = a * 2;
  Vector2<float> r2 = 2 * a;

  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r1.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 4.f, r1.y);

  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r2.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 4.f, r2.y);
}

void test_Vector2_multiplyAssign() {
  Vector2<float> a(1.f, 2.f);
  a *= 2;

  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, a.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 4.f, a.y);
}

void test_Vector2_divide() {
  Vector2<float> a(2.f, 4.f);
  Vector2<float> r1 = a / 2;
  Vector2<float> r2 = 1 / a;

  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, r1.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r1.y);

  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.5f, r2.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.25f, r2.y);
}

void test_Vector2_divideAssign() {
  Vector2<float> a(2.f, 4.f);
  a /= 2;

  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, a.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, a.y);
}
