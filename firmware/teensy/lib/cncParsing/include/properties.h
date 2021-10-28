#ifndef PARSING_PROPERTIES_H
#define PARSING_PROPERTIES_H

#include <cstdlib>

bool parsePropertiesLine(const char* line, char* key, size_t keySize, char* value, size_t valueSize);

#endif
