#include <cnc/modules/CoordinateTransformer.h>
#include <unity.h>

constexpr float MAX_DELTA = 1e-6;

static GCode toGCode(const char* command) {
  GCodeParser parser;
  GCode gcode;
  TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse(command, gcode));
  return gcode;
}

void test_CoordinateTransformer_G10L2P10() {
  CoordinateTransformer transformer;

  TEST_ASSERT_EQUAL(CommandResultType::ERROR,
      transformer.onGCodeCommandReceived(toGCode("G10 L2 P10"), CommandSource::SERIAL_SOURCE, 0).type());
}

void test_CoordinateTransformer_G10L2() {
  CoordinateTransformer transformer;
  Vector3<float> r;

  TEST_ASSERT_EQUAL(CommandResultType::OK,
      transformer.onGCodeCommandReceived(toGCode("G55"), CommandSource::SERIAL_SOURCE, 0).type());

  TEST_ASSERT_EQUAL(CommandResultType::OK,
      transformer.onGCodeCommandReceived(toGCode("G10 L2 P0 X10 Y20 Z30 R90"), CommandSource::SERIAL_SOURCE, 0).type());

  r = transformer.gcodeCoordinateToMachineCoordinate(Vector3<float>(1.f, 2.f, 0.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 8.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 21.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 30.f, r.z);

  r = transformer.machineCoordinateToGcode(Vector3<float>(8.f, 21.f, 30.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.f, r.z);

  r = transformer.machineCoordinateToUserCurrentCoordinate(Vector3<float>(8.f, 21.f, 30.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.f, r.z);

  TEST_ASSERT_EQUAL(CommandResultType::OK,
      transformer.onGCodeCommandReceived(toGCode("G54"), CommandSource::SERIAL_SOURCE, 0).type());
  r = transformer.gcodeCoordinateToMachineCoordinate(Vector3<float>(1.f, 2.f, 0.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.f, r.z);

  r = transformer.machineCoordinateToGcode(Vector3<float>(1.f, 2.f, 3.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 3.f, r.z);

  r = transformer.machineCoordinateToUserCurrentCoordinate(Vector3<float>(1.f, 2.f, 3.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 3.f, r.z);

  TEST_ASSERT_EQUAL(CommandResultType::OK,
      transformer.onGCodeCommandReceived(toGCode("G10 L2 P1 X1"), CommandSource::SERIAL_SOURCE, 0).type());
  TEST_ASSERT_EQUAL(CommandResultType::OK,
      transformer.onGCodeCommandReceived(toGCode("G10 L2 P2 Y1"), CommandSource::SERIAL_SOURCE, 0).type());
  TEST_ASSERT_EQUAL(CommandResultType::OK,
      transformer.onGCodeCommandReceived(toGCode("G10 L2 P3 Z1"), CommandSource::SERIAL_SOURCE, 0).type());
  TEST_ASSERT_EQUAL(CommandResultType::OK,
      transformer.onGCodeCommandReceived(toGCode("G10 L2 P4 X2"), CommandSource::SERIAL_SOURCE, 0).type());
  TEST_ASSERT_EQUAL(CommandResultType::OK,
      transformer.onGCodeCommandReceived(toGCode("G10 L2 P5 Y2"), CommandSource::SERIAL_SOURCE, 0).type());
  TEST_ASSERT_EQUAL(CommandResultType::OK,
      transformer.onGCodeCommandReceived(toGCode("G10 L2 P6 Z2"), CommandSource::SERIAL_SOURCE, 0).type());
  TEST_ASSERT_EQUAL(CommandResultType::OK,
      transformer.onGCodeCommandReceived(toGCode("G10 L2 P7 X3"), CommandSource::SERIAL_SOURCE, 0).type());
  TEST_ASSERT_EQUAL(CommandResultType::OK,
      transformer.onGCodeCommandReceived(toGCode("G10 L2 P8 Y3"), CommandSource::SERIAL_SOURCE, 0).type());

  // Inch for P9
  TEST_ASSERT_EQUAL(CommandResultType::OK,
      transformer.onGCodeCommandReceived(toGCode("G20"), CommandSource::SERIAL_SOURCE, 0).type());
  TEST_ASSERT_EQUAL(CommandResultType::OK,
      transformer.onGCodeCommandReceived(toGCode("G10 L2 P9 Z1"), CommandSource::SERIAL_SOURCE, 0).type());
  TEST_ASSERT_EQUAL(CommandResultType::OK,
      transformer.onGCodeCommandReceived(toGCode("G21"), CommandSource::SERIAL_SOURCE, 0).type());

  TEST_ASSERT_EQUAL(CommandResultType::OK,
      transformer.onGCodeCommandReceived(toGCode("G54"), CommandSource::SERIAL_SOURCE, 0).type());
  r = transformer.gcodeCoordinateToMachineCoordinate(Vector3<float>(1.f, 2.f, 3.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 3.f, r.z);

  TEST_ASSERT_EQUAL(CommandResultType::OK,
      transformer.onGCodeCommandReceived(toGCode("G55"), CommandSource::SERIAL_SOURCE, 0).type());
  r = transformer.gcodeCoordinateToMachineCoordinate(Vector3<float>(1.f, 2.f, 3.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 8.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 33.f, r.z);

  TEST_ASSERT_EQUAL(CommandResultType::OK,
      transformer.onGCodeCommandReceived(toGCode("G56"), CommandSource::SERIAL_SOURCE, 0).type());
  r = transformer.gcodeCoordinateToMachineCoordinate(Vector3<float>(1.f, 2.f, 3.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 4.f, r.z);

  TEST_ASSERT_EQUAL(CommandResultType::OK,
      transformer.onGCodeCommandReceived(toGCode("G57"), CommandSource::SERIAL_SOURCE, 0).type());
  r = transformer.gcodeCoordinateToMachineCoordinate(Vector3<float>(1.f, 2.f, 3.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 3.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 3.f, r.z);

  TEST_ASSERT_EQUAL(CommandResultType::OK,
      transformer.onGCodeCommandReceived(toGCode("G58"), CommandSource::SERIAL_SOURCE, 0).type());
  r = transformer.gcodeCoordinateToMachineCoordinate(Vector3<float>(1.f, 2.f, 3.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 4.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 3.f, r.z);

  TEST_ASSERT_EQUAL(CommandResultType::OK,
      transformer.onGCodeCommandReceived(toGCode("G59"), CommandSource::SERIAL_SOURCE, 0).type());
  r = transformer.gcodeCoordinateToMachineCoordinate(Vector3<float>(1.f, 2.f, 3.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 5.f, r.z);

  TEST_ASSERT_EQUAL(CommandResultType::OK,
      transformer.onGCodeCommandReceived(toGCode("G59.1"), CommandSource::SERIAL_SOURCE, 0).type());
  r = transformer.gcodeCoordinateToMachineCoordinate(Vector3<float>(1.f, 2.f, 3.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 4.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 3.f, r.z);

  TEST_ASSERT_EQUAL(CommandResultType::OK,
      transformer.onGCodeCommandReceived(toGCode("G59.2"), CommandSource::SERIAL_SOURCE, 0).type());
  r = transformer.gcodeCoordinateToMachineCoordinate(Vector3<float>(1.f, 2.f, 3.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 5.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 3.f, r.z);

  TEST_ASSERT_EQUAL(CommandResultType::OK,
      transformer.onGCodeCommandReceived(toGCode("G59.3"), CommandSource::SERIAL_SOURCE, 0).type());
  r = transformer.gcodeCoordinateToMachineCoordinate(Vector3<float>(1.f, 2.f, 3.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 28.4f, r.z);
}

void test_CoordinateTransformer_G10L20() {
  CoordinateTransformer transformer;
  Vector3<float> r;

  transformer.onTargetPositionChanged(Vector3<float>(2.f, 3.f, 4.f));

  TEST_ASSERT_EQUAL(CommandResultType::OK,
      transformer.onGCodeCommandReceived(toGCode("G10 L20 P1 X1 Y2"), CommandSource::SERIAL_SOURCE, 0).type());

  TEST_ASSERT_EQUAL(CommandResultType::OK,
      transformer.onGCodeCommandReceived(toGCode("G20"), CommandSource::SERIAL_SOURCE, 0).type());
  TEST_ASSERT_EQUAL(CommandResultType::OK,
      transformer.onGCodeCommandReceived(toGCode("G10 L20 P2 Z1"), CommandSource::SERIAL_SOURCE, 0).type());
  TEST_ASSERT_EQUAL(CommandResultType::OK,
      transformer.onGCodeCommandReceived(toGCode("G21"), CommandSource::SERIAL_SOURCE, 0).type());

  r = transformer.gcodeCoordinateToMachineCoordinate(Vector3<float>(1.f, 2.f, 3.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 3.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 3.f, r.z);

  TEST_ASSERT_EQUAL(CommandResultType::OK,
      transformer.onGCodeCommandReceived(toGCode("G55"), CommandSource::SERIAL_SOURCE, 0).type());
  r = transformer.gcodeCoordinateToMachineCoordinate(Vector3<float>(1.f, 2.f, 3.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, -18.4f, r.z);
}

void test_CoordinateTransformer_G20G21() {
  CoordinateTransformer transformer;
  Vector3<float> r;

  // Default mm
  r = transformer.gcodeCoordinateToMachineCoordinate(Vector3<float>(1.f, 2.f, 3.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 3.f, r.z);

  r = transformer.machineCoordinateToGcode(Vector3<float>(1.f, 2.f, 3.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 3.f, r.z);

  r = transformer.machineCoordinateToUserCurrentCoordinate(Vector3<float>(1.f, 2.f, 3.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 3.f, r.z);

  r = transformer.machineCoordinateToUserMachineCoordinate(Vector3<float>(1.f, 2.f, 3.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 3.f, r.z);

  // Inch
  TEST_ASSERT_EQUAL(CommandResultType::OK,
      transformer.onGCodeCommandReceived(toGCode("G20"), CommandSource::SERIAL_SOURCE, 0).type());
  r = transformer.gcodeCoordinateToMachineCoordinate(Vector3<float>(1.f, 2.f, 3.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 25.4f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 50.8f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 76.2f, r.z);

  r = transformer.machineCoordinateToGcode(Vector3<float>(25.4f, 50.8f, 76.2f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 3.f, r.z);

  r = transformer.machineCoordinateToUserCurrentCoordinate(Vector3<float>(25.4f, 50.8f, 76.2f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 3.f, r.z);

  r = transformer.machineCoordinateToUserMachineCoordinate(Vector3<float>(25.4f, 50.8f, 76.2f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 3.f, r.z);

  // MM
  TEST_ASSERT_EQUAL(CommandResultType::OK,
      transformer.onGCodeCommandReceived(toGCode("G21"), CommandSource::SERIAL_SOURCE, 0).type());
  r = transformer.gcodeCoordinateToMachineCoordinate(Vector3<float>(1.f, 2.f, 3.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 3.f, r.z);

  r = transformer.machineCoordinateToGcode(Vector3<float>(1.f, 2.f, 3.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 3.f, r.z);

  r = transformer.machineCoordinateToUserCurrentCoordinate(Vector3<float>(1.f, 2.f, 3.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 3.f, r.z);

  r = transformer.machineCoordinateToUserMachineCoordinate(Vector3<float>(1.f, 2.f, 3.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 3.f, r.z);
}

void test_CoordinateTransformer_G90G91() {
  CoordinateTransformer transformer;
  Vector3<float> r;

  transformer.onTargetPositionChanged(Vector3<float>(2.f, 3.f, 4.f));

  TEST_ASSERT_EQUAL(CommandResultType::OK,
      transformer.onGCodeCommandReceived(toGCode("G91"), CommandSource::SERIAL_SOURCE, 0).type());
  r = transformer.gcodeCoordinateToMachineCoordinate(Vector3<float>(1.f, 2.f, 3.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 3.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 5.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 7.f, r.z);

  r = transformer.machineCoordinateToGcode(Vector3<float>(3.f, 5.f, 7.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 3.f, r.z);


  TEST_ASSERT_EQUAL(CommandResultType::OK,
      transformer.onGCodeCommandReceived(toGCode("G90"), CommandSource::SERIAL_SOURCE, 0).type());
  r = transformer.gcodeCoordinateToMachineCoordinate(Vector3<float>(1.f, 2.f, 3.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 3.f, r.z);

  r = transformer.machineCoordinateToGcode(Vector3<float>(1.f, 2.f, 3.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 3.f, r.z);
}

void test_CoordinateTransformer_G92() {
  CoordinateTransformer transformer;
  Vector3<float> r;

  transformer.onTargetPositionChanged(Vector3<float>(1.f, 2.f, 3.f));

  TEST_ASSERT_EQUAL(CommandResultType::OK,
      transformer.onGCodeCommandReceived(toGCode("G92 X25.4"), CommandSource::SERIAL_SOURCE, 0).type());
  r = transformer.gcodeCoordinateToMachineCoordinate(Vector3<float>(1.f, 2.f, 3.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, -23.4f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 3.f, r.z);

  r = transformer.machineCoordinateToGcode(Vector3<float>(-23.4f, 2.f, 3.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 3.f, r.z);

  r = transformer.machineCoordinateToUserCurrentCoordinate(Vector3<float>(-23.4f, 2.f, 3.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 3.f, r.z);


  TEST_ASSERT_EQUAL(CommandResultType::OK,
      transformer.onGCodeCommandReceived(toGCode("G20"), CommandSource::SERIAL_SOURCE, 0).type());
  TEST_ASSERT_EQUAL(CommandResultType::OK,
      transformer.onGCodeCommandReceived(toGCode("G92 Y1"), CommandSource::SERIAL_SOURCE, 0).type());
  TEST_ASSERT_EQUAL(CommandResultType::OK,
      transformer.onGCodeCommandReceived(toGCode("G21"), CommandSource::SERIAL_SOURCE, 0).type());
  r = transformer.gcodeCoordinateToMachineCoordinate(Vector3<float>(1.f, 2.f, 3.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, -21.4f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 3.f, r.z);

  r = transformer.machineCoordinateToGcode(Vector3<float>(1.f, -21.4f, 3.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 3.f, r.z);

  r = transformer.machineCoordinateToUserCurrentCoordinate(Vector3<float>(1.f, -21.4f, 3.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 3.f, r.z);


  TEST_ASSERT_EQUAL(CommandResultType::OK,
      transformer.onGCodeCommandReceived(toGCode("G92 Z-1"), CommandSource::SERIAL_SOURCE, 0).type());
  r = transformer.gcodeCoordinateToMachineCoordinate(Vector3<float>(1.f, 2.f, 3.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 7.f, r.z);

  r = transformer.machineCoordinateToGcode(Vector3<float>(1.f, 2.f, 7.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 3.f, r.z);

  r = transformer.machineCoordinateToUserCurrentCoordinate(Vector3<float>(1.f, 2.f, 7.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 3.f, r.z);


  TEST_ASSERT_EQUAL(CommandResultType::OK,
      transformer.onGCodeCommandReceived(toGCode("G92.1"), CommandSource::SERIAL_SOURCE, 0).type());
  r = transformer.gcodeCoordinateToMachineCoordinate(Vector3<float>(1.f, 2.f, 3.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 3.f, r.z);

  r = transformer.machineCoordinateToGcode(Vector3<float>(1.f, 2.f, 3.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 3.f, r.z);

  r = transformer.machineCoordinateToUserCurrentCoordinate(Vector3<float>(1.f, 2.f, 3.f));
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, r.x);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, r.y);
  TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 3.f, r.z);
}
