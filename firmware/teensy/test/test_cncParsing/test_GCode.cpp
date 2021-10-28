#include <cncParsing.h>
#include <unity.h>

#define TEST_ASSERT_NO_PARAMETERS(code) \
  TEST_ASSERT_TRUE(tl::nullopt == (code).x()); \
  TEST_ASSERT_TRUE(tl::nullopt == (code).y()); \
  TEST_ASSERT_TRUE(tl::nullopt == (code).z()); \
  TEST_ASSERT_TRUE(tl::nullopt == (code).f()); \
  TEST_ASSERT_TRUE(tl::nullopt == (code).i()); \
  TEST_ASSERT_TRUE(tl::nullopt == (code).j()); \
  TEST_ASSERT_TRUE(tl::nullopt == (code).k()); \
  TEST_ASSERT_TRUE(tl::nullopt == (code).s()); \
  TEST_ASSERT_TRUE(tl::nullopt == (code).p()); \
  TEST_ASSERT_TRUE(tl::nullopt == (code).l()); \
  TEST_ASSERT_FALSE(code.isMachineCoordinateSystem())

void test_parseGCode_empty() {
  GCode code;
  GCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::NEXT_LINE_NEEDED, parser.parse("", code));
  TEST_ASSERT_EQUAL(ParsingResult::NEXT_LINE_NEEDED, parser.parse(" ", code));
  TEST_ASSERT_EQUAL(ParsingResult::NEXT_LINE_NEEDED, parser.parse("; comment", code));
  TEST_ASSERT_EQUAL(ParsingResult::NEXT_LINE_NEEDED, parser.parse(" ; comment", code));
}

void test_parseGCode_tooBig() {
  GCode code;
  GCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::ERROR, parser.parse("G0 X1 Z1                                                                                                                                                                                                                                                                      Y10", code));
}

void test_parseGCode_noModalMove() {
  GCode code;
  GCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::ERROR, parser.parse(" X10", code));
}

void test_parseGCode_G0() {
  GCode code;
  GCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("G0 X10", code));
  TEST_ASSERT_EQUAL(0, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_TRUE(10.0 == code.x());
  TEST_ASSERT_TRUE(tl::nullopt == code.y());
  TEST_ASSERT_TRUE(tl::nullopt == code.z());
  TEST_ASSERT_TRUE(tl::nullopt == code.f());
  TEST_ASSERT_TRUE(tl::nullopt == code.i());
  TEST_ASSERT_TRUE(tl::nullopt == code.j());
  TEST_ASSERT_TRUE(tl::nullopt == code.k());
  TEST_ASSERT_TRUE(tl::nullopt == code.s());
  TEST_ASSERT_TRUE(tl::nullopt == code.p());
  TEST_ASSERT_TRUE(tl::nullopt == code.l());
  TEST_ASSERT_FALSE(code.isMachineCoordinateSystem());

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("G0  Y9  F5", code));
  TEST_ASSERT_EQUAL(0, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_TRUE(tl::nullopt == code.x());
  TEST_ASSERT_TRUE(9.0 == code.y());
  TEST_ASSERT_TRUE(tl::nullopt == code.z());
  TEST_ASSERT_TRUE(5.0 == code.f());
  TEST_ASSERT_TRUE(tl::nullopt == code.i());
  TEST_ASSERT_TRUE(tl::nullopt == code.j());
  TEST_ASSERT_TRUE(tl::nullopt == code.k());
  TEST_ASSERT_TRUE(tl::nullopt == code.s());
  TEST_ASSERT_TRUE(tl::nullopt == code.p());
  TEST_ASSERT_TRUE(tl::nullopt == code.l());
  TEST_ASSERT_FALSE(code.isMachineCoordinateSystem());

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse(" X50", code));
  TEST_ASSERT_EQUAL(0, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_TRUE(50.0 == code.x());
  TEST_ASSERT_TRUE(tl::nullopt == code.y());
  TEST_ASSERT_TRUE(tl::nullopt == code.z());
  TEST_ASSERT_TRUE(tl::nullopt == code.f());
  TEST_ASSERT_TRUE(tl::nullopt == code.i());
  TEST_ASSERT_TRUE(tl::nullopt == code.j());
  TEST_ASSERT_TRUE(tl::nullopt == code.k());
  TEST_ASSERT_TRUE(tl::nullopt == code.s());
  TEST_ASSERT_TRUE(tl::nullopt == code.p());
  TEST_ASSERT_TRUE(tl::nullopt == code.l());
  TEST_ASSERT_FALSE(code.isMachineCoordinateSystem());
}

