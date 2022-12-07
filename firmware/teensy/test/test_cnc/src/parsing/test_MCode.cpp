#include <cnc/parsing/MCode.h>
#include <unity.h>

#include <cstring>

using namespace std;

#define TEST_ASSERT_NO_PARAMETERS(code)                                                                                \
    TEST_ASSERT(tl::nullopt == code.x());                                                                              \
    TEST_ASSERT(tl::nullopt == code.y());                                                                              \
    TEST_ASSERT(tl::nullopt == code.z());                                                                              \
    TEST_ASSERT(tl::nullopt == code.s());                                                                              \
    TEST_ASSERT(tl::nullopt == code.p());                                                                              \
    TEST_ASSERT(tl::nullopt == code.i());                                                                              \
    TEST_ASSERT(tl::nullopt == code.d());                                                                              \
    TEST_ASSERT_EQUAL(nullptr, code.path())

bool operator==(const MCode& l, const MCode& r)
{
    return l.code() == r.code() && l.subcode() == r.subcode() && l.x() == r.x() && l.y() == r.y() && l.z() == r.z() &&
           l.s() == r.s() && l.p() == r.p() && l.i() == r.i() && l.d() == r.d();
}

void test_parseMCode_empty()
{
    MCode code;
    MCodeParser parser;

    TEST_ASSERT_EQUAL(ParsingResult::NEXT_LINE_NEEDED, parser.parse("", code));
    TEST_ASSERT_EQUAL(ParsingResult::NEXT_LINE_NEEDED, parser.parse(" ", code));
    TEST_ASSERT_EQUAL(ParsingResult::NEXT_LINE_NEEDED, parser.parse("; comment", code));
    TEST_ASSERT_EQUAL(ParsingResult::NEXT_LINE_NEEDED, parser.parse(" ; comment", code));
}

void test_parseMCode_tooBig()
{
    MCode code;
    MCodeParser parser;

    TEST_ASSERT_EQUAL(
        ParsingResult::ERROR,
        parser.parse(
            "M3                                                                                                        "
            "                                                                                                          "
            "                                                              S5000",
            code));
}

void test_parseGCode_M3()
{
    MCode code1;
    MCode code2;
    MCodeParser parser;

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse(" M3 S5000", code1));
    TEST_ASSERT_EQUAL(3, code1.code());
    TEST_ASSERT(tl::nullopt == code1.subcode());
    TEST_ASSERT(tl::nullopt == code1.x());
    TEST_ASSERT(tl::nullopt == code1.y());
    TEST_ASSERT(tl::nullopt == code1.z());
    TEST_ASSERT(5000 == code1.s());
    TEST_ASSERT(tl::nullopt == code1.p());
    TEST_ASSERT(tl::nullopt == code1.i());
    TEST_ASSERT(tl::nullopt == code1.d());
    TEST_ASSERT_EQUAL(nullptr, code1.path());

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("m3 s5000", code2));
    TEST_ASSERT(code1 == code2);
}

void test_parseGCode_M5()
{
    MCode code1;
    MCode code2;
    MCodeParser parser;

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M5", code1));
    TEST_ASSERT_EQUAL(5, code1.code());
    TEST_ASSERT(tl::nullopt == code1.subcode());
    TEST_ASSERT_NO_PARAMETERS(code1);

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("m5", code2));
    TEST_ASSERT(code1 == code2);
}

void test_parseGCode_M17()
{
    MCode code1;
    MCode code2;
    MCodeParser parser;

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M17", code1));
    TEST_ASSERT_EQUAL(17, code1.code());
    TEST_ASSERT(tl::nullopt == code1.subcode());
    TEST_ASSERT_NO_PARAMETERS(code1);

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("m17", code2));
    TEST_ASSERT(code1 == code2);
}
void test_parseGCode_M18()
{
    MCode code1;
    MCode code2;
    MCodeParser parser;

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M18", code1));
    TEST_ASSERT_EQUAL(18, code1.code());
    TEST_ASSERT(tl::nullopt == code1.subcode());
    TEST_ASSERT_NO_PARAMETERS(code1);

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("m18", code2));
    TEST_ASSERT(code1 == code2);
}

