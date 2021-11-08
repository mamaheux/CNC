#include <cnc/math/Vector3.h>
#include <unity.h>

void test_Vector3_constructor() {
  Vector3<float> a;
  Vector3<float> b(1.f, 2.f, 3.f);

  TEST_ASSERT_EQUAL(a.x, 0.f);
  TEST_ASSERT_EQUAL(a.y, 0.f);
  TEST_ASSERT_EQUAL(a.z, 0.f);

  TEST_ASSERT_EQUAL(b.x, 1.f);
  TEST_ASSERT_EQUAL(b.y, 2.f);
  TEST_ASSERT_EQUAL(b.z, 3.f);
}

void test_Vector3_dot() {
  Vector3<float> a(3.f, 4.f, 5.f);
  Vector3<float> b(1.f, 2.f, 3.f);

  TEST_ASSERT_EQUAL(a.dot(b), 26.f);
}

void test_Vector3_add() {
  Vector3<float> a(3.f, -4.f, 5.f);
  Vector3<float> b(-1.f, 2.f, 4.f);
  Vector3<float> r = a + b;

  TEST_ASSERT_EQUAL(r.x, 2.f);
  TEST_ASSERT_EQUAL(r.y, -2.f);
  TEST_ASSERT_EQUAL(r.z, 9.f);
}

void test_Vector3_addAssign() {
  Vector3<float> a(3.f, -4.f, 5.f);
  Vector3<float> b(-1.f, 2.f, 4.f);
  a += b;

  TEST_ASSERT_EQUAL(a.x, 2.f);
  TEST_ASSERT_EQUAL(a.y, -2.f);
  TEST_ASSERT_EQUAL(a.z, 9.f);
}

void test_Vector3_substract() {
  Vector3<float> a(3.f, -4.f, 5.f);
  Vector3<float> b(-1.f, 2.f, 4.f);
  Vector3<float> r = a - b;

  TEST_ASSERT_EQUAL(r.x, 4.f);
  TEST_ASSERT_EQUAL(r.y, -6.f);
  TEST_ASSERT_EQUAL(r.z, 1.f);
}

void test_Vector3_substractAssign() {
  Vector3<float> a(3.f, -4.f, 5.f);
  Vector3<float> b(-1.f, 2.f, 4.f);
  a -= b;

  TEST_ASSERT_EQUAL(a.x, 4.f);
  TEST_ASSERT_EQUAL(a.y, -6.f);
  TEST_ASSERT_EQUAL(a.z, 1.f);
}

void test_Vector3_multiply() {
  Vector3<float> a(1.f, 2.f, 3.f);
  Vector3<float> r1 = a * 2;
  Vector3<float> r2 = 2 * a;

  TEST_ASSERT_EQUAL(r1.x, 2.f);
  TEST_ASSERT_EQUAL(r1.y, 4.f);
  TEST_ASSERT_EQUAL(r1.z, 6.f);

  TEST_ASSERT_EQUAL(r2.x, 2.f);
  TEST_ASSERT_EQUAL(r2.y, 4.f);
  TEST_ASSERT_EQUAL(r2.z, 6.f);
}

void test_Vector3_multiplyAssign() {
  Vector3<float> a(1.f, 2.f, 3.f);
  a *= 2;

  TEST_ASSERT_EQUAL(a.x, 2.f);
  TEST_ASSERT_EQUAL(a.y, 4.f);
  TEST_ASSERT_EQUAL(a.z, 6.f);
}

void test_Vector3_divide() {
  Vector3<float> a(2.f, 4.f, 8.f);
  Vector3<float> r1 = a / 2;
  Vector3<float> r2 = 1 / a;

  TEST_ASSERT_EQUAL(r1.x, 1.f);
  TEST_ASSERT_EQUAL(r1.y, 2.f);
  TEST_ASSERT_EQUAL(r1.z, 4.f);

  TEST_ASSERT_EQUAL(r2.x, 0.5f);
  TEST_ASSERT_EQUAL(r2.y, 0.25f);
  TEST_ASSERT_EQUAL(r2.z, 0.125f);
}

void test_Vector3_divideAssign() {
  Vector3<float> a(2.f, 4.f, 8.f);
  a /= 2;

  TEST_ASSERT_EQUAL(a.x, 1.f);
  TEST_ASSERT_EQUAL(a.y, 2.f);
  TEST_ASSERT_EQUAL(a.z, 4.f);
}
