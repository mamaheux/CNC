#ifndef TEST_CNC_PARSING_MCODE_H
#define TEST_CNC_PARSING_MCODE_H

void test_parseMCode_empty();
void test_parseMCode_tooBig();

void test_parseMCode_M3();
void test_parseMCode_M5();
void test_parseMCode_M17();
void test_parseMCode_M18();
void test_parseMCode_M84();

void test_parseMCode_M20();
void test_parseMCode_M23();
void test_parseMCode_M24();
void test_parseMCode_M25();
void test_parseMCode_M26();
void test_parseMCode_M27();
void test_parseMCode_M28();
void test_parseMCode_M29();
void test_parseMCode_M30();
void test_parseMCode_M32();

void test_parseMCode_M114();
void test_parseMCode_M114D1();
void test_parseMCode_M114D2();
void test_parseMCode_M114D3();

void test_parseMCode_M203();
void test_parseMCode_M204();
void test_parseMCode_M220();

void test_parseMCode_M957();
void test_parseMCode_M958();

void test_MCode_clear();
void test_MCode_clearM32();

#endif
