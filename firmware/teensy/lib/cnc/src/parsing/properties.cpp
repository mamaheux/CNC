#include <cnc/parsing/properties.h>
#include <cnc/parsing/trim.h>

#include <cstring>

#include <iostream>

using namespace std;

bool parsePropertiesLine(const char* line, char* key, size_t maxKeySize, char* value, size_t maxValueSize) {
  size_t lineSize = strlen(line);

  const char* equalPointer = strchr(line, '=');
  if (equalPointer == nullptr || line == equalPointer) {
    return false;
  }

  const char* hashTagIndex = strchr(line, '#');
  if (hashTagIndex != nullptr && hashTagIndex < equalPointer) {
    return false;
  }

  size_t keySize = equalPointer - line;
  size_t valueSize = lineSize - keySize - 1;
  if (hashTagIndex != nullptr) {
    valueSize = hashTagIndex - equalPointer - 1;
  }

  if (keySize >= maxKeySize - 1 || keySize == 0) {
    return false;
  }
  if (valueSize >= maxValueSize - 1 || valueSize == 0) {
    return false;
  }

  memcpy(key, line, keySize);
  memcpy(value, equalPointer + 1, valueSize);

  key[keySize] = '\0';
  value[valueSize] = '\0';

  trim(key);
  trim(value);

  return strlen(key) != 0 && strlen(value) != 0;
}
