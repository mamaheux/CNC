#include <cnc/modules/ArcConverter.h>
#include <unity.h>

constexpr float MAX_DELTA = 1e-6;

void test_getCenterPointFromRadius() {
  Vector2<double> center = getCenterPointFromRadius(0.0, 0.0, 1.0, 1.0, 1.0, true);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, center.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.f, center.y);

  center = getCenterPointFromRadius(1.0, 1.0, 0.0, 0.0, 1.0, true);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.f, center.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, center.y);

  center = getCenterPointFromRadius(0.0, 0.0, 1.0, 1.0, 1.0, false);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.f, center.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, center.y);

  center = getCenterPointFromRadius(1.0, 1.0, 0.0, 0.0, 1.0, false);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, center.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.f, center.y);

  center = getCenterPointFromRadius(1.0, 1.0, 2.0, 2.0, 0.707107, false);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.5f, center.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.5f, center.y);

  center = getCenterPointFromRadius(1.0, 1.0, 0.0, 0.0, 0.25, false);
  TEST_ASSERT_FLOAT_IS_NAN(center.x);
  TEST_ASSERT_FLOAT_IS_NAN(center.y);
}
