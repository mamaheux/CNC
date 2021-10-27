#include <unity.h>

#include "test_ResourceLock.h"

int main(int argc, char **argv) {
  UNITY_BEGIN();

  RUN_TEST(test_PinLock_isNotRecursive);
  RUN_TEST(test_PinLock_samePin);
  RUN_TEST(test_PwmLock_invalidPin);
  RUN_TEST(test_PwmLock_isNotRecursive);
  RUN_TEST(test_PwmLock_sameGroup);

  UNITY_END();

  return 0;
}
