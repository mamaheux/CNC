#ifndef TEST_CNC_PARSING_MCODE_H
#define TEST_CNC_PARSING_MCODE_H

void test_parseMCode_empty();
void test_parseMCode_tooBig();

void test_parseGCode_M3();
void test_parseGCode_M5();
void test_parseGCode_M17();
void test_parseGCode_M18();
void test_parseGCode_M84();

void test_parseGCode_M20();
void test_parseGCode_M23();
void test_parseGCode_M24();
void test_parseGCode_M25();
void test_parseGCode_M26();
void test_parseGCode_M27();
void test_parseGCode_M28();
void test_parseGCode_M29();
void test_parseGCode_M30();
void test_parseGCode_M32();

void test_parseGCode_M114();
void test_parseGCode_M114D1();
void test_parseGCode_M114D2();
void test_parseGCode_M114D3();

void test_parseGCode_M203();
void test_parseGCode_M204();
void test_parseGCode_M220();

void test_parseGCode_M957();
void test_parseGCode_M958();

#endif
