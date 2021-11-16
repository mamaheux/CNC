#include <cnc/modules/ArcConverter.h>

#include <Arduino.h>

static GCode toGCode(const char* command) {
  GCodeParser parser;
  GCode gcode;
  parser.parse(command, gcode);
  return gcode;
}

void bench_ArcConverter_setArc() {
  constexpr size_t COUNT = 1000;
  GCode arcI = toGCode("G3 X10 Y10 I10");
  GCode arcR = toGCode("G2 X10 Y10 R10");

  CoordinateTransformer coordinateTransformer;
  ArcConverter arcConverter(&coordinateTransformer);

  uint32_t startIUs = micros();
  for (size_t i = 0; i < COUNT; i++) {
    arcConverter.setArc(arcI);
  }
  uint32_t endIUs = micros();

  uint32_t startRUs = micros();
  for (size_t i = 0; i < COUNT; i++) {
    arcConverter.setArc(arcR);
  }
  uint32_t endRUs = micros();

  Serial.print(__FILE__);
  Serial.print(":");
  Serial.print(__func__);
  Serial.print(", Duration I (us): ");
  Serial.print(endIUs - startIUs);
  Serial.print(", Duration R (us): ");
  Serial.println(endRUs - startRUs);
}

void bench_ArcConverter_getNextSegment() {
  GCode arc = toGCode("G3 X100 Y100 I100");

  CoordinateTransformer coordinateTransformer;
  ArcConverter arcConverter(&coordinateTransformer);
  arcConverter.setArc(arc);

  size_t segmentCount = 0;
  GCode segment;
  uint32_t startUs = micros();
  while (arcConverter.getNextSegment(segment)) {
    segmentCount++;
  }
  uint32_t endUs = micros();

  Serial.print(__FILE__);
  Serial.print(":");
  Serial.print(__func__);
  Serial.print(", Segment count: ");
  Serial.print(segmentCount);
  Serial.print(", Duration I (us): ");
  Serial.println(endUs - startUs);
}
