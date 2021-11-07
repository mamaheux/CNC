#ifndef TEST_CNC_PARSING_PROPERTIES_H
#define TEST_CNC_PARSING_PROPERTIES_H

void test_parsePropertiesLine_comment();
void test_parsePropertiesLine_empty();
void test_parsePropertiesLine_valueTrailingSpaceAndComment();
void test_parsePropertiesLine_keyTrailingSpace();
void test_parsePropertiesLine_valueTrailingSpace();
void test_parsePropertiesLine_manySpaces();
void test_parsePropertiesLine_commentedValue();
void test_parsePropertiesLine_emptyKey();
void test_parsePropertiesLine_emptyKeyAndLeadingSpace();
void test_parsePropertiesLine_emptyValue();
void test_parsePropertiesLine_emptyValueAndTrailingSpace();
void test_parsePropertiesLine_newLine();

#endif
