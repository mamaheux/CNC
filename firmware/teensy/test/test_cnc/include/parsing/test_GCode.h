#ifndef TEST_CNC_PARSING_GCODE_H
#define TEST_CNC_PARSING_GCODE_H

void test_GCode_g1();

void test_parseGCode_empty();
void test_parseGCode_tooBig();
void test_parseGCode_noModalMove();

void test_parseGCode_G0();
void test_parseGCode_G1();
void test_parseGCode_G2();
void test_parseGCode_G3();

void test_parseGCode_G4();

void test_parseGCode_G10L2();
void test_parseGCode_G10L20();

void test_parseGCode_G17();
void test_parseGCode_G18();
void test_parseGCode_G19();
void test_parseGCode_G20();
void test_parseGCode_G21();
void test_parseGCode_G28();
void test_parseGCode_G28D1();

void test_parseGCode_G53();
void test_parseGCode_G53NextLine();

void test_parseGCode_G54();
void test_parseGCode_G55();
void test_parseGCode_G56();
void test_parseGCode_G57();
void test_parseGCode_G58();
void test_parseGCode_G59();
void test_parseGCode_G59D1();
void test_parseGCode_G59D2();
void test_parseGCode_G59D3();

void test_parseGCode_G90();
void test_parseGCode_G91();
void test_parseGCode_G92();
void test_parseGCode_G92D1();

void test_GCode_clear();

#endif
