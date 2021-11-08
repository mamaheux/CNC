#include "mcu/modules/CommandSerial.h"
#include "mcu/criticalError.h"
#include "config.h"

#include <cnc/modules/ModuleKernel.h>

#include <cstring>

using namespace std;

static const char* BAUD_RATE_KEY = "command_serial.baud_rate";

CommandSerial::CommandSerial() : m_lineIndex(0) {
  memset(m_lineBuffer, '\0', COMMAND_SERIAL_LINE_BUFFER_SIZE);
}

void CommandSerial::configure(const ConfigItem& item) {
  if (strcmp(item.getKey(), BAUD_RATE_KEY) == 0) {
    m_baudRate = item.getValueInt();
  }
}

void CommandSerial::begin() {
  CRITICAL_ERROR_CHECK_3(m_baudRate.has_value(), "Missing item in config.properties (key = ", BAUD_RATE_KEY, ")");
  COMMAND_SERIAL.begin(*m_baudRate);

  m_kernel->registerToEvent(ModuleEventType::COMMAND_RESPONSE, this);
}

void CommandSerial::update() {
  if (m_pendingCommandId != tl::nullopt) {
    return;
  }

  while (COMMAND_SERIAL.available() > 0) {
    char c = COMMAND_SERIAL.read();
    if (c == '\r') {
      continue;
    }
    else if (c == '\n') {
      m_lineBuffer[m_lineIndex] = '\0';
      m_lineIndex = 0;
      m_kernel->executeCommand(m_lineBuffer, CommandSource::SERIAL_SOURCE, m_pendingCommandId);
      break;
    }
    else {
      m_lineBuffer[m_lineIndex] = c;
    }
    m_lineIndex++;
  }
}

void CommandSerial::onCommandResponse(const char* response, CommandSource source, uint32_t commandId, bool isComplete) {
  if (source == CommandSource::SERIAL_SOURCE && m_pendingCommandId == commandId) {
    COMMAND_SERIAL.println(response);
    if (isComplete) {
      m_pendingCommandId = tl::nullopt;
    }
  }
}
