#include <cnc/parsing/SystemCommand.h>

#include <cstring>

using namespace std;

SystemCommandParser::SystemCommandParser() {}

ParsingResult SystemCommandParser::parse(const char* line, SystemCommand& command)
{
    if (strstr(line, "$H") != nullptr)
    {
        command = SystemCommand::HOMING;
    }
    else if (isComment(line))
    {
        return ParsingResult::NEXT_LINE_NEEDED;
    }
    else
    {
        return ParsingResult::ERROR;
    }

    return ParsingResult::OK;
}

bool SystemCommandParser::isComment(const char* line)
{
    for (; *line != '\0'; line++)
    {
        if (*line == ';')
        {
            return true;
        }
        else if (*line != ' ' && *line != '\t')
        {
            return false;
        }
    }

    return true;
}
