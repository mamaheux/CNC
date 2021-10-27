#ifndef CNC_RESOURCES_CNC_RESOURCES_H
#define CNC_RESOURCES_CNC_RESOURCES_H

#include <ResourceLock.h>

#include <cstdint>

constexpr uint8_t PIN_COUNT = 42;

constexpr bool HAS_PWM_BY_PIN[PIN_COUNT] = {
  true, // 0
  true, // 1
  true, // 2
  true, // 3
  true, // 4
  true, // 5
  true, // 6
  true, // 7
  true, // 8
  true, // 9
  true, // 10
  true, // 11
  true, // 12
  true, // 13
  true, // 14
  true, // 15
  false, // 16
  false, // 17
  true, // 18
  true, // 19
  false, // 20
  false, // 21
  true, // 22
  true, // 23
  true, // 24
  true, // 25
  false, // 26
  false, // 27
  true, // 28
  true, // 29
  false, // 30
  false, // 31
  false, // 32
  true, // 33
  false, // 34
  false, // 35
  true, // 36
  true, // 37
  false, // 38
  false, // 39
  false, // 40
  false, // 41
};

constexpr uint8_t PWM_GROUP_COUNT = 20;
constexpr uint8_t INVALID_PWM_GROUP = 255;
constexpr uint8_t PWM_GROUP_BY_PIN[PIN_COUNT] = {
  1, // 0
  0, // 1
  11, // 2
  11, // 3
  4, // 4
  5, // 5
  6, // 6
  3, // 7
  3, // 8
  6, // 9
  12, // 10
  14, // 11
  13, // 12
  15, // 13
  18, // 14
  19, // 15
  INVALID_PWM_GROUP, // 16
  INVALID_PWM_GROUP, // 17
  17, // 18
  16, // 19
  INVALID_PWM_GROUP, // 20
  INVALID_PWM_GROUP, // 21
  9, // 22
  10, // 23
  2, // 24
  3, // 25
  INVALID_PWM_GROUP, // 26
  INVALID_PWM_GROUP, // 27
  8, // 28
  8, // 29
  INVALID_PWM_GROUP, // 30
  INVALID_PWM_GROUP, // 31
  INVALID_PWM_GROUP, // 32
  4, // 33
  INVALID_PWM_GROUP, // 34
  INVALID_PWM_GROUP, // 35
  7, // 36
  7, // 37
  INVALID_PWM_GROUP, // 38
  INVALID_PWM_GROUP, // 39
  INVALID_PWM_GROUP, // 40
  INVALID_PWM_GROUP, // 41
};

constexpr bool HAS_ADC_BY_PIN[PIN_COUNT] = {
  false, // 0
  false, // 1
  false, // 2
  false, // 3
  false, // 4
  false, // 5
  false, // 6
  false, // 7
  false, // 8
  false, // 9
  false, // 10
  false, // 11
  false, // 12
  false, // 13
  true, // 14
  true, // 15
  true, // 16
  true, // 17
  true, // 18
  true, // 19
  true, // 20
  true, // 21
  true, // 22
  true, // 23
  true, // 24
  true, // 25
  true, // 26
  true, // 27
  false, // 28
  false, // 29
  false, // 30
  false, // 31
  false, // 32
  false, // 33
  false, // 34
  false, // 35
  false, // 36
  false, // 37
  true, // 38
  true, // 39
  true, // 40
  true, // 41
};

#endif
