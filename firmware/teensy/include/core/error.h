#ifndef CORE_ERROR_H
#define CORE_ERROR_H

#include "config.h"

#include <cstring>

#define __FUNCTION_NAME__ __PRETTY_FUNCTION__
#define __FILENAME__ (std::strrchr(__FILE__, '/') ? std::strrchr(__FILE__, '/') + 1 : __FILE__)
constexpr uint32_t ERROR_DELAY_MS = 10000;

#define ERROR_CHECK(code, reason) \
  while (!(code)) { \
    DEBUG_SERIAL.println((reason)); \
    DEBUG_SERIAL.print("Function name: "); \
    DEBUG_SERIAL.println(__FUNCTION_NAME__); \
    DEBUG_SERIAL.print("Filename: "); \
    DEBUG_SERIAL.println(__FILENAME__); \
    delay(ERROR_DELAY_MS); \
  }

#define ERROR_CHECK_2(code, reason1, reason2) \
  while (!(code)) { \
    DEBUG_SERIAL.print((reason1)); \
    DEBUG_SERIAL.println((reason2)); \
    DEBUG_SERIAL.print("Function name: "); \
    DEBUG_SERIAL.println(__FUNCTION_NAME__); \
    DEBUG_SERIAL.print("Filename: "); \
    DEBUG_SERIAL.println(__FILENAME__); \
    delay(ERROR_DELAY_MS); \
  }

#define ERROR_CHECK_3(code, reason1, reason2, reason3) \
  while (!(code)) { \
    DEBUG_SERIAL.print((reason1)); \
    DEBUG_SERIAL.print((reason2)); \
    DEBUG_SERIAL.println((reason3)); \
    DEBUG_SERIAL.print("Function name: "); \
    DEBUG_SERIAL.println(__FUNCTION_NAME__); \
    DEBUG_SERIAL.print("Filename: "); \
    DEBUG_SERIAL.println(__FILENAME__); \
    delay(ERROR_DELAY_MS); \
  }

#define ERROR_CHECK_4(code, reason1, reason2, reason3, reason4) \
  while (!(code)) { \
    DEBUG_SERIAL.print((reason1)); \
    DEBUG_SERIAL.print((reason2)); \
    DEBUG_SERIAL.print((reason3)); \
    DEBUG_SERIAL.println((reason4)); \
    DEBUG_SERIAL.print("Function name: "); \
    DEBUG_SERIAL.println(__FUNCTION_NAME__); \
    DEBUG_SERIAL.print("Filename: "); \
    DEBUG_SERIAL.println(__FILENAME__); \
    delay(ERROR_DELAY_MS); \
  }

#endif
