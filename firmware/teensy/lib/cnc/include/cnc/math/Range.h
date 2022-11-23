#ifndef CNC_MATH_RANGE_H
#define CNC_MATH_RANGE_H

#include <cnc/math/Vector3.h>

template <class T>
class InclusiveRange3 {
  Vector3<T> m_min;
  Vector3<T> m_max;

public:
  InclusiveRange3(const Vector3<T>& min, const Vector3<T>& max);
  bool contains(const Vector3<T>& v);
};

template <class T>
inline InclusiveRange3<T>::InclusiveRange3(const Vector3<T>& min, const Vector3<T>& max) :
    m_min(min), m_max(max) {

}

template <class T>
inline bool InclusiveRange3<T>::contains(const Vector3<T>& v) {
  return m_min.x <= v.x && m_min.y <= v.y && m_min.z <= v.z &&
      v.x <= m_max.x && v.y <= m_max.y && v.z <= m_max.z;
}

#endif
