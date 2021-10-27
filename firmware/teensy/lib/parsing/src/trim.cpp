#include <trim.h>

#include <cstring>

using namespace std;

void ltrim(char* string) {
  size_t stringSize = strlen(string);
  if (stringSize == 0) {
    return;
  }

  size_t count = 0;
  while (string[count] == ' ' || string[count] == '\t' || string[count] == '\n') {
    count++;
  }

  if (count >= stringSize) {
    string[0] = '\0';
  }
  else {
    memmove(string, string + count, stringSize - count);
    string[stringSize - count] = '\0';
  }
}

void rtrim(char* string) {
  size_t stringSize = strlen(string);
  if (stringSize == 0) {
    return;
  }

  size_t count = 0;
  while (string[stringSize - count - 1] == ' ' || string[stringSize - count - 1] == '\t' || string[stringSize - count - 1] == '\n') {
    count++;
  }

  if (count >= stringSize) {
    string[0] = '\0';
  }
  else {
    string[stringSize - count] = '\0';
  }
}

void trim(char* string) {
  ltrim(string);
  rtrim(string);
}
