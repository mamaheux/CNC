#ifndef CNC_COLLECTIONS_BOUNDED_QUEUE_H
#define CNC_COLLECTIONS_BOUNDED_QUEUE_H

#include <tl/optional.hpp>

#include <cstdlib>

template <class T, size_t MAX_SIZE>
class BoundedQueue {
  T m_data[MAX_SIZE];
  size_t m_writeIndex;
  size_t m_readIndex;
  size_t m_size;

public:
  BoundedQueue();

  tl::optional<T> pop();
  bool pop(T& value);
  bool push(const T& value);

  bool last(T& value) const;

  bool isEmpty() const;
  bool isFull() const;
  size_t size();
};

template <class T, size_t MAX_SIZE>
inline BoundedQueue<T, MAX_SIZE>::BoundedQueue() : m_writeIndex(0), m_readIndex(0), m_size(0) {
}

template <class T, size_t MAX_SIZE>
inline tl::optional<T> BoundedQueue<T, MAX_SIZE>::pop() {
  if (isEmpty()) {
    return tl::nullopt;
  }

  m_size--;
  size_t index = m_readIndex;
  m_readIndex = (m_readIndex + 1) % MAX_SIZE;
  return m_data[index];
}

template <class T, size_t MAX_SIZE>
inline bool BoundedQueue<T, MAX_SIZE>::pop(T& value) {
  if (isEmpty()) {
    return false;
  }

  m_size--;
  size_t index = m_readIndex;
  m_readIndex = (m_readIndex + 1) % MAX_SIZE;
  value = m_data[index];
  return true;
}

template <class T, size_t MAX_SIZE>
inline bool BoundedQueue<T, MAX_SIZE>::push(const T& value) {
  if (isFull()) {
    return false;
  }

  m_size++;
  m_data[m_writeIndex] = value;
  m_writeIndex = (m_writeIndex + 1) % MAX_SIZE;
  return true;
}

template <class T, size_t MAX_SIZE>
inline bool BoundedQueue<T, MAX_SIZE>::last(T& value) const {
  if (isEmpty()) {
    return false;
  }

  if (m_writeIndex == 0) {
    value = m_data[MAX_SIZE - 1];
  }
  else {
    value = m_data[m_writeIndex - 1];
  }

  return true;
}

template <class T, size_t MAX_SIZE>
inline bool BoundedQueue<T, MAX_SIZE>::isEmpty() const {
  return m_size == 0;
}

template <class T, size_t MAX_SIZE>
inline bool BoundedQueue<T, MAX_SIZE>::isFull() const {
  return m_size >= MAX_SIZE;
}

template <class T, size_t MAX_SIZE>
inline size_t BoundedQueue<T, MAX_SIZE>::size() {
  return m_size;
}

#endif
