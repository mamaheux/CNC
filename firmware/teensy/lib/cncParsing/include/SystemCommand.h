#ifndef PARSING_SYSTEM_COMMAND_H
#define PARSING_SYSTEM_COMMAND_H

#include <ParsingResult.h>

enum class SystemCommand {
	HOMING
};

class SystemCommandParser {
public:
	SystemCommandParser();

	ParsingResult parse(const char* line, SystemCommand& gcode);
};

#endif
