#include <cnc/resources/ResourceLock.h>
#include <unity.h>

void test_PinLock_isNotRecursive()
{
    {
        PinLock lock;
        TEST_ASSERT_TRUE(lock.tryLock(10));
        TEST_ASSERT_FALSE(lock.tryLock(10));
    }
    PinLock lock;
    TEST_ASSERT_TRUE(lock.tryLock(10));
}

void test_PinLock_samePin()
{
    PinLock lock1;
    PinLock lock2;
    PinLock lock3;
    TEST_ASSERT_TRUE(lock1.tryLock(10));
    TEST_ASSERT_FALSE(lock2.tryLock(10));
    TEST_ASSERT_TRUE(lock3.tryLock(11));
}

void test_PwmLock_invalidPin()
{
    PwmLock lock;
    TEST_ASSERT_FALSE(lock.tryLock(26, 10000));
}

void test_PwmLock_isNotRecursive()
{
    {
        PwmLock lock;
        TEST_ASSERT_TRUE(lock.tryLock(28, 10000));
        TEST_ASSERT_FALSE(lock.tryLock(28, 10000));
    }
    PwmLock lock;
    TEST_ASSERT_TRUE(lock.tryLock(28, 10000));
}

void test_PwmLock_sameGroup()
{
    {
        PwmLock lock1;
        PwmLock lock2;
        PwmLock lock3;
        TEST_ASSERT_TRUE(lock1.tryLock(28, 10000));
        TEST_ASSERT_FALSE(lock2.tryLock(29, 11000));
        TEST_ASSERT_TRUE(lock3.tryLock(29, 10000));
    }
    PwmLock lock;
    TEST_ASSERT_TRUE(lock.tryLock(29, 11000));
}
