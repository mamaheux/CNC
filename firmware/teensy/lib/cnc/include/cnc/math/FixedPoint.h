#ifndef CNC_MATH_FIXED_POINT_H
#define CNC_MATH_FIXED_POINT_H

#include <cstddef>
#include <cstdint>
#include <cmath>
#include <type_traits>

template<class T, int N, int LeftShift>
struct BitMask
{
    static constexpr T Value = static_cast<T>((1 << (N - 1) | BitMask<T, N - 1, 0>::Value) << LeftShift);
};

template<class T, int LeftShift>
struct BitMask<T, 0, LeftShift>
{
    static constexpr T Value = static_cast<T>(0);
};

template<size_t IntegerSize, size_t FractionSize>
class FixedPoint
{
public:
    typedef typename std::conditional<
        IntegerSize + FractionSize + 1 == sizeof(int8_t) * 8,
        int8_t,
        typename std::conditional<
            IntegerSize + FractionSize + 1 == sizeof(int16_t) * 8,
            int16_t,
            typename std::conditional<
                IntegerSize + FractionSize + 1 == sizeof(int32_t) * 8,
                int32_t,
                typename std::conditional<IntegerSize + FractionSize + 1 == sizeof(int64_t) * 8, int64_t, int64_t>::
                    type>::type>::type>::type T;

    typedef typename std::conditional<
        IntegerSize + FractionSize + 1 == sizeof(int8_t) * 8,
        int16_t,
        typename std::conditional<
            IntegerSize + FractionSize + 1 == sizeof(int16_t) * 8,
            int32_t,
            typename std::conditional<IntegerSize + FractionSize + 1 == sizeof(int32_t) * 8, int64_t, int64_t>::type>::
            type>::type T2X;

    static constexpr FixedPoint<IntegerSize, FractionSize> ZERO = 0.0;
    static constexpr FixedPoint<IntegerSize, FractionSize> ONE = 1.0;

private:
    static constexpr T ONE_FIXED_POINT = static_cast<T>(1) << static_cast<T>(FractionSize);
    static constexpr T FRACTION_MASK = BitMask<T, FractionSize, 0>::Value;

    T m_value;

public:
    FixedPoint();
    explicit constexpr FixedPoint(T fixedPointValue);
    constexpr FixedPoint(float value);
    constexpr FixedPoint(double value);
    FixedPoint(const FixedPoint<IntegerSize, FractionSize>& other);

    FixedPoint<IntegerSize, FractionSize>& multiplyAccumulate(
        const FixedPoint<IntegerSize, FractionSize>& left,
        const FixedPoint<IntegerSize, FractionSize>& right);
    FixedPoint<IntegerSize, FractionSize>& multiplySubtract(
        const FixedPoint<IntegerSize, FractionSize>& left,
        const FixedPoint<IntegerSize, FractionSize>& right);

    FixedPoint<IntegerSize, FractionSize>& operator=(const FixedPoint<IntegerSize, FractionSize>& other);

    FixedPoint<IntegerSize, FractionSize>& operator+=(const FixedPoint<IntegerSize, FractionSize>& other);
    FixedPoint<IntegerSize, FractionSize>& operator-=(const FixedPoint<IntegerSize, FractionSize>& other);
    FixedPoint<IntegerSize, FractionSize>& operator*=(const FixedPoint<IntegerSize, FractionSize>& other);
    FixedPoint<IntegerSize, FractionSize>& operator/=(const FixedPoint<IntegerSize, FractionSize>& other);

    FixedPoint<IntegerSize, FractionSize>& operator++();
    const FixedPoint<IntegerSize, FractionSize> operator++(int dummy);
    FixedPoint<IntegerSize, FractionSize>& operator--();
    const FixedPoint<IntegerSize, FractionSize> operator--(int dummy);

    FixedPoint<IntegerSize, FractionSize> operator-();

    explicit operator float() const;
    explicit operator double() const;

private:
    static T multiplyFixedPoint(T left, T right);
    static T divideFixedPoint(T left, T right);
    static T sign_(T value);
    static T abs_(T value);

public:
    friend inline FixedPoint<IntegerSize, FractionSize>
        operator+(const FixedPoint<IntegerSize, FractionSize>& left, const FixedPoint<IntegerSize, FractionSize>& right)
    {
        return FixedPoint(static_cast<T>(left.m_value + right.m_value));
    }

    friend inline FixedPoint<IntegerSize, FractionSize>
        operator-(const FixedPoint<IntegerSize, FractionSize>& left, const FixedPoint<IntegerSize, FractionSize>& right)
    {
        return FixedPoint<IntegerSize, FractionSize>(static_cast<T>(left.m_value - right.m_value));
    }

    friend inline FixedPoint<IntegerSize, FractionSize>
        operator*(const FixedPoint<IntegerSize, FractionSize>& left, const FixedPoint<IntegerSize, FractionSize>& right)
    {
        return FixedPoint(FixedPoint<IntegerSize, FractionSize>::multiplyFixedPoint(left.m_value, right.m_value));
    }

