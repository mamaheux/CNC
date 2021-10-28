#include "core/ConfigFile.h"
#include "core/criticalError.h"

#include <cncParsing.h>

ConfigItem::ConfigItem() {
  memset(m_key, '\0', CONFIG_ITEM_BUFFER_SIZE);
  memset(m_value, '\0', CONFIG_ITEM_BUFFER_SIZE);
}

ConfigFile::ConfigFile(const char* path) {
  CRITICAL_ERROR_CHECK_3(SD.exists(path), "The configuration file does not exist (", path, ")");
  m_file = SD.open(path, FILE_READ);
  CRITICAL_ERROR_CHECK(m_file, "Opening the configuration file failed");
}

ConfigFile::~ConfigFile() {
  if (m_file) {
    m_file.close();
  }
}

bool ConfigFile::readNext(ConfigItem& item) {
  constexpr size_t MAX_LINE_SIZE = 512;
  char line[MAX_LINE_SIZE];
  memset(line, '\0', MAX_LINE_SIZE);

  for (size_t i = 0;  i < MAX_LINE_SIZE && m_file.available() > 0; i++) {
    line[i] = m_file.read();
    if (line[i] == '\n') {
      line[i] = '\0';
      break;
    }
  }

  return parsePropertiesLine(line, item.m_key, CONFIG_ITEM_BUFFER_SIZE, item.m_value, CONFIG_ITEM_BUFFER_SIZE);
}
