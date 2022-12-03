#ifndef CNC_UTILS_MACRO_H
#define CNC_UTILS_MACRO_H

#include <cstring>

#define __FUNCTION_NAME__ __PRETTY_FUNCTION__
#define __FILENAME__      (std::strrchr(__FILE__, '/') ? std::strrchr(__FILE__, '/') + 1 : __FILE__)

#endif
