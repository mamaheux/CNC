#include <cnc/math/ZRotation.h>
#include <unity.h>

void test_degToRad() {
  TEST_ASSERT_EQUAL(degToRad(90.f), M_PI / 2);
  TEST_ASSERT_EQUAL(degToRad(45.f), M_PI / 4);
}

void test_radToDeg() {
  TEST_ASSERT_EQUAL(radToDeg(M_PI / 2), 90.f);
  TEST_ASSERT_EQUAL(radToDeg(M_PI / 4), 45.f);
}

void test_ZRotation_rotate() {
  ZRotation<float> rotation(0);
  Vector3<float> a(1.f, 0.f, 2.f);

  Vector3<float> r1 = rotation.rotate(a);

  rotation.setAngleRad(M_PI);
  Vector3<float> r2 = rotation.rotate(a);

  rotation.setAngleRad(M_PI / 2);
  Vector3<float> r3 = rotation.rotate(a);

  TEST_ASSERT_EQUAL(r1.x, 1.f);
  TEST_ASSERT_EQUAL(r1.y, 0.f);
  TEST_ASSERT_EQUAL(r1.z, 2.f);

  TEST_ASSERT_EQUAL(r2.x, -1.f);
  TEST_ASSERT_EQUAL(r2.y, 0.f);
  TEST_ASSERT_EQUAL(r2.z, 2.f);

  TEST_ASSERT_EQUAL(r3.x, 0.f);
  TEST_ASSERT_EQUAL(r3.y, 1.f);
  TEST_ASSERT_EQUAL(r3.z, 2.f);
}
