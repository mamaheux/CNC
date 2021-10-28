#include <SystemCommand.h>

#include <cstring>

using namespace std;

SystemCommandParser::SystemCommandParser() {}

ParsingResult SystemCommandParser::parse(const char* line, SystemCommand& command) {
  if (strstr(line, "$H") != nullptr) {
    command = SystemCommand::HOMING;
  }
  else {
    return ParsingResult::ERROR;
  }

  return ParsingResult::OK;
}
