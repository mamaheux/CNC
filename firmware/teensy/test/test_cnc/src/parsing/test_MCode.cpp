#include <cnc/parsing/MCode.h>
#include <unity.h>

#define TEST_ASSERT_NO_PARAMETERS(code) \
  TEST_ASSERT_TRUE(tl::nullopt == code.x()); \
  TEST_ASSERT_TRUE(tl::nullopt == code.y()); \
  TEST_ASSERT_TRUE(tl::nullopt == code.z()); \
  TEST_ASSERT_TRUE(tl::nullopt == code.s()); \
  TEST_ASSERT_TRUE(tl::nullopt == code.p()); \
  TEST_ASSERT_TRUE(tl::nullopt == code.i()); \
  TEST_ASSERT_TRUE(tl::nullopt == code.d()); \
  TEST_ASSERT_EQUAL(nullptr,  code.path())

void test_parseMCode_empty() {
  MCode code;
  MCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::NEXT_LINE_NEEDED, parser.parse("", code));
  TEST_ASSERT_EQUAL(ParsingResult::NEXT_LINE_NEEDED, parser.parse(" ", code));
  TEST_ASSERT_EQUAL(ParsingResult::NEXT_LINE_NEEDED, parser.parse("; comment", code));
  TEST_ASSERT_EQUAL(ParsingResult::NEXT_LINE_NEEDED, parser.parse(" ; comment", code));
}

void test_parseMCode_tooBig() {
  MCode code;
  MCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::ERROR, parser.parse("M3                                                                                                                                                                                                                                                                                S5000", code));
}

