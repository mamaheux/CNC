#ifndef TEST_RESOURCE_LOCK_H
#define TEST_RESOURCE_LOCK_H

void test_PinLock_isNotRecursive();
void test_PinLock_samePin();

void test_PwmLock_invalidPin();
void test_PwmLock_isNotRecursive();
void test_PwmLock_sameGroup();

#endif