void test_parseGCode_G1() {
  GCode code;
  GCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("G1 X10; bob", code));
  TEST_ASSERT_EQUAL(1, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_TRUE(10.0 == code.x());
  TEST_ASSERT_TRUE(tl::nullopt == code.y());
  TEST_ASSERT_TRUE(tl::nullopt == code.z());
  TEST_ASSERT_TRUE(tl::nullopt == code.f());
  TEST_ASSERT_TRUE(tl::nullopt == code.i());
  TEST_ASSERT_TRUE(tl::nullopt == code.j());
  TEST_ASSERT_TRUE(tl::nullopt == code.k());
  TEST_ASSERT_TRUE(tl::nullopt == code.s());
  TEST_ASSERT_TRUE(tl::nullopt == code.p());
  TEST_ASSERT_TRUE(tl::nullopt == code.l());
  TEST_ASSERT_FALSE(code.isMachineCoordinateSystem());

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("G1  Y9  F5 ; bob", code));
  TEST_ASSERT_EQUAL(1, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_TRUE(tl::nullopt == code.x());
  TEST_ASSERT_TRUE(9.0 == code.y());
  TEST_ASSERT_TRUE(tl::nullopt == code.z());
  TEST_ASSERT_TRUE(5.0 == code.f());
  TEST_ASSERT_TRUE(tl::nullopt == code.i());
  TEST_ASSERT_TRUE(tl::nullopt == code.j());
  TEST_ASSERT_TRUE(tl::nullopt == code.k());
  TEST_ASSERT_TRUE(tl::nullopt == code.s());
  TEST_ASSERT_TRUE(tl::nullopt == code.p());
  TEST_ASSERT_TRUE(tl::nullopt == code.l());
  TEST_ASSERT_FALSE(code.isMachineCoordinateSystem());

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse(" X50", code));
  TEST_ASSERT_EQUAL(1, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_TRUE(50.0 == code.x());
  TEST_ASSERT_TRUE(tl::nullopt == code.y());
  TEST_ASSERT_TRUE(tl::nullopt == code.z());
  TEST_ASSERT_TRUE(tl::nullopt == code.f());
  TEST_ASSERT_TRUE(tl::nullopt == code.i());
  TEST_ASSERT_TRUE(tl::nullopt == code.j());
  TEST_ASSERT_TRUE(tl::nullopt == code.k());
  TEST_ASSERT_TRUE(tl::nullopt == code.s());
  TEST_ASSERT_TRUE(tl::nullopt == code.p());
  TEST_ASSERT_TRUE(tl::nullopt == code.l());
  TEST_ASSERT_FALSE(code.isMachineCoordinateSystem());
}

void test_parseGCode_G2() {
  GCode code;
  GCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("G2 X5.5 Y5 I-5 J5 F10", code));
  TEST_ASSERT_EQUAL(2, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_TRUE(5.5 == code.x());
  TEST_ASSERT_TRUE(5 == code.y());
  TEST_ASSERT_TRUE(tl::nullopt == code.z());
  TEST_ASSERT_TRUE(10.0 == code.f());
  TEST_ASSERT_TRUE(-5.0 == code.i());
  TEST_ASSERT_TRUE(5.0 == code.j());
  TEST_ASSERT_TRUE(tl::nullopt == code.k());
  TEST_ASSERT_TRUE(tl::nullopt == code.s());
  TEST_ASSERT_TRUE(tl::nullopt == code.p());
  TEST_ASSERT_TRUE(tl::nullopt == code.l());
  TEST_ASSERT_FALSE(code.isMachineCoordinateSystem());
}

void test_parseGCode_G3() {
  GCode code;
  GCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("G3 Y5.5 Z5 J-5 K5 F10", code));
  TEST_ASSERT_EQUAL(3, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_TRUE(tl::nullopt == code.x());
  TEST_ASSERT_TRUE(5.5 == code.y());
  TEST_ASSERT_TRUE(5.0 == code.z());
  TEST_ASSERT_TRUE(10.0 == code.f());
  TEST_ASSERT_TRUE(tl::nullopt == code.i());
  TEST_ASSERT_TRUE(-5.0 == code.j());
  TEST_ASSERT_TRUE(5.0 == code.k());
  TEST_ASSERT_TRUE(tl::nullopt == code.s());
  TEST_ASSERT_TRUE(tl::nullopt == code.p());
  TEST_ASSERT_TRUE(tl::nullopt == code.l());
  TEST_ASSERT_FALSE(code.isMachineCoordinateSystem());
}

