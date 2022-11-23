#ifndef MCU_MODULES_COMMAND_SERIAL_H
#define MCU_MODULES_COMMAND_SERIAL_H

#include <cnc/modules/Module.h>

#include <tl/optional.hpp>

constexpr size_t COMMAND_SERIAL_LINE_BUFFER_SIZE = 256;

class CommandSerial : public Module {
  tl::optional<uint32_t> m_baudRate;

  size_t m_lineIndex;
  char m_lineBuffer[COMMAND_SERIAL_LINE_BUFFER_SIZE];
  tl::optional<uint32_t> m_pendingCommandId;

public:
  CommandSerial();
  ~CommandSerial() override = default;

  DECLARE_NOT_COPYABLE(CommandSerial);
  DECLARE_NOT_MOVABLE(CommandSerial);

  void configure(const ConfigItem& item) override;
  void checkConfigErrors(std::function<void(const char*, const char*, const char*)> onMissingConfigItem) override;
  void begin() override;

  void update() override;

  void onCommandResponse(const char* response, CommandSource source, uint32_t commandId, bool isComplete) override;
};

#endif
