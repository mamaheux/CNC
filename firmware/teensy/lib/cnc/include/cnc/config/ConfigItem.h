#ifndef CNC_CONFIG_CONFIG_ITEM_H
#define CNC_CONFIG_CONFIG_ITEM_H

#include <cstdlib>

constexpr size_t CONFIG_ITEM_BUFFER_SIZE = 256;

class ConfigFile;

class ConfigItem
{
    char m_key[CONFIG_ITEM_BUFFER_SIZE];
    char m_value[CONFIG_ITEM_BUFFER_SIZE];

public:
    ConfigItem();
    ConfigItem(const char* key, const char* value);

    const char* getKey() const;
    const char* getValueString() const;
    int getValueInt() const;
    float getValueFloat() const;
    double getValueDouble() const;

    friend ConfigFile;
};

inline const char* ConfigItem::getKey() const
{
    return m_key;
}

inline const char* ConfigItem::getValueString() const
{
    return m_value;
}

inline int ConfigItem::getValueInt() const
{
    return std::atoi(m_value);
}

inline float ConfigItem::getValueFloat() const
{
    return static_cast<float>(std::atof(m_value));
}

inline double ConfigItem::getValueDouble() const
{
    return std::atof(m_value);
}

#endif
