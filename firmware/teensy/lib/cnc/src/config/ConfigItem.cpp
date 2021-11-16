#include <cnc/config/ConfigItem.h>

#include <cstring>

using namespace std;

ConfigItem::ConfigItem() {
  memset(m_key, '\0', CONFIG_ITEM_BUFFER_SIZE);
  memset(m_value, '\0', CONFIG_ITEM_BUFFER_SIZE);
}