    friend inline FixedPoint<IntegerSize, FractionSize> operator/(const FixedPoint& left, const FixedPoint& right)
    {
        return FixedPoint<IntegerSize, FractionSize>(
            FixedPoint<IntegerSize, FractionSize>::divideFixedPoint(left.m_value, right.m_value));
    }


    friend inline bool operator==(
        const FixedPoint<IntegerSize, FractionSize>& left,
        const FixedPoint<IntegerSize, FractionSize>& right)
    {
        return left.m_value == right.m_value;
    }

    friend inline bool operator!=(
        const FixedPoint<IntegerSize, FractionSize>& left,
        const FixedPoint<IntegerSize, FractionSize>& right)
    {
        return left.m_value != right.m_value;
    }

    friend inline bool
        operator<(const FixedPoint<IntegerSize, FractionSize>& left, const FixedPoint<IntegerSize, FractionSize>& right)
    {
        return left.m_value < right.m_value;
    }

    friend inline bool operator<=(
        const FixedPoint<IntegerSize, FractionSize>& left,
        const FixedPoint<IntegerSize, FractionSize>& right)
    {
        return left.m_value <= right.m_value;
    }

    friend inline bool
        operator>(const FixedPoint<IntegerSize, FractionSize>& left, const FixedPoint<IntegerSize, FractionSize>& right)
    {
        return left.m_value > right.m_value;
    }

    friend inline bool operator>=(
        const FixedPoint<IntegerSize, FractionSize>& left,
        const FixedPoint<IntegerSize, FractionSize>& right)
    {
        return left.m_value >= right.m_value;
    }
};

template<size_t IntegerSize, size_t FractionSize>
constexpr FixedPoint<IntegerSize, FractionSize> FixedPoint<IntegerSize, FractionSize>::ZERO;

template<size_t IntegerSize, size_t FractionSize>
constexpr FixedPoint<IntegerSize, FractionSize> FixedPoint<IntegerSize, FractionSize>::ONE;

template<size_t IntegerSize, size_t FractionSize>
inline FixedPoint<IntegerSize, FractionSize>::FixedPoint() : m_value(0)
{
    static_assert(IntegerSize + FractionSize + 1 == sizeof(T) * 8, "All bits of the type must be used");
}

template<size_t IntegerSize, size_t FractionSize>
inline constexpr FixedPoint<IntegerSize, FractionSize>::FixedPoint(T fixedPointValue) : m_value(fixedPointValue)
{
    static_assert(IntegerSize + FractionSize + 1 == sizeof(T) * 8, "All bits of the type must be used");
}

template<size_t IntegerSize, size_t FractionSize>
inline constexpr FixedPoint<IntegerSize, FractionSize>::FixedPoint(float value)
    : m_value(std::round(static_cast<T>(value * ONE_FIXED_POINT)))
{
    static_assert(IntegerSize + FractionSize + 1 == sizeof(T) * 8, "All bits of the type must be used");
}

template<size_t IntegerSize, size_t FractionSize>
inline constexpr FixedPoint<IntegerSize, FractionSize>::FixedPoint(double value)
    : m_value(std::round(static_cast<T>(value * ONE_FIXED_POINT)))
{
    static_assert(IntegerSize + FractionSize + 1 == sizeof(T) * 8, "All bits of the type must be used");
}

template<size_t IntegerSize, size_t FractionSize>
inline FixedPoint<IntegerSize, FractionSize>::FixedPoint(const FixedPoint<IntegerSize, FractionSize>& other)
    : m_value(other.m_value)
{
    static_assert(IntegerSize + FractionSize + 1 == sizeof(T) * 8, "All bits of the type must be used");
}

template<size_t IntegerSize, size_t FractionSize>
inline FixedPoint<IntegerSize, FractionSize>& FixedPoint<IntegerSize, FractionSize>::multiplyAccumulate(
    const FixedPoint<IntegerSize, FractionSize>& left,
    const FixedPoint<IntegerSize, FractionSize>& right)
{
    m_value += multiplyFixedPoint(left.m_value, right.m_value);
    return *this;
}

template<size_t IntegerSize, size_t FractionSize>
inline FixedPoint<IntegerSize, FractionSize>& FixedPoint<IntegerSize, FractionSize>::multiplySubtract(
    const FixedPoint<IntegerSize, FractionSize>& left,
    const FixedPoint<IntegerSize, FractionSize>& right)
{
    m_value -= multiplyFixedPoint(left.m_value, right.m_value);
    return *this;
}

template<size_t IntegerSize, size_t FractionSize>
inline FixedPoint<IntegerSize, FractionSize>&
    FixedPoint<IntegerSize, FractionSize>::operator=(const FixedPoint<IntegerSize, FractionSize>& other)
{
    m_value = other.m_value;
    return *this;
}

template<size_t IntegerSize, size_t FractionSize>
inline FixedPoint<IntegerSize, FractionSize>&
    FixedPoint<IntegerSize, FractionSize>::operator+=(const FixedPoint<IntegerSize, FractionSize>& other)
{
    m_value += other.m_value;
    return *this;
}

