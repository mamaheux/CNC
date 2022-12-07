#ifndef MCU_CONFIG_FILE_H
#define MCU_CONFIG_FILE_H

#include <cnc/config/ConfigItem.h>

#include <SD.h>

#include <cstdlib>

class ConfigFile
{
    File m_file;

public:
    explicit ConfigFile(const char* path);
    ~ConfigFile();

    bool readNext(ConfigItem& item);
    bool isEndOfFile();
};

inline bool ConfigFile::isEndOfFile()
{
    return m_file.available() == 0;
}

#endif
