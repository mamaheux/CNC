#ifndef CNC_CONFIG_CONFIG_ITEM_H
#define CNC_CONFIG_CONFIG_ITEM_H

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

#endif