void test_parseGCode_M84()
{
    MCode code1;
    MCode code2;
    MCodeParser parser;

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M84", code1));
    TEST_ASSERT_EQUAL(84, code1.code());
    TEST_ASSERT(tl::nullopt == code1.subcode());
    TEST_ASSERT_NO_PARAMETERS(code1);

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("m84", code2));
    TEST_ASSERT(code1 == code2);
}

void test_parseGCode_M20()
{
    MCode code1;
    MCode code2;
    MCodeParser parser;

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M20", code1));
    TEST_ASSERT_EQUAL(20, code1.code());
    TEST_ASSERT(tl::nullopt == code1.subcode());
    TEST_ASSERT_NO_PARAMETERS(code1);

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("m20", code2));
    TEST_ASSERT(code1 == code2);
}

void test_parseGCode_M23()
{
    MCode code1;
    MCode code2;
    MCodeParser parser;

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M23  path/file.gcode   ", code1));
    TEST_ASSERT_EQUAL(23, code1.code());
    TEST_ASSERT(tl::nullopt == code1.subcode());
    TEST_ASSERT(tl::nullopt == code1.x());
    TEST_ASSERT(tl::nullopt == code1.y());
    TEST_ASSERT(tl::nullopt == code1.z());
    TEST_ASSERT(tl::nullopt == code1.s());
    TEST_ASSERT(tl::nullopt == code1.p());
    TEST_ASSERT(tl::nullopt == code1.i());
    TEST_ASSERT(tl::nullopt == code1.d());
    TEST_ASSERT_EQUAL_STRING("path/file.gcode", code1.path());

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("m23  path/file.gcode   ", code2));
    TEST_ASSERT(code1 == code2);
}

void test_parseGCode_M24()
{
    MCode code1;
    MCode code2;
    MCodeParser parser;

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M24", code1));
    TEST_ASSERT_EQUAL(24, code1.code());
    TEST_ASSERT(tl::nullopt == code1.subcode());
    TEST_ASSERT_NO_PARAMETERS(code1);

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("m24", code2));
    TEST_ASSERT(code1 == code2);
}

void test_parseGCode_M25()
{
    MCode code1;
    MCode code2;
    MCodeParser parser;

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M25", code1));
    TEST_ASSERT_EQUAL(25, code1.code());
    TEST_ASSERT(tl::nullopt == code1.subcode());
    TEST_ASSERT_NO_PARAMETERS(code1);

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("m25", code2));
    TEST_ASSERT(code1 == code2);
}

void test_parseGCode_M26()
{
    MCode code1;
    MCode code2;
    MCodeParser parser;

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M26", code1));
    TEST_ASSERT_EQUAL(26, code1.code());
    TEST_ASSERT(tl::nullopt == code1.subcode());
    TEST_ASSERT_NO_PARAMETERS(code1);

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("m26", code2));
    TEST_ASSERT(code1 == code2);
}

void test_parseGCode_M27()
{
    MCode code1;
    MCode code2;
    MCodeParser parser;

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M27", code1));
    TEST_ASSERT_EQUAL(27, code1.code());
    TEST_ASSERT(tl::nullopt == code1.subcode());
    TEST_ASSERT_NO_PARAMETERS(code1);

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("m27", code2));
    TEST_ASSERT(code1 == code2);
}

void test_parseGCode_M28()
{
    MCode code1;
    MCode code2;
    MCodeParser parser;

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M28 file.gcode ", code1));
    TEST_ASSERT_EQUAL(28, code1.code());
    TEST_ASSERT(tl::nullopt == code1.subcode());
    TEST_ASSERT(tl::nullopt == code1.x());
    TEST_ASSERT(tl::nullopt == code1.y());
    TEST_ASSERT(tl::nullopt == code1.z());
    TEST_ASSERT(tl::nullopt == code1.s());
    TEST_ASSERT(tl::nullopt == code1.p());
    TEST_ASSERT(tl::nullopt == code1.i());
    TEST_ASSERT(tl::nullopt == code1.d());
    TEST_ASSERT_EQUAL_STRING("file.gcode", code1.path());

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("m28 file.gcode ", code2));
    TEST_ASSERT(code1 == code2);
}

