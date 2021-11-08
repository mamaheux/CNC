#ifndef CNC_UTILS_TYPE_TRAITS_H
#define CNC_UTILS_TYPE_TRAITS_H

#include <type_traits>

template<class T>
struct AlwaysFalse : public std::false_type {
};

#endif