#ifndef CORE_COMMAND_RESULT_H
#define CORE_COMMAND_RESULT_H

enum class CommandResult {
  OK,
  PENDING,
  ERROR
};

inline CommandResult agregateCommandResult(CommandResult r1, CommandResult r2) {
  if (r1 == CommandResult::ERROR || r2 == CommandResult::ERROR) {
    return CommandResult::ERROR;
  }
  else if (r1 == CommandResult::PENDING || r2 == CommandResult::PENDING) {
    return CommandResult::PENDING;
  }
  return CommandResult::OK;
}

#endif
