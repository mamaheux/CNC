#include <cnc/parsing/GCode.h>

#include <Arduino.h>

constexpr size_t GCODE_LINE_COUNT = 60;
constexpr const char* GCODE_LINES[GCODE_LINE_COUNT] = {
  "G0 X0 Y1 Z2 F50",
  "G1 X0 Y1 Z2 F50",
  "G2 X0 Y1 Z2 I3 J4 F10",
  "G2 X0 Y1 Z2 R10 F10",
  "G3 X0 Y1 Z2 I3 J4 F10",
  "G3 X0 Y1 Z2 R10 F10",
  "G3 P10 S10",
  "G0 X0 Y1 Z2 F50",
  "G1 X0 Y1 Z2 F50",
  "G2 X0 Y1 Z2 I3 J4 F10",
  "G2 X0 Y1 Z2 R10 F10",
  "G3 X0 Y1 Z2 I3 J4 F10",
  "G3 X0 Y1 Z2 R10 F10",
  "G3 P10 S10",
  "G0 X0 Y1 Z2 F50",
  "G1 X0 Y1 Z2 F50",
  "G2 X0 Y1 Z2 I3 J4 F10",
  "G2 X0 Y1 Z2 R10 F10",
  "G3 X0 Y1 Z2 I3 J4 F10",
  "G3 X0 Y1 Z2 R10 F10",
  "G3 P10 S10",
  "G0 X0 Y1 Z2 F50",
  "G1 X0 Y1 Z2 F50",
  "G2 X0 Y1 Z2 I3 J4 F10",
  "G2 X0 Y1 Z2 R10 F10",
  "G3 X0 Y1 Z2 I3 J4 F10",
  "G3 X0 Y1 Z2 R10 F10",
  "G3 P10 S10",
  "G0 X0 Y1 Z2 F50",
  "G1 X0 Y1 Z2 F50",
  "G2 X0 Y1 Z2 I3 J4 F10",
  "G2 X0 Y1 Z2 R10 F10",
  "G3 X0 Y1 Z2 I3 J4 F10",
  "G3 X0 Y1 Z2 R10 F10",
  "G3 P10 S10",
  "G10 L2 P2 X1 Y2 Z3 R10",
  "G10 L20 P2 X1 Y2 Z3",
  "G17",
  "G18",
  "G19",
  "G20",
  "G21",
  "G28",
  "G28.1",
  "G53 G0 X0 Y0 Z0",
  "G54",
  "G55",
  "G56",
  "G57",
  "G58",
  "G59",
  "G59.1",
  "G59.2",
  "G59.3",
  "G90",
  "G90.1",
  "G91",
  "G91.1",
  "G92 X1 Y2 Z3",
  "G92.1"
};

void bench_parseGCode() {
  GCode gcode;
  GCodeParser parser;

  uint32_t startUs = micros();
  for (size_t i = 0; i < GCODE_LINE_COUNT; i++) {
    parser.parse(GCODE_LINES[i], gcode);
  }
  uint32_t endUs = micros();

  Serial.print(__FILE__);
  Serial.print(":");
  Serial.print(__func__);
  Serial.print(", Duration (us): ");
  Serial.println(endUs - startUs);
}
