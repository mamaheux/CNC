#include <cnc/math/Range.h>
#include <unity.h>

void test_InclusiveRange3()
{
    InclusiveRange3<int> range(Vector3<int>(0, 1, 2), Vector3<int>(3, 4, 5));

    TEST_ASSERT_FALSE(range.contains(Vector3<int>(-1, 1, 2)));
    TEST_ASSERT_FALSE(range.contains(Vector3<int>(0, 0, 2)));
    TEST_ASSERT_FALSE(range.contains(Vector3<int>(0, 1, 1)));
    TEST_ASSERT_FALSE(range.contains(Vector3<int>(4, 4, 5)));
    TEST_ASSERT_FALSE(range.contains(Vector3<int>(3, 5, 5)));
    TEST_ASSERT_FALSE(range.contains(Vector3<int>(3, 4, 6)));

    TEST_ASSERT_TRUE(range.contains(Vector3<int>(0, 2, 3)));
    TEST_ASSERT_TRUE(range.contains(Vector3<int>(1, 1, 3)));
    TEST_ASSERT_TRUE(range.contains(Vector3<int>(1, 2, 2)));
    TEST_ASSERT_TRUE(range.contains(Vector3<int>(1, 2, 3)));
    TEST_ASSERT_TRUE(range.contains(Vector3<int>(1, 2, 3)));
    TEST_ASSERT_TRUE(range.contains(Vector3<int>(1, 2, 3)));
    TEST_ASSERT_TRUE(range.contains(Vector3<int>(3, 3, 4)));
    TEST_ASSERT_TRUE(range.contains(Vector3<int>(2, 4, 3)));
    TEST_ASSERT_TRUE(range.contains(Vector3<int>(2, 3, 5)));
}
