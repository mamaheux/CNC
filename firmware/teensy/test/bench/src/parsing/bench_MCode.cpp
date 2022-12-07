#include <cnc/parsing/MCode.h>

#include <Arduino.h>

constexpr size_t MCODE_LINE_COUNT = 60;
constexpr const char* MCODE_LINES[MCODE_LINE_COUNT] = {
    "M3 S500",
    "M5",
    "M17",
    "M18",
    "M20",
    "M23 file/path/asd.gcode",
    "M24",
    "M26",
    "M27",
    "M28 file/path/asd.gcode",
    "M29",
    "M30 file/path/asd.gcode",
    "M32 file/path/asd.gcode",
    "M114",
    "M114.1",
    "M114.2",
    "M114.3",
    "M203 X100 Y200 Z300",
    "M204 X100 Y200 Z300",
    "M220 S25",
    "M957",
    "M958 P0.001 I0.002 D0.00005",
    "M3 S500",
    "M5",
    "M17",
    "M18",
    "M20",
    "M23 file/path/asd.gcode",
    "M24",
    "M26",
    "M27",
    "M28 file/path/asd.gcode",
    "M29",
    "M30 file/path/asd.gcode",
    "M32 file/path/asd.gcode",
    "M114",
    "M114.1",
    "M114.2",
    "M114.3",
    "M203 X100 Y200 Z300",
    "M204 X100 Y200 Z300",
    "M220 S25",
    "M957",
    "M958 P0.001 I0.002 D0.00005",
    "M3 S500",
    "M5",
    "M17",
    "M18",
    "M20",
    "M23 file/path/asd.gcode",
    "M24",
    "M26",
    "M27",
    "M28 file/path/asd.gcode",
    "M29",
    "M30 file/path/asd.gcode",
    "M32 file/path/asd.gcode",
    "M114",
    "M114.1",
    "M114.2"};

void bench_parseMCode()
{
    MCode mcode;
    MCodeParser parser;

    uint32_t startUs = micros();
    for (size_t i = 0; i < MCODE_LINE_COUNT; i++)
    {
        parser.parse(MCODE_LINES[i], mcode);
    }
    uint32_t endUs = micros();

    Serial.print(__FILE__);
    Serial.print(":");
    Serial.print(__func__);
    Serial.print(", Duration (us): ");
    Serial.println(endUs - startUs);
}
