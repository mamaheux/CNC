#include <cnc/config/ConfigItem.h>
#include <cnc/space.h>

#include <cstring>

using namespace std;

FLASHMEM ConfigItem::ConfigItem()
{
    memset(m_key, '\0', CONFIG_ITEM_BUFFER_SIZE);
    memset(m_value, '\0', CONFIG_ITEM_BUFFER_SIZE);
}

FLASHMEM ConfigItem::ConfigItem(const char* key, const char* value)
{
    strncpy(m_key, key, CONFIG_ITEM_BUFFER_SIZE - 1);
    strncpy(m_value, value, CONFIG_ITEM_BUFFER_SIZE - 1);
}
