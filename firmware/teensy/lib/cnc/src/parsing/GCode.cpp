#include <cnc/parsing/GCode.h>
#include <cnc/parsing/trim.h>

#include <limits>
#include <cstring>

using namespace std;

GCode::GCode() : m_x(tl::nullopt),
  m_y(tl::nullopt),
  m_z(tl::nullopt),
  m_f(tl::nullopt),
  m_i(tl::nullopt),
  m_j(tl::nullopt),
  m_k(tl::nullopt),
  m_s(tl::nullopt),
  m_p(tl::nullopt),
  m_l(tl::nullopt),
  m_code(numeric_limits<uint32_t>::max()),
  m_subcode(tl::nullopt),
  m_isMachineCoordinateSystem(false)
{
}

void GCode::clear() {
  m_x = tl::nullopt;
  m_y = tl::nullopt;
  m_z = tl::nullopt;
  m_f = tl::nullopt;
  m_i = tl::nullopt;
  m_j = tl::nullopt;
  m_k = tl::nullopt;
  m_s = tl::nullopt;
  m_p = tl::nullopt;
  m_l = tl::nullopt;
  m_code = numeric_limits<uint32_t>::max();
  m_subcode = tl::nullopt;
  m_isMachineCoordinateSystem = false;
}

GCodeParser::GCodeParser() : m_modalMoveCode(tl::nullopt), m_isMachineCoordinateSystem(false) {
  memset(m_lineBuffer, '\0', GCODE_LINE_BUFFER_SIZE);
}

ParsingResult GCodeParser::parse(const char* line, GCode& gcode) {
  gcode.clear();

  const char* commentPointer = strchr(line, ';');
  size_t lineSize = 0;
  if (commentPointer == nullptr) {
    lineSize = strlen(line);
  }
  else {
    lineSize = commentPointer - line;
  }

  if (lineSize >= GCODE_LINE_BUFFER_SIZE) {
    return ParsingResult::ERROR;
  }
  else {
    memcpy(m_lineBuffer, line, lineSize);
    m_lineBuffer[lineSize] = '\0';
  }
  rtrim(m_lineBuffer);
  lineSize = strlen(m_lineBuffer);

  if (lineSize == 0) {
    return ParsingResult::NEXT_LINE_NEEDED;
  }
  else if (m_isMachineCoordinateSystem) {
    return parseG53FollowingLine(gcode);
  }
  else if (m_lineBuffer[0] == ' ') {
    return parseModalMove(gcode);
  }
  else if (lineSize >= 3 && m_lineBuffer[0] == 'G' && m_lineBuffer[1] == '5' && m_lineBuffer[2] == '3') {
    return parseG53(gcode, lineSize);
  }

  return parseNormalGCode(m_lineBuffer, gcode);
}

ParsingResult GCodeParser::parseModalMove(GCode& gcode) {
  if (!m_modalMoveCode.has_value()) {
    return ParsingResult::ERROR;
  }

  gcode.m_code = *m_modalMoveCode;
  return parseParameters(m_lineBuffer + 1, gcode);
}

ParsingResult GCodeParser::parseG53(GCode& gcode, size_t lineSize) {
  if (lineSize == 3) {
    m_isMachineCoordinateSystem = true;
    return ParsingResult::NEXT_LINE_NEEDED;
  }

  gcode.m_isMachineCoordinateSystem = true;
  ltrim(m_lineBuffer + 3);
  if (m_lineBuffer[3] != 'G') {
    gcode.m_code = *m_modalMoveCode;
    return parseParameters(m_lineBuffer + 3, gcode);
  }
  else {
    return parseNormalGCode(m_lineBuffer + 3, gcode);
  }
}

ParsingResult GCodeParser::parseG53FollowingLine(GCode& gcode) {
  m_isMachineCoordinateSystem = false;
  gcode.m_isMachineCoordinateSystem = true;

  if (m_lineBuffer[0] == ' ') {
    return parseModalMove(gcode);
  }
  else {
    return parseNormalGCode(m_lineBuffer, gcode);
  }
}

ParsingResult GCodeParser::parseNormalGCode(char* start, GCode& gcode) {
  if (start[0] != 'G') {
    return ParsingResult::ERROR;
  }

  gcode.m_code = atoi(start + 1);
  if (0 <= gcode.m_code && gcode.m_code <= 3) {
    m_modalMoveCode = gcode.m_code;
  }

  const char* dotPointer = strchr(start + 1, '.');
  char* spacePointer = const_cast<char*>(strchr(start + 1, ' '));
  if ((dotPointer != nullptr && spacePointer == nullptr) || (dotPointer != nullptr && spacePointer != nullptr && dotPointer < spacePointer)) {
    gcode.m_subcode = atoi(dotPointer + 1);
  }

  if (spacePointer == nullptr) {
    return ParsingResult::OK;
  }
  else {
    return parseParameters(spacePointer + 1, gcode);
  }
}

ParsingResult GCodeParser::parseParameters(char* start, GCode& gcode) {
  int l;
  switch (start[0])
  {
  case '\0':
    return ParsingResult::OK;
  case ' ':
    return parseParameters(start + 1, gcode);
  case 'X':
    gcode.m_x = atof(start + 1);
    break;
  case 'Y':
    gcode.m_y = atof(start + 1);
    break;
  case 'Z':
    gcode.m_z = atof(start + 1);
    break;
  case 'F':
    gcode.m_f = atof(start + 1);
    break;
  case 'I':
    gcode.m_i = atof(start + 1);
    break;
  case 'J':
    gcode.m_j = atof(start + 1);
    break;
  case 'K':
    gcode.m_k = atof(start + 1);
    break;
  case 'S':
    gcode.m_s = atof(start + 1);
    break;
  case 'P':
    gcode.m_p = atof(start + 1);
    break;
  case 'R':
    gcode.m_r = atof(start + 1);
    break;
  case 'L':
    l = atoi(start + 1);
    if  (l >= 0) {
      gcode.m_l = l;
    }
    else {
      return ParsingResult::ERROR;
    }
    break;
  default:
    return ParsingResult::ERROR;
  }

  char* spacePointer = const_cast<char*>(strchr(start + 1, ' '));
  if (spacePointer == nullptr) {
    return ParsingResult::OK;
  }
  else {
    return parseParameters(spacePointer + 1, gcode);
  }
}