template<size_t IntegerSize, size_t FractionSize>
inline FixedPoint<IntegerSize, FractionSize>&
    FixedPoint<IntegerSize, FractionSize>::operator-=(const FixedPoint<IntegerSize, FractionSize>& other)
{
    m_value -= other.m_value;
    return *this;
}

template<size_t IntegerSize, size_t FractionSize>
inline FixedPoint<IntegerSize, FractionSize>&
    FixedPoint<IntegerSize, FractionSize>::operator*=(const FixedPoint<IntegerSize, FractionSize>& other)
{
    m_value = multiplyFixedPoint(m_value, other.m_value);
    return *this;
}

template<size_t IntegerSize, size_t FractionSize>
inline FixedPoint<IntegerSize, FractionSize>&
    FixedPoint<IntegerSize, FractionSize>::operator/=(const FixedPoint<IntegerSize, FractionSize>& other)
{
    m_value = divideFixedPoint(m_value, other.m_value);
    return *this;
}

template<size_t IntegerSize, size_t FractionSize>
inline FixedPoint<IntegerSize, FractionSize>& FixedPoint<IntegerSize, FractionSize>::operator++()
{
    m_value += ONE_FIXED_POINT;
    return *this;
}

template<size_t IntegerSize, size_t FractionSize>
inline const FixedPoint<IntegerSize, FractionSize> FixedPoint<IntegerSize, FractionSize>::operator++(int dummy)
{
    T oldValue = m_value;
    m_value += ONE_FIXED_POINT;
    return FixedPoint<IntegerSize, FractionSize>(oldValue);
}

template<size_t IntegerSize, size_t FractionSize>
inline FixedPoint<IntegerSize, FractionSize>& FixedPoint<IntegerSize, FractionSize>::operator--()
{
    m_value -= ONE_FIXED_POINT;
    return *this;
}

template<size_t IntegerSize, size_t FractionSize>
inline const FixedPoint<IntegerSize, FractionSize> FixedPoint<IntegerSize, FractionSize>::operator--(int dummy)
{
    T oldValue = m_value;
    m_value -= ONE_FIXED_POINT;
    return FixedPoint<IntegerSize, FractionSize>(oldValue);
}

template<size_t IntegerSize, size_t FractionSize>
inline FixedPoint<IntegerSize, FractionSize> FixedPoint<IntegerSize, FractionSize>::operator-()
{
    return FixedPoint<IntegerSize, FractionSize>(static_cast<T>(-m_value));
}

template<size_t IntegerSize, size_t FractionSize>
inline FixedPoint<IntegerSize, FractionSize>::operator float() const
{
    T signValue = FixedPoint::sign_(m_value);
    T absValue = FixedPoint::abs_(m_value);
    float integerPart = absValue >> FractionSize;
    float fractionalPart = static_cast<float>(absValue & FRACTION_MASK) / ONE_FIXED_POINT;
    return (integerPart + fractionalPart) * signValue;
}

template<size_t IntegerSize, size_t FractionSize>
inline FixedPoint<IntegerSize, FractionSize>::operator double() const
{
    T signValue = FixedPoint::sign_(m_value);
    T absValue = FixedPoint::abs_(m_value);
    double integerPart = absValue >> FractionSize;
    double fractionalPart = static_cast<double>(absValue & FRACTION_MASK) / ONE_FIXED_POINT;
    return (integerPart + fractionalPart) * signValue;
}

template<size_t IntegerSize, size_t FractionSize>
inline typename FixedPoint<IntegerSize, FractionSize>::T
    FixedPoint<IntegerSize, FractionSize>::multiplyFixedPoint(T left, T right)
{
    static_assert(sizeof(T) < sizeof(int64_t), "The multiplication is not supported.");
    return static_cast<T>((static_cast<T2X>(left) * static_cast<T2X>(right)) >> FractionSize);
}

template<size_t IntegerSize, size_t FractionSize>
inline typename FixedPoint<IntegerSize, FractionSize>::T
    FixedPoint<IntegerSize, FractionSize>::divideFixedPoint(T left, T right)
{
    static_assert(sizeof(T) < sizeof(int64_t), "The division is not supported.");
    return static_cast<T>((static_cast<T2X>(left) << FractionSize) / static_cast<T2X>(right));
}

template<size_t IntegerSize, size_t FractionSize>
inline typename FixedPoint<IntegerSize, FractionSize>::T FixedPoint<IntegerSize, FractionSize>::sign_(T value)
{
    return static_cast<T>(value < 0 ? -1 : 1);
}

template<size_t IntegerSize, size_t FractionSize>
inline typename FixedPoint<IntegerSize, FractionSize>::T FixedPoint<IntegerSize, FractionSize>::abs_(T value)
{
    return value < 0 ? -value : value;
}

typedef FixedPoint<3, 4> FixedPointQ3_4;
typedef FixedPoint<7, 8> FixedPointQ7_8;
typedef FixedPoint<15, 16> FixedPointQ15_16;
typedef FixedPoint<31, 32> FixedPointQ31_32;

#endif