void test_parseGCode_G4() {
  GCode code;
  GCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("G4 S10.25 P10.75", code));
  TEST_ASSERT_EQUAL(4, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_TRUE(tl::nullopt == code.x());
  TEST_ASSERT_TRUE(tl::nullopt == code.y());
  TEST_ASSERT_TRUE(tl::nullopt == code.z());
  TEST_ASSERT_TRUE(tl::nullopt == code.f());
  TEST_ASSERT_TRUE(tl::nullopt == code.i());
  TEST_ASSERT_TRUE(tl::nullopt == code.j());
  TEST_ASSERT_TRUE(tl::nullopt == code.k());
  TEST_ASSERT_TRUE(10.25 == code.s());
  TEST_ASSERT_TRUE(10.75 == code.p());
  TEST_ASSERT_TRUE(tl::nullopt == code.l());
  TEST_ASSERT_FALSE(code.isMachineCoordinateSystem());
}

void test_parseGCode_G10L2() {
  GCode code;
  GCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("G10 L2 P3 X5 Y6", code));
  TEST_ASSERT_EQUAL(10, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_TRUE(5 == code.x());
  TEST_ASSERT_TRUE(6 == code.y());
  TEST_ASSERT_TRUE(tl::nullopt == code.z());
  TEST_ASSERT_TRUE(tl::nullopt == code.f());
  TEST_ASSERT_TRUE(tl::nullopt == code.i());
  TEST_ASSERT_TRUE(tl::nullopt == code.j());
  TEST_ASSERT_TRUE(tl::nullopt == code.k());
  TEST_ASSERT_TRUE(tl::nullopt == code.s());
  TEST_ASSERT_TRUE(3 == code.p());
  TEST_ASSERT_TRUE(2 == code.l());
  TEST_ASSERT_FALSE(code.isMachineCoordinateSystem());
}

void test_parseGCode_G10L20() {
  GCode code;
  GCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("G10 L20 P3 X1", code));
  TEST_ASSERT_EQUAL(10, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_TRUE(1 == code.x());
  TEST_ASSERT_TRUE(tl::nullopt == code.y());
  TEST_ASSERT_TRUE(tl::nullopt == code.z());
  TEST_ASSERT_TRUE(tl::nullopt == code.f());
  TEST_ASSERT_TRUE(tl::nullopt == code.i());
  TEST_ASSERT_TRUE(tl::nullopt == code.j());
  TEST_ASSERT_TRUE(tl::nullopt == code.k());
  TEST_ASSERT_TRUE(tl::nullopt == code.s());
  TEST_ASSERT_TRUE(3 == code.p());
  TEST_ASSERT_TRUE(20 == code.l());
  TEST_ASSERT_FALSE(code.isMachineCoordinateSystem());
}

void test_parseGCode_G17() {
  GCode code;
  GCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("G17", code));
  TEST_ASSERT_EQUAL(17, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_NO_PARAMETERS(code);
}

void test_parseGCode_G18() {
  GCode code;
  GCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("G18", code));
  TEST_ASSERT_EQUAL(18, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_NO_PARAMETERS(code);
}

void test_parseGCode_G19() {
  GCode code;
  GCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("G19", code));
  TEST_ASSERT_EQUAL(19, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_NO_PARAMETERS(code);
}

void test_parseGCode_G20() {
  GCode code;
  GCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("G20 ", code));
  TEST_ASSERT_EQUAL(20, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_NO_PARAMETERS(code);
}

void test_parseGCode_G21() {
  GCode code;
  GCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("G21 ", code));
  TEST_ASSERT_EQUAL(21, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_NO_PARAMETERS(code);
}

void test_parseGCode_G28() {
  GCode code;
  GCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("G28 X1.125", code));
  TEST_ASSERT_EQUAL(28, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_TRUE(1.125 == code.x());
  TEST_ASSERT_TRUE(tl::nullopt == code.y());
  TEST_ASSERT_TRUE(tl::nullopt == code.z());
  TEST_ASSERT_TRUE(tl::nullopt == code.f());
  TEST_ASSERT_TRUE(tl::nullopt == code.i());
  TEST_ASSERT_TRUE(tl::nullopt == code.j());
  TEST_ASSERT_TRUE(tl::nullopt == code.k());
  TEST_ASSERT_TRUE(tl::nullopt == code.s());
  TEST_ASSERT_TRUE(tl::nullopt == code.p());
  TEST_ASSERT_TRUE(tl::nullopt == code.l());
  TEST_ASSERT_FALSE(code.isMachineCoordinateSystem());
}

