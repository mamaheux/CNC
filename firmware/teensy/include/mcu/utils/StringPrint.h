#ifndef MCU_UTILS_STRING_PRINT_H
#define MCU_UTILS_STRING_PRINT_H

#include <Arduino.h>

class StringPrint : public Print {
  char* m_buffer;
  size_t m_size;
  size_t m_index;

public:
  StringPrint(char* buffer, size_t size);

  size_t write(uint8_t b) override;
  int availableForWrite() override;

  void finish();
};

#endif
