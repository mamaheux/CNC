#include <cnc/math/ZRotation.h>
#include <unity.h>

constexpr float MAX_DELTA = 1e-7;

void test_degToRad() {
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, M_PI / 2, degToRad(90.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, M_PI / 4, degToRad(45.f));
}

void test_radToDeg() {
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 90.f, radToDeg(M_PI / 2));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 45.f, radToDeg(M_PI / 4));
}

void test_ZRotation_rotate() {
  ZRotation<float> rotation(0);
  Vector3<float> a(1.f, 0.f, 2.f);

  Vector3<float> r1 = rotation.rotate(a);

  rotation.setAngleRad(M_PI);
  Vector3<float> r2 = rotation.rotate(a);

  rotation.setAngleRad(M_PI / 2);
  Vector3<float> r3 = rotation.rotate(a);

  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, r1.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.f, r1.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r1.z);

  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, -1.f, r2.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.f, r2.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r2.z);

  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.f, r3.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, r3.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r3.z);
}
