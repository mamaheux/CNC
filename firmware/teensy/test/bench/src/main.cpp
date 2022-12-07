#include "../include/parsing/bench_GCode.h"
#include "../include/parsing/bench_MCode.h"

#include "../include/modules/bench_ArcConverter.h"

#include <Arduino.h>
#include <unity.h>

constexpr uint32_t SETUP_DELAY_MS = 2000;
constexpr uint32_t LOOP_DELAY_MS = 10000;

void setup()
{
    delay(SETUP_DELAY_MS);

    UNITY_BEGIN();
}

void loop()
{
    bench_parseGCode();
    bench_parseMCode();

    bench_ArcConverter_setArc();
    bench_ArcConverter_getNextSegment();

    Serial.println();
    delay(LOOP_DELAY_MS);
}
