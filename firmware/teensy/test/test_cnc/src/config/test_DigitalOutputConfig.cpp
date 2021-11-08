#include <cnc/config/DigitalOutputConfig.h>
#include <unity.h>

void test_DigitalOutputConfig_constructor() {
  DigitalOutputConfig config(10, true);

  TEST_ASSERT_EQUAL(10, config.pin());
  TEST_ASSERT_TRUE(config.inverted());
}

void test_DigitalOutputConfig_parse() {
  TEST_ASSERT_TRUE(DigitalOutputConfig::parse("") == tl::nullopt);

  tl::optional<DigitalOutputConfig> config = DigitalOutputConfig::parse("1");
  TEST_ASSERT_TRUE(config.has_value());
  TEST_ASSERT_EQUAL(1, config->pin());
  TEST_ASSERT_FALSE(config->inverted());

  config = DigitalOutputConfig::parse("!20");
  TEST_ASSERT_TRUE(config.has_value());
  TEST_ASSERT_EQUAL(20, config->pin());
  TEST_ASSERT_TRUE(config->inverted());
}
