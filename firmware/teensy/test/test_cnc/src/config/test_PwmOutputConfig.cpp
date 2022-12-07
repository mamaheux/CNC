#include <cnc/config/PwmOutputConfig.h>
#include <unity.h>

void test_PwmOutputConfig_constructor()
{
    PwmOutputConfig config(10, true, 1000.f);

    TEST_ASSERT_EQUAL(10, config.pin());
    TEST_ASSERT_TRUE(config.inverted());
    TEST_ASSERT_EQUAL(1000, config.frequency());
}

void test_PwmOutputConfig_parse()
{
    TEST_ASSERT_TRUE(PwmOutputConfig::parse("", 1000.f) == tl::nullopt);

    tl::optional<PwmOutputConfig> config = PwmOutputConfig::parse("1", 2000.f);
    TEST_ASSERT_TRUE(config.has_value());
    TEST_ASSERT_EQUAL(1, config->pin());
    TEST_ASSERT_FALSE(config->inverted());
    TEST_ASSERT_EQUAL(2000.f, config->frequency());

    config = PwmOutputConfig::parse("!20", 3000);
    TEST_ASSERT_TRUE(config.has_value());
    TEST_ASSERT_EQUAL(20, config->pin());
    TEST_ASSERT_TRUE(config->inverted());
    TEST_ASSERT_EQUAL(3000.f, config->frequency());
}
