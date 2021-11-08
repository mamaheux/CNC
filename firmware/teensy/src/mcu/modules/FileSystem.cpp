#include <mcu/modules/FileSystem.h>

#include <cnc/modules/ModuleKernel.h>

FileSystem::FileSystem() {
}

void FileSystem::configure(const ConfigItem& item) {
}

void FileSystem::begin() {
  m_kernel->registerToEvent(ModuleEventType::RAW_COMMAND, this);
  m_kernel->registerToEvent(ModuleEventType::MCODE_COMMAND, this);
}

RawCommandResult FileSystem::onRawCommandReceived(const char* line, CommandSource source, uint32_t commandId) {
  if (m_newFile && source != CommandSource::FILE_SOURCE) {
    writeOrStopNewFile(line, source, commandId);
    return RawCommandResult::HANDLED;
  }

  return RawCommandResult::NOT_HANDLED;
}

CommandResult FileSystem::onMCodeCommandReceived(const MCode& mcode, CommandSource source, uint32_t commandId) {
  if (mcode.code() == 20) {
    File root = SD.open("/");
    listFiles(root, 0, source, commandId);
    m_kernel->sendCommandResponse(OK_COMMAND_RESPONSE, source, commandId);
    return CommandResult::OK_RESPONSE_SENT;
  }
  else if (mcode.code() == 28) {
    return startNewFile(mcode.path());
  }
  else if (mcode.code() == 30) {
    return deleteFile(mcode.path());
  }

  return CommandResult::NOT_HANDLED;
}

void FileSystem::listFiles(File& directory, size_t spaceCount, CommandSource source, uint32_t commandId) {
  if (spaceCount >= MAX_FILE_SYSTEM_RESPONSE_SIZE) {
    return;
  }

  while (true) {
    File nextFile = directory.openNextFile();
    if (!nextFile) {
      break;
    }

    sendFileEntry(nextFile, spaceCount, source, commandId);
    if (nextFile.isDirectory()) {
      listFiles(nextFile, spaceCount + 2, source, commandId);
    }
  }
}

void FileSystem::sendFileEntry(File& file, size_t spaceCount, CommandSource source, uint32_t commandId) {
  size_t nameSize = strlen(file.name());
  size_t directorySize = 0;
  if (file.isDirectory()) {
    directorySize = 1;
  }
  if ((spaceCount + nameSize + directorySize) >= MAX_FILE_SYSTEM_RESPONSE_SIZE) {
    return;
  }
  memset(m_response, ' ', spaceCount);

  strcpy(m_response + spaceCount, file.name());
  if (file.isDirectory()) {
    m_response[spaceCount + nameSize] = '/';
    m_response[spaceCount + nameSize + 1] = '\0';
  }
  else {
    m_response[spaceCount + nameSize] = '\0';
  }

  m_kernel->sendCommandResponse(m_response, source, commandId, false);
}

CommandResult FileSystem::startNewFile(const char* path) {
  m_newFile = SD.open(path, FILE_WRITE);
  if (m_newFile) {
    return CommandResult::OK;
  }
  else {
    return CommandResult::ERROR;
  }
}

void FileSystem::writeOrStopNewFile(const char* line, CommandSource source, uint32_t commandId) {
  MCode mcode;
  if (m_mcodeParser.parse(line, mcode) == ParsingResult::OK && mcode.code() == 29) {
    m_newFile.close();
    m_newFile = File();
  }
  else {
    m_newFile.println(line);
  }
  m_kernel->sendCommandResponse(OK_COMMAND_RESPONSE, source, commandId);
}

CommandResult FileSystem::deleteFile(const char* path) {
  if (SD.remove(path)) {
    return CommandResult::OK;
  }
  else {
    return CommandResult::ERROR;
  }
}
