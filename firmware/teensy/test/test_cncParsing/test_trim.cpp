#include <cncParsing.h>
#include <unity.h>

void test_ltrim_oneSpace() {
    char string[] = { ' ', 'a', 'b', ' ', '\0' };
    ltrim(string);
    TEST_ASSERT_EQUAL_STRING("ab ", string);
}

void test_ltrim_twoSpace() {
    char string[] = { ' ', ' ', 'a', 'b', ' ', '\0' };
    ltrim(string);
    TEST_ASSERT_EQUAL_STRING("ab ", string);
}

void test_ltrim_allSpaceCharacters() {
    char string[] = { ' ', '\t', '\n', 'a', 'b', ' ', '\0' };
    ltrim(string);
    TEST_ASSERT_EQUAL_STRING("ab ", string);
}

void test_ltrim_noSpace() {
    char string[] = { 'a', 'b', '\0' };
    ltrim(string);
    TEST_ASSERT_EQUAL_STRING("ab", string);
}

void test_ltrim_empty() {
    char string[] = { '\0' };
    ltrim(string);
    TEST_ASSERT_EQUAL_STRING("", string);
}

void test_rtrim_oneSpace() {
    char string[] = { ' ', 'a', 'b', ' ', '\0' };
    rtrim(string);
    TEST_ASSERT_EQUAL_STRING(" ab", string);
}

void test_rtrim_twoSpace() {
    char string[] = { ' ', 'a', 'b', ' ', ' ', '\0' };
    rtrim(string);
    TEST_ASSERT_EQUAL_STRING(" ab", string);
}

void test_rtrim_allSpaceCharacters() {
    char string[] = { ' ', 'a', 'b', ' ', '\t', '\n', '\0' };
    rtrim(string);
    TEST_ASSERT_EQUAL_STRING(" ab", string);
}

void test_rtrim_noSpace() {
    char string[] = { 'a', 'b', '\0' };
    rtrim(string);
    TEST_ASSERT_EQUAL_STRING("ab", string);
}

void test_rtrim_empty() {
    char string[] = { '\0' };
    rtrim(string);
    TEST_ASSERT_EQUAL_STRING("", string);
}

void test_trim_oneSpace() {
    char string[] = { ' ', 'a', 'b', ' ', '\0' };
    trim(string);
    TEST_ASSERT_EQUAL_STRING("ab", string);
}

void test_trim_twoSpace() {
    char string[] = { ' ', ' ', 'a', 'b', ' ', ' ', '\0' };
    trim(string);
    TEST_ASSERT_EQUAL_STRING("ab", string);
}

void test_trim_allSpaceCharacters() {
    char string[] = { '\t', 'a', 'b', '\n', ' ', '\0' };
    trim(string);
    TEST_ASSERT_EQUAL_STRING("ab", string);
}

void test_trim_noSpace() {
    char string[] = { 'a', 'b', '\0' };
    trim(string);
    TEST_ASSERT_EQUAL_STRING("ab", string);
}

void test_trim_empty() {
    char string[] = { '\0' };
    trim(string);
    TEST_ASSERT_EQUAL_STRING("", string);
}
