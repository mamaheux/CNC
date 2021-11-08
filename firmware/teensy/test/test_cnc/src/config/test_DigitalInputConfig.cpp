#include <cnc/config/DigitalInputConfig.h>
#include <unity.h>

void test_DigitalInputConfig_constructor() {
  DigitalInputConfig config(10, true, DigitalInputMode::PULLUP);

  TEST_ASSERT_EQUAL(10, config.pin());
  TEST_ASSERT_TRUE(config.inverted());
  TEST_ASSERT_EQUAL(DigitalInputMode::PULLUP, config.mode());
}

void test_DigitalInputConfig_parse() {
  TEST_ASSERT_TRUE(DigitalInputConfig::parse("") == tl::nullopt);

  tl::optional<DigitalInputConfig> config = DigitalInputConfig::parse("1");
  TEST_ASSERT_TRUE(config.has_value());
  TEST_ASSERT_EQUAL(1, config->pin());
  TEST_ASSERT_FALSE(config->inverted());
  TEST_ASSERT_EQUAL(DigitalInputMode::NORMAL, config->mode());

  config = DigitalInputConfig::parse("!2");
  TEST_ASSERT_TRUE(config.has_value());
  TEST_ASSERT_EQUAL(2, config->pin());
  TEST_ASSERT_TRUE(config->inverted());
  TEST_ASSERT_EQUAL(DigitalInputMode::NORMAL, config->mode());

  config = DigitalInputConfig::parse("3v");
  TEST_ASSERT_TRUE(config.has_value());
  TEST_ASSERT_EQUAL(3, config->pin());
  TEST_ASSERT_FALSE(config->inverted());
  TEST_ASSERT_EQUAL(DigitalInputMode::PULLDOWN, config->mode());

  config = DigitalInputConfig::parse("!4v");
  TEST_ASSERT_TRUE(config.has_value());
  TEST_ASSERT_EQUAL(4, config->pin());
  TEST_ASSERT_TRUE(config->inverted());
  TEST_ASSERT_EQUAL(DigitalInputMode::PULLDOWN, config->mode());

  config = DigitalInputConfig::parse("10^");
  TEST_ASSERT_TRUE(config.has_value());
  TEST_ASSERT_EQUAL(10, config->pin());
  TEST_ASSERT_FALSE(config->inverted());
  TEST_ASSERT_EQUAL(DigitalInputMode::PULLUP, config->mode());

  config = DigitalInputConfig::parse("!11^");
  TEST_ASSERT_TRUE(config.has_value());
  TEST_ASSERT_EQUAL(11, config->pin());
  TEST_ASSERT_TRUE(config->inverted());
  TEST_ASSERT_EQUAL(DigitalInputMode::PULLUP, config->mode());
}