void test_parseGCode_M29()
{
    MCode code1;
    MCode code2;
    MCodeParser parser;

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M29", code1));
    TEST_ASSERT_EQUAL(29, code1.code());
    TEST_ASSERT(tl::nullopt == code1.subcode());
    TEST_ASSERT_NO_PARAMETERS(code1);

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("m29", code2));
    TEST_ASSERT(code1 == code2);
}

void test_parseGCode_M30()
{
    MCode code1;
    MCode code2;
    MCodeParser parser;

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M30 a/path/file.gcode", code1));
    TEST_ASSERT_EQUAL(30, code1.code());
    TEST_ASSERT(tl::nullopt == code1.subcode());
    TEST_ASSERT(tl::nullopt == code1.x());
    TEST_ASSERT(tl::nullopt == code1.y());
    TEST_ASSERT(tl::nullopt == code1.z());
    TEST_ASSERT(tl::nullopt == code1.s());
    TEST_ASSERT(tl::nullopt == code1.p());
    TEST_ASSERT(tl::nullopt == code1.i());
    TEST_ASSERT(tl::nullopt == code1.d());
    TEST_ASSERT_EQUAL_STRING("a/path/file.gcode", code1.path());

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("m30 a/path/file.gcode", code2));
    TEST_ASSERT(code1 == code2);
}

void test_parseGCode_M32()
{
    MCode code1;
    MCode code2;
    MCodeParser parser;

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M32 a/long/path/file.gcode  ", code1));
    TEST_ASSERT_EQUAL(32, code1.code());
    TEST_ASSERT(tl::nullopt == code1.subcode());
    TEST_ASSERT(tl::nullopt == code1.x());
    TEST_ASSERT(tl::nullopt == code1.y());
    TEST_ASSERT(tl::nullopt == code1.z());
    TEST_ASSERT(tl::nullopt == code1.s());
    TEST_ASSERT(tl::nullopt == code1.p());
    TEST_ASSERT(tl::nullopt == code1.i());
    TEST_ASSERT(tl::nullopt == code1.d());
    TEST_ASSERT_EQUAL_STRING("a/long/path/file.gcode", code1.path());

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("m32 a/long/path/file.gcode  ", code2));
    TEST_ASSERT(code1 == code2);
}

void test_parseGCode_M114()
{
    MCode code1;
    MCode code2;
    MCodeParser parser;

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M114", code1));
    TEST_ASSERT_EQUAL(114, code1.code());
    TEST_ASSERT(tl::nullopt == code1.subcode());
    TEST_ASSERT_NO_PARAMETERS(code1);

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("m114", code2));
    TEST_ASSERT(code1 == code2);
}

void test_parseGCode_M114D1()
{
    MCode code1;
    MCode code2;
    MCodeParser parser;

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M114.1 ; comment", code1));
    TEST_ASSERT_EQUAL(114, code1.code());
    TEST_ASSERT(1 == code1.subcode());
    TEST_ASSERT_NO_PARAMETERS(code1);

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("m114.1 ; comment", code2));
    TEST_ASSERT(code1 == code2);
}

void test_parseGCode_M114D2()
{
    MCode code1;
    MCode code2;
    MCodeParser parser;

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M114.2; comment", code1));
    TEST_ASSERT_EQUAL(114, code1.code());
    TEST_ASSERT(2 == code1.subcode());
    TEST_ASSERT_NO_PARAMETERS(code1);

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("m114.2 ; comment", code2));
    TEST_ASSERT(code1 == code2);
}

void test_parseGCode_M114D3()
{
    MCode code1;
    MCode code2;
    MCodeParser parser;

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M114.3", code1));
    TEST_ASSERT_EQUAL(114, code1.code());
    TEST_ASSERT(3 == code1.subcode());
    TEST_ASSERT_NO_PARAMETERS(code1);

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("m114.3 ; comment", code2));
    TEST_ASSERT(code1 == code2);
}

