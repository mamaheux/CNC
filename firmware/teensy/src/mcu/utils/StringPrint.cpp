#include "mcu/utils/StringPrint.h"

StringPrint::StringPrint(char* buffer, size_t size) : m_buffer(buffer), m_size(size), m_index(0) {
  memset(m_buffer, '\0', m_size);
}

size_t StringPrint::write(uint8_t b) {
  if (m_index < (m_size - 1)) {
    m_buffer[m_index] = static_cast<char>(b);
    m_index++;
    return 1;
  }
  return 0;
}

int StringPrint::availableForWrite() {
  return m_size - m_index - 1;
}
