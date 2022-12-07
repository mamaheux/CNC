#include <cnc/config/AnalogInputConfig.h>
#include <unity.h>

void test_AnalogInputConfig_constructor()
{
    AnalogInputConfig config(10);

    TEST_ASSERT_EQUAL(10, config.pin());
}

void test_AnalogInputConfig_parse()
{
    TEST_ASSERT_TRUE(AnalogInputConfig::parse("") == tl::nullopt);

    tl::optional<AnalogInputConfig> config = AnalogInputConfig::parse("10");
    TEST_ASSERT_TRUE(config.has_value());
    TEST_ASSERT_EQUAL(10, config->pin());
}
