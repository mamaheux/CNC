#ifndef CNC_MATH_VECTOR2_H
#define CNC_MATH_VECTOR2_H

#include <cmath>

template <class T>
class Vector2 {
public:
  T x;
  T y;

  Vector2();
  Vector2(T x, T y);

  T dot(const Vector2<T>& other) const;
  T angle(const Vector2<T>& other) const;

  T norm() const;
  void normalize();
  Vector2<T> normalized() const;
};

template <class T>
Vector2<T>::Vector2() : x(0), y(0) {
}

template <class T>
Vector2<T>::Vector2(T x, T y) : x(x), y(y) {
}

template <class T>
T Vector2<T>::dot(const Vector2<T>& other) const {
  return x * other.x + y * other.y;
}

template <class T>
T Vector2<T>::angle(const Vector2<T>& other) const {
  return std::acos(dot(other) / norm() / other.norm());
}

template <class T>
T Vector2<T>::norm() const {
  return std::sqrt(x * x + y * y);
}

template <class T>
void Vector2<T>::normalize() {
  T n = norm();
  x /= n;
  y /= n;
}

template <class T>
Vector2<T> Vector2<T>::normalized() const {
  Vector2<T> normalizedVector(*this);
  normalizedVector.normalize();
  return normalizedVector;
}

template <class T>
Vector2<T> operator+(const Vector2<T>& l, const Vector2<T>& r) {
  return Vector2<T>(l.x + r.x, l.y + r.y);
}

template <class T>
Vector2<T>& operator+=(Vector2<T>& l, const Vector2<T>& r) {
  l.x += r.x;
  l.y += r.y;
  return l;
}

template <class T>
Vector2<T> operator-(const Vector2<T>& l, const Vector2<T>& r) {
  return Vector2<T>(l.x - r.x, l.y - r.y);
}

template <class T>
Vector2<T>& operator-=(Vector2<T>& l, const Vector2<T>& r) {
  l.x -= r.x;
  l.y -= r.y;
  return l;
}

template <class T, class C>
Vector2<T> operator*(const Vector2<T>& l, const C& r) {
  return Vector2<T>(l.x * r, l.y * r);
}

template <class T, class C>
Vector2<T> operator*(const C& l, const Vector2<T>& r) {
  return Vector2<T>(l * r.x, l * r.y);
}

template <class T, class C>
Vector2<T>& operator*=(Vector2<T>& l, const C& r) {
  l.x *= r;
  l.y *= r;
  return l;
}

template <class T, class C>
Vector2<T> operator/(const Vector2<T>& l, const C& r) {
  return Vector2<T>(l.x / r, l.y / r);
}

template <class T, class C>
Vector2<T> operator/(const C& l, const Vector2<T>& r) {
  return Vector2<T>(l / r.x, l / r.y);
}

template <class T, class C>
Vector2<T>& operator/=(Vector2<T>& l, const C& r) {
  l.x /= r;
  l.y /= r;
  return l;
}

#endif
