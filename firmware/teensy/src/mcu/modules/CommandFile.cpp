#include "mcu/modules/CommandFile.h"
#include "mcu/utils/StringPrint.h"
#include "mcu/criticalError.h"

#include <cnc/modules/ModuleKernel.h>

#include <cstring>

using namespace std;

static const char* ERROR_RESPONSE_PREFIX = "error:";
constexpr size_t ERROR_RESPONSE_PREFIX_SIZE = 6;

static const char* LOG_FILE_PATH = "log.txt";

CommandFile::CommandFile() : m_completedLineCount(0), m_lineCount(0), m_isStarted(false), m_lineIndex(0) {
  memset(m_lineBuffer, '\0', COMMAND_FILE_LINE_BUFFER_SIZE);
}

void CommandFile::configure(const ConfigItem& item) {
}

void CommandFile::begin() {
  m_kernel->registerToEvent(ModuleEventType::MCODE_COMMAND, this);
  m_kernel->registerToEvent(ModuleEventType::COMMAND_RESPONSE, this);
}

void CommandFile::update() {
  if (m_pendingCommandId != tl::nullopt || !m_commandFile || !m_isStarted) {
    return;
  }

  while (m_commandFile.available() > 0) {
    char c = m_commandFile.read();
    if (c == '\r') {
      continue;
    }
    else if (c == '\n' || m_lineIndex == (COMMAND_FILE_LINE_BUFFER_SIZE - 1)) {
      m_lineBuffer[m_lineIndex] = '\0';
      m_lineIndex = 0;
      m_kernel->executeCommand(m_lineBuffer, CommandSource::SERIAL_SOURCE, m_pendingCommandId);
      m_completedLineCount++;
      break;
    }
    else {
      m_lineBuffer[m_lineIndex] = c;
    }
    m_lineIndex++;
  }
  if (m_commandFile.available()) {
    closeFiles();
  }
}

CommandResult CommandFile::onMCodeCommandReceived(const MCode& mcode, CommandSource source, uint32_t commandId) {
  if (source == CommandSource::FILE_SOURCE) {
    return CommandResult::NOT_HANDLED;
  }

  if ((mcode.code() == 23 && m_commandFile) ||
      (mcode.code() == 32 && m_commandFile) ||
      (mcode.code() == 23 && mcode.path() == nullptr && !m_commandFile) ||
      (mcode.code() == 32 && mcode.path() == nullptr && !m_commandFile)) {
    return CommandResult::ERROR;
  }
  else if (mcode.code() == 23) {
    return openFiles(mcode);
  }
  else if (mcode.code() == 24) {
    m_isStarted = true;
  }
  else if (mcode.code() == 25) {
    m_isStarted = false;
  }
  else if (mcode.code() == 26) {
    closeFiles();
  }
  else if (mcode.code() == 27) {
    sendProgress(source, commandId);
  }
  else if (mcode.code() == 32) {
    openFiles(mcode);
    m_isStarted = true;
  }
  else {
    return CommandResult::NOT_HANDLED;
  }

  return CommandResult::OK;
}

void CommandFile::onCommandResponse(const char* response, CommandSource source, uint32_t commandId, bool isComplete) {
  if (source == CommandSource::SERIAL_SOURCE && m_pendingCommandId == commandId) {
    if (strncmp(response, ERROR_RESPONSE_PREFIX, ERROR_RESPONSE_PREFIX_SIZE) != 0) {
      m_logFile.print(m_completedLineCount);
      m_logFile.print(": ");
      m_logFile.print(m_lineBuffer);
      m_logFile.print(" --> ");
      m_logFile.println(response);
      closeFiles();
    }
    if (isComplete) {
      m_pendingCommandId = tl::nullopt;
    }
  }
}

CommandResult CommandFile::openFiles(const MCode& mcode) {
  m_commandFile = SD.open(mcode.path(), FILE_READ);
  SD.remove(LOG_FILE_PATH);
  m_logFile = SD.open(LOG_FILE_PATH, FILE_WRITE);

  if (m_commandFile && m_logFile) {
    m_completedLineCount = 0;
    m_lineCount = countLines();
    m_isStarted = false;
    return CommandResult::OK;
  }
  else {
    closeFiles();
    return CommandResult::ERROR;
  }
}

size_t CommandFile::countLines() {
  size_t count = 0;

  m_commandFile.seek(0);
  while (m_commandFile.available() > 0) {
    if (m_commandFile.read() == '\n') {
      count++;
    }
  }
  m_commandFile.seek(0);

  return count;
}

void CommandFile::closeFiles() {
  m_commandFile.close();
  m_commandFile = File();
  m_logFile.close();
  m_logFile = File();

  m_completedLineCount = 0;
  m_lineCount = 0;
  m_isStarted = false;
}

void CommandFile::sendProgress(CommandSource source, uint32_t commandId) {
  StringPrint stringPrint(m_response, MAX_COMMAND_FILE_RESPONSE_SIZE);
  stringPrint.print(m_completedLineCount);
  stringPrint.print("/");
  stringPrint.print(m_lineCount);

  m_kernel->sendCommandResponse(m_response, source, commandId, false);
}