void test_parseGCode_G28D1() {
  GCode code;
  GCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("G28.1", code));
  TEST_ASSERT_EQUAL(28, code.code());
  TEST_ASSERT_TRUE(1 == code.subcode());
  TEST_ASSERT_NO_PARAMETERS(code);
}

void test_parseGCode_G53() {
  GCode code;
  GCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("G53 G0 X1", code));
  TEST_ASSERT_EQUAL(0, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_TRUE(1 == code.x());
  TEST_ASSERT_TRUE(tl::nullopt == code.y());
  TEST_ASSERT_TRUE(tl::nullopt == code.z());
  TEST_ASSERT_TRUE(tl::nullopt == code.f());
  TEST_ASSERT_TRUE(tl::nullopt == code.i());
  TEST_ASSERT_TRUE(tl::nullopt == code.j());
  TEST_ASSERT_TRUE(tl::nullopt == code.k());
  TEST_ASSERT_TRUE(tl::nullopt == code.s());
  TEST_ASSERT_TRUE(tl::nullopt == code.p());
  TEST_ASSERT_TRUE(tl::nullopt == code.l());
  TEST_ASSERT_TRUE(code.isMachineCoordinateSystem());

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("G1 X2", code));
  TEST_ASSERT_EQUAL(1, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_TRUE(2 == code.x());
  TEST_ASSERT_TRUE(tl::nullopt == code.y());
  TEST_ASSERT_TRUE(tl::nullopt == code.z());
  TEST_ASSERT_TRUE(tl::nullopt == code.f());
  TEST_ASSERT_TRUE(tl::nullopt == code.i());
  TEST_ASSERT_TRUE(tl::nullopt == code.j());
  TEST_ASSERT_TRUE(tl::nullopt == code.k());
  TEST_ASSERT_TRUE(tl::nullopt == code.s());
  TEST_ASSERT_TRUE(tl::nullopt == code.p());
  TEST_ASSERT_TRUE(tl::nullopt == code.l());
  TEST_ASSERT_FALSE(code.isMachineCoordinateSystem());

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("G53 X3", code));
  TEST_ASSERT_EQUAL(1, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_TRUE(3 == code.x());
  TEST_ASSERT_TRUE(tl::nullopt == code.y());
  TEST_ASSERT_TRUE(tl::nullopt == code.z());
  TEST_ASSERT_TRUE(tl::nullopt == code.f());
  TEST_ASSERT_TRUE(tl::nullopt == code.i());
  TEST_ASSERT_TRUE(tl::nullopt == code.j());
  TEST_ASSERT_TRUE(tl::nullopt == code.k());
  TEST_ASSERT_TRUE(tl::nullopt == code.s());
  TEST_ASSERT_TRUE(tl::nullopt == code.p());
  TEST_ASSERT_TRUE(tl::nullopt == code.l());
  TEST_ASSERT_TRUE(code.isMachineCoordinateSystem());
}