void test_parseGCode_M3() {
  MCode code;
  MCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M3 S5000", code));
  TEST_ASSERT_EQUAL(3, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_TRUE(tl::nullopt == code.x());
  TEST_ASSERT_TRUE(tl::nullopt == code.y());
  TEST_ASSERT_TRUE(tl::nullopt == code.z());
  TEST_ASSERT_TRUE(5000 == code.s());
  TEST_ASSERT_TRUE(tl::nullopt == code.p());
  TEST_ASSERT_TRUE(tl::nullopt == code.i());
  TEST_ASSERT_TRUE(tl::nullopt == code.d());
  TEST_ASSERT_EQUAL(nullptr,  code.path());
}

void test_parseGCode_M5() {
  MCode code;
  MCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M5", code));
  TEST_ASSERT_EQUAL(5, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_NO_PARAMETERS(code);
}

void test_parseGCode_M17() {
  MCode code;
  MCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M17", code));
  TEST_ASSERT_EQUAL(17, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_NO_PARAMETERS(code);
}
void test_parseGCode_M18() {
  MCode code;
  MCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M18", code));
  TEST_ASSERT_EQUAL(18, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_NO_PARAMETERS(code);
}

void test_parseGCode_M84() {
  MCode code;
  MCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M84", code));
  TEST_ASSERT_EQUAL(84, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_NO_PARAMETERS(code);
}

void test_parseGCode_M20() {
  MCode code;
  MCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M20", code));
  TEST_ASSERT_EQUAL(20, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_NO_PARAMETERS(code);
}

void test_parseGCode_M23() {
  MCode code;
  MCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M23  path/file.gcode   ", code));
  TEST_ASSERT_EQUAL(23, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_TRUE(tl::nullopt == code.x());
  TEST_ASSERT_TRUE(tl::nullopt == code.y());
  TEST_ASSERT_TRUE(tl::nullopt == code.z());
  TEST_ASSERT_TRUE(tl::nullopt == code.s());
  TEST_ASSERT_TRUE(tl::nullopt == code.p());
  TEST_ASSERT_TRUE(tl::nullopt == code.i());
  TEST_ASSERT_TRUE(tl::nullopt == code.d());
  TEST_ASSERT_EQUAL_STRING("path/file.gcode", code.path());
}

void test_parseGCode_M24() {
  MCode code;
  MCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M24", code));
  TEST_ASSERT_EQUAL(24, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_NO_PARAMETERS(code);
}

void test_parseGCode_M25() {
  MCode code;
  MCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M25", code));
  TEST_ASSERT_EQUAL(25, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_NO_PARAMETERS(code);
}

void test_parseGCode_M26() {
  MCode code;
  MCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M26", code));
  TEST_ASSERT_EQUAL(26, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_NO_PARAMETERS(code);
}

void test_parseGCode_M27() {
  MCode code;
  MCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M27", code));
  TEST_ASSERT_EQUAL(27, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_NO_PARAMETERS(code);
}

void test_parseGCode_M28() {
  MCode code;
  MCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M28 file.gcode ", code));
  TEST_ASSERT_EQUAL(28, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_TRUE(tl::nullopt == code.x());
  TEST_ASSERT_TRUE(tl::nullopt == code.y());
  TEST_ASSERT_TRUE(tl::nullopt == code.z());
  TEST_ASSERT_TRUE(tl::nullopt == code.s());
  TEST_ASSERT_TRUE(tl::nullopt == code.p());
  TEST_ASSERT_TRUE(tl::nullopt == code.i());
  TEST_ASSERT_TRUE(tl::nullopt == code.d());
  TEST_ASSERT_EQUAL_STRING("file.gcode", code.path());
}

void test_parseGCode_M29() {
  MCode code;
  MCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M29", code));
  TEST_ASSERT_EQUAL(29, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_NO_PARAMETERS(code);
}

void test_parseGCode_M30() {
  MCode code;
  MCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M30 a/path/file.gcode", code));
  TEST_ASSERT_EQUAL(30, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_TRUE(tl::nullopt == code.x());
  TEST_ASSERT_TRUE(tl::nullopt == code.y());
  TEST_ASSERT_TRUE(tl::nullopt == code.z());
  TEST_ASSERT_TRUE(tl::nullopt == code.s());
  TEST_ASSERT_TRUE(tl::nullopt == code.p());
  TEST_ASSERT_TRUE(tl::nullopt == code.i());
  TEST_ASSERT_TRUE(tl::nullopt == code.d());
  TEST_ASSERT_EQUAL_STRING("a/path/file.gcode", code.path());
}

void test_parseGCode_M32() {
  MCode code;
  MCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M32 a/long/path/file.gcode  ", code));
  TEST_ASSERT_EQUAL(32, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_TRUE(tl::nullopt == code.x());
  TEST_ASSERT_TRUE(tl::nullopt == code.y());
  TEST_ASSERT_TRUE(tl::nullopt == code.z());
  TEST_ASSERT_TRUE(tl::nullopt == code.s());
  TEST_ASSERT_TRUE(tl::nullopt == code.p());
  TEST_ASSERT_TRUE(tl::nullopt == code.i());
  TEST_ASSERT_TRUE(tl::nullopt == code.d());
  TEST_ASSERT_EQUAL_STRING("a/long/path/file.gcode", code.path());
}

void test_parseGCode_M114() {
  MCode code;
  MCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M114", code));
  TEST_ASSERT_EQUAL(114, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_NO_PARAMETERS(code);
}

void test_parseGCode_M114D1() {
  MCode code;
  MCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M114.1", code));
  TEST_ASSERT_EQUAL(114, code.code());
  TEST_ASSERT_TRUE(1 == code.subcode());
  TEST_ASSERT_NO_PARAMETERS(code);
}

void test_parseGCode_M114D2() {
  MCode code;
  MCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M114.2", code));
  TEST_ASSERT_EQUAL(114, code.code());
  TEST_ASSERT_TRUE(2 == code.subcode());
  TEST_ASSERT_NO_PARAMETERS(code);
}

void test_parseGCode_M114D3() {
  MCode code;
  MCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M114.3", code));
  TEST_ASSERT_EQUAL(114, code.code());
  TEST_ASSERT_TRUE(3 == code.subcode());
  TEST_ASSERT_NO_PARAMETERS(code);
}

void test_parseGCode_M203() {
  MCode code;
  MCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M203 X1.25 Y1.5 Z1.75", code));
  TEST_ASSERT_EQUAL(203, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_TRUE(1.25 == code.x());
  TEST_ASSERT_TRUE(1.5 == code.y());
  TEST_ASSERT_TRUE(1.75 == code.z());
  TEST_ASSERT_TRUE(tl::nullopt == code.s());
  TEST_ASSERT_TRUE(tl::nullopt == code.p());
  TEST_ASSERT_TRUE(tl::nullopt == code.i());
  TEST_ASSERT_TRUE(tl::nullopt == code.d());
  TEST_ASSERT_EQUAL(nullptr,  code.path());
}

void test_parseGCode_M204() {
  MCode code;
  MCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M204 X1.25 Y1.5 Z1.75", code));
  TEST_ASSERT_EQUAL(204, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_TRUE(1.25 == code.x());
  TEST_ASSERT_TRUE(1.5 == code.y());
  TEST_ASSERT_TRUE(1.75 == code.z());
  TEST_ASSERT_TRUE(tl::nullopt == code.s());
  TEST_ASSERT_TRUE(tl::nullopt == code.p());
  TEST_ASSERT_TRUE(tl::nullopt == code.i());
  TEST_ASSERT_TRUE(tl::nullopt == code.d());
  TEST_ASSERT_EQUAL(nullptr,  code.path());
}

void test_parseGCode_M220() {
  MCode code;
  MCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M220 S25", code));
  TEST_ASSERT_EQUAL(220, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_TRUE(tl::nullopt == code.x());
  TEST_ASSERT_TRUE(tl::nullopt == code.y());
  TEST_ASSERT_TRUE(tl::nullopt == code.z());
  TEST_ASSERT_TRUE(25 == code.s());
  TEST_ASSERT_TRUE(tl::nullopt == code.p());
  TEST_ASSERT_TRUE(tl::nullopt == code.i());
  TEST_ASSERT_TRUE(tl::nullopt == code.d());
  TEST_ASSERT_EQUAL(nullptr,  code.path());
}

void test_parseGCode_M957() {
  MCode code;
  MCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M957", code));
  TEST_ASSERT_EQUAL(957, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_NO_PARAMETERS(code);
}

void test_parseGCode_M958() {
  MCode code;
  MCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M958 P0.75 I0.5 D0.25", code));
  TEST_ASSERT_EQUAL(958, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_TRUE(tl::nullopt == code.x());
  TEST_ASSERT_TRUE(tl::nullopt == code.y());
  TEST_ASSERT_TRUE(tl::nullopt == code.z());
  TEST_ASSERT_TRUE(tl::nullopt == code.s());
  TEST_ASSERT_TRUE(0.75 == code.p());
  TEST_ASSERT_TRUE(0.5 == code.i());
  TEST_ASSERT_TRUE(0.25 == code.d());
  TEST_ASSERT_EQUAL(nullptr,  code.path());
}
