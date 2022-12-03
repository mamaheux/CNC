#ifndef MCU_ERROR_H
#define MCU_ERROR_H

#include "config.h"

#include <cnc/utils/macro.h>

#include <cstring>

constexpr uint32_t ERROR_DELAY_MS = 1000;

#define CRITICAL_ERROR_CHECK(code, reason)                                                                             \
    while (!(code))                                                                                                    \
    {                                                                                                                  \
        DEBUG_SERIAL.println((reason));                                                                                \
        DEBUG_SERIAL.print("Function name: ");                                                                         \
        DEBUG_SERIAL.println(__FUNCTION_NAME__);                                                                       \
        DEBUG_SERIAL.print("Filename: ");                                                                              \
        DEBUG_SERIAL.println(__FILENAME__);                                                                            \
        toogleErrorLed();                                                                                              \
        delay(ERROR_DELAY_MS);                                                                                         \
    }

#define CRITICAL_ERROR_CHECK_2(code, reason1, reason2)                                                                 \
    while (!(code))                                                                                                    \
    {                                                                                                                  \
        DEBUG_SERIAL.print((reason1));                                                                                 \
        DEBUG_SERIAL.println((reason2));                                                                               \
        DEBUG_SERIAL.print("Function name: ");                                                                         \
        DEBUG_SERIAL.println(__FUNCTION_NAME__);                                                                       \
        DEBUG_SERIAL.print("Filename: ");                                                                              \
        DEBUG_SERIAL.println(__FILENAME__);                                                                            \
        toogleErrorLed();                                                                                              \
        delay(ERROR_DELAY_MS);                                                                                         \
    }

#define CRITICAL_ERROR_CHECK_3(code, reason1, reason2, reason3)                                                        \
    while (!(code))                                                                                                    \
    {                                                                                                                  \
        DEBUG_SERIAL.print((reason1));                                                                                 \
        DEBUG_SERIAL.print((reason2));                                                                                 \
        DEBUG_SERIAL.println((reason3));                                                                               \
        DEBUG_SERIAL.print("Function name: ");                                                                         \
        DEBUG_SERIAL.println(__FUNCTION_NAME__);                                                                       \
        DEBUG_SERIAL.print("Filename: ");                                                                              \
        DEBUG_SERIAL.println(__FILENAME__);                                                                            \
        toogleErrorLed();                                                                                              \
        delay(ERROR_DELAY_MS);                                                                                         \
    }

#define CRITICAL_ERROR_CHECK_4(code, reason1, reason2, reason3, reason4)                                               \
    while (!(code))                                                                                                    \
    {                                                                                                                  \
        DEBUG_SERIAL.print((reason1));                                                                                 \
        DEBUG_SERIAL.print((reason2));                                                                                 \
        DEBUG_SERIAL.print((reason3));                                                                                 \
        DEBUG_SERIAL.println((reason4));                                                                               \
        DEBUG_SERIAL.print("Function name: ");                                                                         \
        DEBUG_SERIAL.println(__FUNCTION_NAME__);                                                                       \
        DEBUG_SERIAL.print("Filename: ");                                                                              \
        DEBUG_SERIAL.println(__FILENAME__);                                                                            \
        toogleErrorLed();                                                                                              \
        delay(ERROR_DELAY_MS);                                                                                         \
    }

#define CRITICAL_ERROR_CHECK_5(code, reason1, reason2, reason3, reason4, reason5)                                      \
    while (!(code))                                                                                                    \
    {                                                                                                                  \
        DEBUG_SERIAL.print((reason1));                                                                                 \
        DEBUG_SERIAL.print((reason2));                                                                                 \
        DEBUG_SERIAL.print((reason3));                                                                                 \
        DEBUG_SERIAL.print((reason4));                                                                                 \
        DEBUG_SERIAL.println((reason5));                                                                               \
        DEBUG_SERIAL.print("Function name: ");                                                                         \
        DEBUG_SERIAL.println(__FUNCTION_NAME__);                                                                       \
        DEBUG_SERIAL.print("Filename: ");                                                                              \
        DEBUG_SERIAL.println(__FILENAME__);                                                                            \
        toogleErrorLed();                                                                                              \
        delay(ERROR_DELAY_MS);                                                                                         \
    }

#define ON_CRITICAL_ERROR(reason1, functionName, filename)                                                             \
    while (true)                                                                                                       \
    {                                                                                                                  \
        DEBUG_SERIAL.print((reason1));                                                                                 \
        DEBUG_SERIAL.print("Function name: ");                                                                         \
        DEBUG_SERIAL.println((functionName));                                                                          \
        DEBUG_SERIAL.print("Filename: ");                                                                              \
        DEBUG_SERIAL.println((filename));                                                                              \
        toogleErrorLed();                                                                                              \
        delay(ERROR_DELAY_MS);                                                                                         \
    }

#define ON_CRITICAL_ERROR_2(reason1, reason2, functionName, filename)                                                  \
    while (true)                                                                                                       \
    {                                                                                                                  \
        DEBUG_SERIAL.print((reason1));                                                                                 \
        DEBUG_SERIAL.print((reason2));                                                                                 \
        DEBUG_SERIAL.print("Function name: ");                                                                         \
        DEBUG_SERIAL.println((functionName));                                                                          \
        DEBUG_SERIAL.print("Filename: ");                                                                              \
        DEBUG_SERIAL.println((filename));                                                                              \
        toogleErrorLed();                                                                                              \
        delay(ERROR_DELAY_MS);                                                                                         \
    }

#define ON_CRITICAL_ERROR_3(reason1, reason2, reason3, functionName, filename)                                         \
    while (true)                                                                                                       \
    {                                                                                                                  \
        DEBUG_SERIAL.print((reason1));                                                                                 \
        DEBUG_SERIAL.print((reason2));                                                                                 \
        DEBUG_SERIAL.print((reason3));                                                                                 \
        DEBUG_SERIAL.print("Function name: ");                                                                         \
        DEBUG_SERIAL.println((functionName));                                                                          \
        DEBUG_SERIAL.print("Filename: ");                                                                              \
        DEBUG_SERIAL.println((filename));                                                                              \
        toogleErrorLed();                                                                                              \
        delay(ERROR_DELAY_MS);                                                                                         \
    }

void setupCriticalErrorCheck();
void toogleErrorLed();

#endif
