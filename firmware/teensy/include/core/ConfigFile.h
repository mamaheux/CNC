#ifndef CORE_CONFIG_FILE_H
#define CORE_CONFIG_FILE_H

#include <SD.h>

#include <cstdlib>

constexpr size_t CONFIG_ITEM_BUFFER_SIZE = 256;

class ConfigFile;

class ConfigItem {
  char m_key[CONFIG_ITEM_BUFFER_SIZE];
  char m_value[CONFIG_ITEM_BUFFER_SIZE];

public:
  ConfigItem();

  const char* getKey();
  const char* getValueString();
  int getValueInt();
  float getValueFloat();

  friend ConfigFile;
};

inline const char* ConfigItem::getKey() {
  return m_key;
}

inline const char* ConfigItem::getValueString() {
  return m_value;
}

inline int ConfigItem::getValueInt() {
  return std::atoi(m_value);
}

inline float ConfigItem::getValueFloat() {
  return static_cast<float>(std::atof(m_value));
}

class ConfigFile {
  File m_file;

public:
  ConfigFile(const char* path);
  ~ConfigFile();

  bool readNext(ConfigItem& item);
  bool isEndOfFile();
};

inline bool ConfigFile::isEndOfFile() {
  return m_file.available() == 0;
}

#endif
