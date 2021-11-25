#ifndef CNC_PARSING_SYSTEM_COMMAND_H
#define CNC_PARSING_SYSTEM_COMMAND_H

#include <cnc/parsing/ParsingResult.h>

enum class SystemCommand {
	HOMING
};

class SystemCommandParser {
public:
	SystemCommandParser();

	ParsingResult parse(const char* line, SystemCommand& gcode);

private:
	bool isComment(const char* line);
};

#endif