void test_parseGCode_M203()
{
    MCode code1;
    MCode code2;
    MCodeParser parser;

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M203 X1.25 Y1.5 Z1.75", code1));
    TEST_ASSERT_EQUAL(203, code1.code());
    TEST_ASSERT(tl::nullopt == code1.subcode());
    TEST_ASSERT(1.25 == code1.x());
    TEST_ASSERT(1.5 == code1.y());
    TEST_ASSERT(1.75 == code1.z());
    TEST_ASSERT(tl::nullopt == code1.s());
    TEST_ASSERT(tl::nullopt == code1.p());
    TEST_ASSERT(tl::nullopt == code1.i());
    TEST_ASSERT(tl::nullopt == code1.d());
    TEST_ASSERT_EQUAL(nullptr, code1.path());

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("m203 x1.25 y1.5 z1.75", code2));
    TEST_ASSERT(code1 == code2);
}

void test_parseGCode_M204()
{
    MCode code1;
    MCode code2;
    MCodeParser parser;

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M204 X1.25 Y1.5 Z1.75", code1));
    TEST_ASSERT_EQUAL(204, code1.code());
    TEST_ASSERT(tl::nullopt == code1.subcode());
    TEST_ASSERT(1.25 == code1.x());
    TEST_ASSERT(1.5 == code1.y());
    TEST_ASSERT(1.75 == code1.z());
    TEST_ASSERT(tl::nullopt == code1.s());
    TEST_ASSERT(tl::nullopt == code1.p());
    TEST_ASSERT(tl::nullopt == code1.i());
    TEST_ASSERT(tl::nullopt == code1.d());
    TEST_ASSERT_EQUAL(nullptr, code1.path());

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("m204 x1.25 y1.5 z1.75", code2));
    TEST_ASSERT(code1 == code2);
}

void test_parseGCode_M220()
{
    MCode code1;
    MCode code2;
    MCodeParser parser;

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M220 S25", code1));
    TEST_ASSERT_EQUAL(220, code1.code());
    TEST_ASSERT(tl::nullopt == code1.subcode());
    TEST_ASSERT(tl::nullopt == code1.x());
    TEST_ASSERT(tl::nullopt == code1.y());
    TEST_ASSERT(tl::nullopt == code1.z());
    TEST_ASSERT(25 == code1.s());
    TEST_ASSERT(tl::nullopt == code1.p());
    TEST_ASSERT(tl::nullopt == code1.i());
    TEST_ASSERT(tl::nullopt == code1.d());
    TEST_ASSERT_EQUAL(nullptr, code1.path());

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("m220 s25", code2));
    TEST_ASSERT(code1 == code2);
}

void test_parseGCode_M957()
{
    MCode code1;
    MCode code2;
    MCodeParser parser;

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M957", code1));
    TEST_ASSERT_EQUAL(957, code1.code());
    TEST_ASSERT(tl::nullopt == code1.subcode());
    TEST_ASSERT_NO_PARAMETERS(code1);

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("m957", code2));
    TEST_ASSERT(code1 == code2);
}

void test_parseGCode_M958()
{
    MCode code1;
    MCode code2;
    MCodeParser parser;

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("M958 P0.75 I0.5 D0.25", code1));
    TEST_ASSERT_EQUAL(958, code1.code());
    TEST_ASSERT(tl::nullopt == code1.subcode());
    TEST_ASSERT(tl::nullopt == code1.x());
    TEST_ASSERT(tl::nullopt == code1.y());
    TEST_ASSERT(tl::nullopt == code1.z());
    TEST_ASSERT(tl::nullopt == code1.s());
    TEST_ASSERT(0.75 == code1.p());
    TEST_ASSERT(0.5 == code1.i());
    TEST_ASSERT(0.25 == code1.d());
    TEST_ASSERT_EQUAL(nullptr, code1.path());

    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse("m958 p0.75 i0.5 d0.25", code2));
    TEST_ASSERT(code1 == code2);
}
