#include <cnc/parsing/properties.h>
#include <unity.h>

void test_parsePropertiesLine_comment() {
  constexpr size_t BUFFER_SIZE = 256;
  char key[BUFFER_SIZE];
  char value[BUFFER_SIZE];

  TEST_ASSERT_FALSE(parsePropertiesLine("#blablabla=", key, BUFFER_SIZE, value, BUFFER_SIZE));
}

void test_parsePropertiesLine_empty() {
  constexpr size_t BUFFER_SIZE = 256;
  char key[BUFFER_SIZE];
  char value[BUFFER_SIZE];

  TEST_ASSERT_FALSE(parsePropertiesLine("", key, BUFFER_SIZE, value, BUFFER_SIZE));
}

void test_parsePropertiesLine_valueTrailingSpaceAndComment() {
  constexpr size_t BUFFER_SIZE = 256;
  char key[BUFFER_SIZE];
  char value[BUFFER_SIZE];

  TEST_ASSERT_TRUE(parsePropertiesLine("key0=abc #no trailing space", key, BUFFER_SIZE, value, BUFFER_SIZE));
  TEST_ASSERT_EQUAL_STRING("key0", key);
  TEST_ASSERT_EQUAL_STRING("abc", value);
}

void test_parsePropertiesLine_keyTrailingSpace() {
  constexpr size_t BUFFER_SIZE = 256;
  char key[BUFFER_SIZE];
  char value[BUFFER_SIZE];

  TEST_ASSERT_TRUE(parsePropertiesLine("key1 =abc", key, BUFFER_SIZE, value, BUFFER_SIZE));
  TEST_ASSERT_EQUAL_STRING("key1", key);
  TEST_ASSERT_EQUAL_STRING("abc", value);
}

void test_parsePropertiesLine_valueTrailingSpace() {
  constexpr size_t BUFFER_SIZE = 256;
  char key[BUFFER_SIZE];
  char value[BUFFER_SIZE];

  TEST_ASSERT_TRUE(parsePropertiesLine("key2=abc ", key, BUFFER_SIZE, value, BUFFER_SIZE));
  TEST_ASSERT_EQUAL_STRING("key2", key);
  TEST_ASSERT_EQUAL_STRING("abc", value);
}

void test_parsePropertiesLine_manySpaces() {
  constexpr size_t BUFFER_SIZE = 256;
  char key[BUFFER_SIZE];
  char value[BUFFER_SIZE];

  TEST_ASSERT_TRUE(parsePropertiesLine(" key3 = abc ", key, BUFFER_SIZE, value, BUFFER_SIZE));
  TEST_ASSERT_EQUAL_STRING("key3", key);
  TEST_ASSERT_EQUAL_STRING("abc", value);
}

void test_parsePropertiesLine_commentedValue() {
  constexpr size_t BUFFER_SIZE = 256;
  char key[BUFFER_SIZE];
  char value[BUFFER_SIZE];

  TEST_ASSERT_FALSE(parsePropertiesLine("key4=#bob ", key, BUFFER_SIZE, value, BUFFER_SIZE));
}

void test_parsePropertiesLine_emptyKey() {
  constexpr size_t BUFFER_SIZE = 256;
  char key[BUFFER_SIZE];
  char value[BUFFER_SIZE];

  TEST_ASSERT_FALSE(parsePropertiesLine("=abc", key, BUFFER_SIZE, value, BUFFER_SIZE));
}

void test_parsePropertiesLine_emptyKeyAndLeadingSpace() {
  constexpr size_t BUFFER_SIZE = 256;
  char key[BUFFER_SIZE];
  char value[BUFFER_SIZE];

  TEST_ASSERT_FALSE(parsePropertiesLine(" =abc", key, BUFFER_SIZE, value, BUFFER_SIZE));
}

void test_parsePropertiesLine_emptyValue() {
  constexpr size_t BUFFER_SIZE = 256;
  char key[BUFFER_SIZE];
  char value[BUFFER_SIZE];

  TEST_ASSERT_FALSE(parsePropertiesLine("key6=", key, BUFFER_SIZE, value, BUFFER_SIZE));
}

void test_parsePropertiesLine_emptyValueAndTrailingSpace() {
  constexpr size_t BUFFER_SIZE = 256;
  char key[BUFFER_SIZE];
  char value[BUFFER_SIZE];

  TEST_ASSERT_FALSE(parsePropertiesLine("key6= ", key, BUFFER_SIZE, value, BUFFER_SIZE));
}

void test_parsePropertiesLine_newLine() {
  constexpr size_t BUFFER_SIZE = 256;
  char key[BUFFER_SIZE];
  char value[BUFFER_SIZE];

  TEST_ASSERT_FALSE(parsePropertiesLine("\n", key, BUFFER_SIZE, value, BUFFER_SIZE));
}