void test_parseGCode_G53NextLine() {
  GCode code;
  GCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::NEXT_LINE_NEEDED, parser.parse("G53 ", code));
  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("G0 X1", code));
  TEST_ASSERT_EQUAL(0, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_TRUE(1 == code.x());
  TEST_ASSERT_TRUE(tl::nullopt == code.y());
  TEST_ASSERT_TRUE(tl::nullopt == code.z());
  TEST_ASSERT_TRUE(tl::nullopt == code.f());
  TEST_ASSERT_TRUE(tl::nullopt == code.i());
  TEST_ASSERT_TRUE(tl::nullopt == code.j());
  TEST_ASSERT_TRUE(tl::nullopt == code.k());
  TEST_ASSERT_TRUE(tl::nullopt == code.s());
  TEST_ASSERT_TRUE(tl::nullopt == code.p());
  TEST_ASSERT_TRUE(tl::nullopt == code.l());
  TEST_ASSERT_TRUE(code.isMachineCoordinateSystem());

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("G1 X2", code));
  TEST_ASSERT_EQUAL(1, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_TRUE(2 == code.x());
  TEST_ASSERT_TRUE(tl::nullopt == code.y());
  TEST_ASSERT_TRUE(tl::nullopt == code.z());
  TEST_ASSERT_TRUE(tl::nullopt == code.f());
  TEST_ASSERT_TRUE(tl::nullopt == code.i());
  TEST_ASSERT_TRUE(tl::nullopt == code.j());
  TEST_ASSERT_TRUE(tl::nullopt == code.k());
  TEST_ASSERT_TRUE(tl::nullopt == code.s());
  TEST_ASSERT_TRUE(tl::nullopt == code.p());
  TEST_ASSERT_TRUE(tl::nullopt == code.l());
  TEST_ASSERT_FALSE(code.isMachineCoordinateSystem());

  TEST_ASSERT_EQUAL(ParsingResult::NEXT_LINE_NEEDED, parser.parse("G53 ", code));
  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse(" X3", code));
  TEST_ASSERT_EQUAL(1, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_TRUE(3 == code.x());
  TEST_ASSERT_TRUE(tl::nullopt == code.y());
  TEST_ASSERT_TRUE(tl::nullopt == code.z());
  TEST_ASSERT_TRUE(tl::nullopt == code.f());
  TEST_ASSERT_TRUE(tl::nullopt == code.i());
  TEST_ASSERT_TRUE(tl::nullopt == code.j());
  TEST_ASSERT_TRUE(tl::nullopt == code.k());
  TEST_ASSERT_TRUE(tl::nullopt == code.s());
  TEST_ASSERT_TRUE(tl::nullopt == code.p());
  TEST_ASSERT_TRUE(tl::nullopt == code.l());
  TEST_ASSERT_TRUE(code.isMachineCoordinateSystem());
}

void test_parseGCode_G54() {
  GCode code;
  GCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("G54", code));
  TEST_ASSERT_EQUAL(54, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_NO_PARAMETERS(code);
}

void test_parseGCode_G55() {
  GCode code;
  GCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("G55", code));
  TEST_ASSERT_EQUAL(55, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_NO_PARAMETERS(code);
}

void test_parseGCode_G56() {
  GCode code;
  GCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("G56", code));
  TEST_ASSERT_EQUAL(56, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_NO_PARAMETERS(code);
}

void test_parseGCode_G57() {
  GCode code;
  GCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("G57", code));
  TEST_ASSERT_EQUAL(57, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_NO_PARAMETERS(code);
}

void test_parseGCode_G58() {
  GCode code;
  GCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("G58", code));
  TEST_ASSERT_EQUAL(58, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_NO_PARAMETERS(code);
}

void test_parseGCode_G59() {
  GCode code;
  GCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("G59", code));
  TEST_ASSERT_EQUAL(59, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_NO_PARAMETERS(code);
}

void test_parseGCode_G59D1() {
  GCode code;
  GCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("G59.1", code));
  TEST_ASSERT_EQUAL(59, code.code());
  TEST_ASSERT_TRUE(1 == code.subcode());
  TEST_ASSERT_NO_PARAMETERS(code);
}

void test_parseGCode_G59D2() {
  GCode code;
  GCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("G59.2", code));
  TEST_ASSERT_EQUAL(59, code.code());
  TEST_ASSERT_TRUE(2 == code.subcode());
  TEST_ASSERT_NO_PARAMETERS(code);
}

void test_parseGCode_G59D3() {
  GCode code;
  GCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("G59.3", code));
  TEST_ASSERT_EQUAL(59, code.code());
  TEST_ASSERT_TRUE(3 == code.subcode());
  TEST_ASSERT_NO_PARAMETERS(code);
}

void test_parseGCode_G90() {
  GCode code;
  GCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("G90", code));
  TEST_ASSERT_EQUAL(90, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_NO_PARAMETERS(code);
}

void test_parseGCode_G91() {
  GCode code;
  GCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("G91", code));
  TEST_ASSERT_EQUAL(91, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_NO_PARAMETERS(code);
}

void test_parseGCode_G92() {
  GCode code;
  GCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("G92", code));
  TEST_ASSERT_EQUAL(92, code.code());
  TEST_ASSERT_TRUE(tl::nullopt == code.subcode());
  TEST_ASSERT_NO_PARAMETERS(code);
}

void test_parseGCode_G92D1() {
  GCode code;
  GCodeParser parser;

  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("G92.1", code));
  TEST_ASSERT_EQUAL(92, code.code());
  TEST_ASSERT_TRUE(1 == code.subcode());
  TEST_ASSERT_NO_PARAMETERS(code);
}
