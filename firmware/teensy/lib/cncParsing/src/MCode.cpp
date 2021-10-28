#include <MCode.h>
#include <trim.h>

#include <limits>
#include <cstring>

using namespace std;

MCode::MCode() : m_x(tl::nullopt),
  m_y(tl::nullopt),
  m_z(tl::nullopt),
  m_s(tl::nullopt),
  m_p(tl::nullopt),
  m_i(tl::nullopt),
  m_d(tl::nullopt),
  m_path(nullptr),
  m_code(numeric_limits<uint32_t>::max()),
  m_subcode(tl::nullopt)
{
}

void MCode::clear() {
  m_x = tl::nullopt;
  m_y = tl::nullopt;
  m_z = tl::nullopt;
  m_s = tl::nullopt;
  m_p = tl::nullopt;
  m_i = tl::nullopt;
  m_d = tl::nullopt;
  m_path = nullptr;
  m_code = numeric_limits<uint32_t>::max();
  m_subcode = tl::nullopt;
}

MCodeParser::MCodeParser() {
  memset(m_lineBuffer, '\0', MCODE_LINE_BUFFER_SIZE);
}

ParsingResult MCodeParser::parse(const char* line, MCode& mcode) {
  mcode.clear();

  const char* commentPointer = strchr(line, ';');
  size_t lineSize = 0;
  if (commentPointer == nullptr) {
    lineSize = strlen(line);
  }
  else {
    lineSize = commentPointer - line;
  }

  if (lineSize >= MCODE_LINE_BUFFER_SIZE) {
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

  return parseNormalMCode(m_lineBuffer, mcode);
}

ParsingResult MCodeParser::parseNormalMCode(char* start, MCode& mcode) {
  if (start[0] != 'M') {
    return ParsingResult::ERROR;
  }

  mcode.m_code = atoi(start + 1);

  const char* dotPointer = strchr(start + 1, '.');
  char* spacePointer = const_cast<char*>(strchr(start + 1, ' '));
  if ((dotPointer != nullptr && spacePointer == nullptr) || (dotPointer != nullptr && spacePointer != nullptr && dotPointer < spacePointer)) {
    mcode.m_subcode = atoi(dotPointer + 1);
  }

  if (spacePointer == nullptr) {
    return ParsingResult::OK;
  }

  switch (mcode.m_code) {
    case 23:
    case 28:
    case 30:
    case 32:
      return parsePath(spacePointer + 1, mcode);

    default:
      return parseParameters(spacePointer + 1, mcode);
  }
}

ParsingResult MCodeParser::parseParameters(char* start, MCode& mcode) {
  switch (start[0])
  {
  case '\0':
    return ParsingResult::OK;
  case ' ':
    return parseParameters(start + 1, mcode);
  case 'X':
    mcode.m_x = atof(start + 1);
    break;
  case 'Y':
    mcode.m_y = atof(start + 1);
    break;
  case 'Z':
    mcode.m_z = atof(start + 1);
    break;
  case 'S':
    mcode.m_s = atof(start + 1);
    break;
  case 'P':
    mcode.m_p = atof(start + 1);
    break;
  case 'I':
    mcode.m_i = atof(start + 1);
    break;
  case 'D':
    mcode.m_d = atof(start + 1);
    break;
  default:
    return ParsingResult::ERROR;
  }

  char* spacePointer = const_cast<char*>(strchr(start + 1, ' '));
  if (spacePointer == nullptr) {
    return ParsingResult::OK;
  }
  else {
    return parseParameters(spacePointer + 1, mcode);
  }
}

ParsingResult MCodeParser::parsePath(char* start, MCode& mcode) {
  trim(start);
  mcode.m_path = start;
  return ParsingResult::OK;
}
