#ifndef CNC_MATH_Z_ROTATION_H
#define CNC_MATH_Z_ROTATION_H

#include <cnc/math/Vector3.h>
#include <cnc/utils/TypeTraits.h>

#include <cmath>

template<class T>
inline T degToRad(T v)
{
    return v * static_cast<T>(M_PI) / 180;
}

template<class T>
inline T radToDeg(T v)
{
    return v * 180 / static_cast<T>(M_PI);
}

template<class T>
class ZRotation
{
    T m_m00;
    T m_m01;
    T m_m10;
    T m_m11;

public:
    ZRotation(T angleRad);

    void setAngleRad(T angleRad);

    Vector3<T> rotate(const Vector3<T>& v);
};

template<class T>
ZRotation<T>::ZRotation(T angleRad)
{
    setAngleRad(angleRad);
}

template<class T>
inline void ZRotation<T>::setAngleRad(T angleRad)
{
    static_assert(AlwaysFalse<T>::value, "Not supported type");
}

template<>
inline void ZRotation<float>::setAngleRad(float angleRad)
{
    m_m00 = std::cos(angleRad);
    m_m11 = m_m00;
    m_m10 = std::sin(angleRad);
    m_m01 = -m_m10;
}

template<>
inline void ZRotation<double>::setAngleRad(double angleRad)
{
    m_m00 = std::cos(angleRad);
    m_m11 = m_m00;
    m_m10 = std::sin(angleRad);
    m_m01 = -m_m10;
}

template<class T>
inline Vector3<T> ZRotation<T>::rotate(const Vector3<T>& v)
{
    return Vector3<T>(m_m00 * v.x + m_m01 * v.y, m_m10 * v.x + m_m11 * v.y, v.z);
}

#endif
