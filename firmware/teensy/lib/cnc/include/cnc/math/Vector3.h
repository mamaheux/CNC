#ifndef CNC_MATH_VECTOR3_H
#define CNC_MATH_VECTOR3_H

template <class T>
class Vector3 {
public:
  T x;
  T y;
  T z;

  Vector3();
  Vector3(T x, T y, T z);

  T dot(const Vector3<T>& other) const;
};

template <class T>
Vector3<T>::Vector3() : x(0), y(0), z(0) {
}

template <class T>
Vector3<T>::Vector3(T x, T y, T z) : x(x), y(y), z(z) {
}

template <class T>
T Vector3<T>::dot(const Vector3<T>& other) const {
  return x * other.x + y * other.y + z * other.z;
}

template <class T>
Vector3<T> operator+(const Vector3<T>& l, const Vector3<T>& r) {
  return Vector3<T>(l.x + r.x, l.y + r.y, l.z + r.z);
}

template <class T>
Vector3<T>& operator+=(Vector3<T>& l, const Vector3<T>& r) {
  l.x += r.x;
  l.y += r.y;
  l.z += r.z;
  return l;
}

template <class T>
Vector3<T> operator-(const Vector3<T>& l, const Vector3<T>& r) {
  return Vector3<T>(l.x - r.x, l.y - r.y, l.z - r.z);
}

template <class T>
Vector3<T>& operator-=(Vector3<T>& l, const Vector3<T>& r) {
  l.x -= r.x;
  l.y -= r.y;
  l.z -= r.z;
  return l;
}

template <class T, class C>
Vector3<T> operator*(const Vector3<T>& l, const C& r) {
  return Vector3<T>(l.x * r, l.y * r, l.z * r);
}

template <class T, class C>
Vector3<T> operator*(const C& l, const Vector3<T>& r) {
  return Vector3<T>(l * r.x, l * r.y, l * r.z);
}

template <class T, class C>
Vector3<T>& operator*=(Vector3<T>& l, const C& r) {
  l.x *= r;
  l.y *= r;
  l.z *= r;
  return l;
}

template <class T, class C>
Vector3<T> operator/(const Vector3<T>& l, const C& r) {
  return Vector3<T>(l.x / r, l.y / r, l.z / r);
}

template <class T, class C>
Vector3<T> operator/(const C& l, const Vector3<T>& r) {
  return Vector3<T>(l / r.x, l / r.y, l / r.z);
}

template <class T, class C>
Vector3<T>& operator/=(Vector3<T>& l, const C& r) {
  l.x /= r;
  l.y /= r;
  l.z /= r;
  return l;
}

#endif
