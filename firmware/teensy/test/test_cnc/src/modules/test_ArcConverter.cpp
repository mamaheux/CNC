#include <cnc/modules/ArcConverter.h>
#include <unity.h>

#include <cstring>
#include <vector>

using namespace std;

constexpr float MAX_DELTA = 1e-5;

static GCode toGCode(const char* command)
{
    GCodeParser parser;
    GCode gcode;
    TEST_ASSERT_EQUAL(ParsingResult::OK, parser.parse(command, gcode));
    return gcode;
}

static ConfigItem createMaxErrorInMmConfigItem()
{
    ConfigItem item;
    strcpy(const_cast<char*>(item.getKey()), "arc_converter.max_error_in_mm");
    strcpy(const_cast<char*>(item.getValueString()), "0.1");
    return item;
}

static void assertArc(
    ArcConverter& arcConverter,
    const vector<float>& x,
    const vector<float>& y,
    const vector<float>& z,
    tl::optional<float> f = tl::nullopt,
    tl::optional<float> s = tl::nullopt)
{
    GCode code;
    TEST_ASSERT(x.size() == y.size());
    TEST_ASSERT(x.size() == z.size());

    for (size_t i = 0; i < x.size(); i++)
    {
        TEST_ASSERT_TRUE(arcConverter.getNextSegment(code));
        TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, x[i], *code.x());
        TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, y[i], *code.y());
        TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, z[i], *code.z());
        TEST_ASSERT_TRUE(code.isMachineCoordinateSystem());
        TEST_ASSERT(code.f() == f);
        TEST_ASSERT(code.s() == s);
    }

    TEST_ASSERT_FALSE(arcConverter.getNextSegment(code));
}

void test_getCenterPointFromRadius()
{
    Vector2<double> center = getCenterPointFromRadius(0.0, 0.0, 1.0, 1.0, 1.0, true);
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, center.x);
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.f, center.y);

    center = getCenterPointFromRadius(0.0, 0.0, 1.0, 1.0, -1.0, true);
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.f, center.x);
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, center.y);

    center = getCenterPointFromRadius(1.0, 1.0, 0.0, 0.0, 1.0, true);
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.f, center.x);
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, center.y);

    center = getCenterPointFromRadius(1.0, 1.0, 0.0, 0.0, -1.0, true);
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, center.x);
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.f, center.y);


    center = getCenterPointFromRadius(0.0, 0.0, 1.0, 1.0, 1.0, false);
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.f, center.x);
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, center.y);

    center = getCenterPointFromRadius(0.0, 0.0, 1.0, 1.0, -1.0, false);
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, center.x);
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.f, center.y);

    center = getCenterPointFromRadius(1.0, 1.0, 0.0, 0.0, 1.0, false);
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, center.x);
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.f, center.y);

    center = getCenterPointFromRadius(1.0, 1.0, 0.0, 0.0, -1.0, false);
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.f, center.x);
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, center.y);


    center = getCenterPointFromRadius(0.0, 0.0, 1.0, 1.0, 0.70710678f, true);
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.5f, center.x);
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.5f, center.y);

    center = getCenterPointFromRadius(0.0, 0.0, 1.0, 1.0, -0.70710678f, true);
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.5f, center.x);
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.5f, center.y);

    center = getCenterPointFromRadius(0.0, 0.0, 1.0, 1.0, 0.70710678f, false);
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.5f, center.x);
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.5f, center.y);

    center = getCenterPointFromRadius(0.0, 0.0, 1.0, 1.0, -0.70710678f, false);
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.5f, center.x);
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.5f, center.y);


    center = getCenterPointFromRadius(1.0, 1.0, 0.0, 0.0, 0.70710678f, true);
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.5f, center.x);
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.5f, center.y);

    center = getCenterPointFromRadius(1.0, 1.0, 0.0, 0.0, -0.70710678f, true);
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.5f, center.x);
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.5f, center.y);

    center = getCenterPointFromRadius(1.0, 1.0, 0.0, 0.0, 0.70710678f, false);
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.5f, center.x);
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.5f, center.y);

    center = getCenterPointFromRadius(1.0, 1.0, 0.0, 0.0, -0.70710678f, false);
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.5f, center.x);
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.5f, center.y);
}

void test_ArcConverter_invalidCode()
{
    CoordinateTransformer coordinateTransformer;
    ArcConverter arcConverter(&coordinateTransformer);

    TEST_ASSERT_TRUE(arcConverter.isFinished());
    TEST_ASSERT_EQUAL(CommandResultType::NOT_HANDLED, arcConverter.setArc(toGCode("G0")).type());
    TEST_ASSERT_EQUAL(CommandResultType::NOT_HANDLED, arcConverter.setArc(toGCode("G1")).type());
}

void test_ArcConverter_invalidCenterPoint()
{
    CoordinateTransformer coordinateTransformer;
    ArcConverter arcConverter(&coordinateTransformer);

    // Default plan (XY)
    TEST_ASSERT_EQUAL(CommandResultType::ERROR, arcConverter.setArc(toGCode("G2")).type());
    TEST_ASSERT_TRUE(arcConverter.isFinished());
    TEST_ASSERT_EQUAL(CommandResultType::ERROR, arcConverter.setArc(toGCode("G2 K3")).type());
    TEST_ASSERT_EQUAL(CommandResultType::ERROR, arcConverter.setArc(toGCode("G2 I1 J2 K3")).type());
    TEST_ASSERT_EQUAL(CommandResultType::ERROR, arcConverter.setArc(toGCode("G3")).type());
    TEST_ASSERT_EQUAL(CommandResultType::ERROR, arcConverter.setArc(toGCode("G3 K3")).type());
    TEST_ASSERT_EQUAL(CommandResultType::ERROR, arcConverter.setArc(toGCode("G3 I1 J2 K3")).type());

    // Plan XZ
    TEST_ASSERT_EQUAL(
        CommandResultType::OK,
        arcConverter.onGCodeCommandReceived(toGCode("G18"), CommandSource::SERIAL_SOURCE, 0).type());
    TEST_ASSERT_EQUAL(CommandResultType::ERROR, arcConverter.setArc(toGCode("G2")).type());
    TEST_ASSERT_EQUAL(CommandResultType::ERROR, arcConverter.setArc(toGCode("G2 J3")).type());
    TEST_ASSERT_EQUAL(CommandResultType::ERROR, arcConverter.setArc(toGCode("G2 I1 J2 K3")).type());
    TEST_ASSERT_EQUAL(CommandResultType::ERROR, arcConverter.setArc(toGCode("G3")).type());
    TEST_ASSERT_EQUAL(CommandResultType::ERROR, arcConverter.setArc(toGCode("G3 J3")).type());
    TEST_ASSERT_EQUAL(CommandResultType::ERROR, arcConverter.setArc(toGCode("G3 I1 J2 K3")).type());

    // Plan YZ
    TEST_ASSERT_EQUAL(
        CommandResultType::OK,
        arcConverter.onGCodeCommandReceived(toGCode("G19"), CommandSource::SERIAL_SOURCE, 0).type());
    TEST_ASSERT_EQUAL(CommandResultType::ERROR, arcConverter.setArc(toGCode("G2")).type());
    TEST_ASSERT_EQUAL(CommandResultType::ERROR, arcConverter.setArc(toGCode("G2 I3")).type());
    TEST_ASSERT_EQUAL(CommandResultType::ERROR, arcConverter.setArc(toGCode("G2 I1 J2 K3")).type());
    TEST_ASSERT_EQUAL(CommandResultType::ERROR, arcConverter.setArc(toGCode("G3")).type());
    TEST_ASSERT_EQUAL(CommandResultType::ERROR, arcConverter.setArc(toGCode("G3 I3")).type());
    TEST_ASSERT_EQUAL(CommandResultType::ERROR, arcConverter.setArc(toGCode("G3 I1 J2 K3")).type());

    TEST_ASSERT_EQUAL(CommandResultType::ERROR, arcConverter.setArc(toGCode("G2 R5")).type());
    TEST_ASSERT_EQUAL(CommandResultType::ERROR, arcConverter.setArc(toGCode("G2 X10 R0.1")).type());
    TEST_ASSERT_EQUAL(CommandResultType::ERROR, arcConverter.setArc(toGCode("G3 R5")).type());
    TEST_ASSERT_EQUAL(CommandResultType::ERROR, arcConverter.setArc(toGCode("G3 X10 R0.1")).type());

    // Paln XY
    TEST_ASSERT_EQUAL(
        CommandResultType::OK,
        arcConverter.onGCodeCommandReceived(toGCode("G17"), CommandSource::SERIAL_SOURCE, 0).type());
    TEST_ASSERT_EQUAL(CommandResultType::ERROR, arcConverter.setArc(toGCode("G2")).type());
    TEST_ASSERT_EQUAL(CommandResultType::ERROR, arcConverter.setArc(toGCode("G2 K3")).type());
    TEST_ASSERT_EQUAL(CommandResultType::ERROR, arcConverter.setArc(toGCode("G2 I1 J2 K3")).type());
    TEST_ASSERT_EQUAL(CommandResultType::ERROR, arcConverter.setArc(toGCode("G3")).type());
    TEST_ASSERT_EQUAL(CommandResultType::ERROR, arcConverter.setArc(toGCode("G3 K3")).type());
    TEST_ASSERT_EQUAL(CommandResultType::ERROR, arcConverter.setArc(toGCode("G3 I1 J2 K3")).type());
}

void test_ArcConverter_reallySmallRadius()
{
    CoordinateTransformer coordinateTransformer;
    ArcConverter arcConverter(&coordinateTransformer);

    // The distance between the center point and the start point is not equal
    // to the distance between the center point and the end point.
    arcConverter.configure(createMaxErrorInMmConfigItem());
    arcConverter.onTargetPositionChanged(Vector3<float>(0.f, 0.f, 0.f));
    TEST_ASSERT_EQUAL(CommandResultType::OK, arcConverter.setArc(toGCode("G2 X0.02 Y0.02 I0.01 J0.01")).type());
    TEST_ASSERT_FALSE(arcConverter.isFinished());

    assertArc(arcConverter, {0.02f}, {0.02f}, {0.f});
}

void test_ArcConverter_invalidP()
{
    CoordinateTransformer coordinateTransformer;
    ArcConverter arcConverter(&coordinateTransformer);

    // The distance between the center point and the start point is not equal
    // to the distance between the center point and the end point.
    arcConverter.onTargetPositionChanged(Vector3<float>(0.f, 0.f, 0.f));
    TEST_ASSERT_EQUAL(CommandResultType::ERROR, arcConverter.setArc(toGCode("G2 X1 Y1 I1 P1.01")).type());
    TEST_ASSERT_TRUE(arcConverter.isFinished());
}

void test_ArcConverter_absoluteR()
{
    CoordinateTransformer coordinateTransformer;
    ArcConverter arcConverter(&coordinateTransformer);

    TEST_ASSERT_EQUAL(
        CommandResultType::OK,
        arcConverter.onGCodeCommandReceived(toGCode("G90.1"), CommandSource::SERIAL_SOURCE, 0).type());
    arcConverter.onTargetPositionChanged(Vector3<float>(0.f, 0.f, 0.f));
    TEST_ASSERT_EQUAL(CommandResultType::ERROR, arcConverter.setArc(toGCode("G2 X1 Y1 R1")).type());

    TEST_ASSERT_EQUAL(
        CommandResultType::OK,
        arcConverter.onGCodeCommandReceived(toGCode("G91.1"), CommandSource::SERIAL_SOURCE, 0).type());
    TEST_ASSERT_EQUAL(CommandResultType::OK, arcConverter.setArc(toGCode("G2 X1 Y1 R1")).type());
}

void test_ArcConverter_g2XY()
{
    CoordinateTransformer coordinateTransformer;
    ArcConverter arcConverter(&coordinateTransformer);

    arcConverter.configure(createMaxErrorInMmConfigItem());
    arcConverter.onTargetPositionChanged(Vector3<float>(0.f, 0.f, 0.f));
    TEST_ASSERT_EQUAL(CommandResultType::OK, arcConverter.setArc(toGCode("G2 X1 Y1 I1")).type());
    assertArc(arcConverter, {0.38f, 1.f}, {0.784602f, 1.f}, {0.f, 0.f});

    arcConverter.onTargetPositionChanged(Vector3<float>(1.f, 1.f, 0.f));
    TEST_ASSERT_EQUAL(CommandResultType::OK, arcConverter.setArc(toGCode("G2 X0 Y0 J-1")).type());
    assertArc(
        arcConverter,
        {1.784602f, 1.972906f, 1.421802f, 0.550128f, 0.0203569f, 0.f},
        {0.62f, -0.2312f, -0.906688f, -0.893093f, -0.200747f, 0.f},
        {0.f, 0.f, 0.f, 0.f, 0.f, 0.f});
}

void test_ArcConverter_g3XY()
{
    CoordinateTransformer coordinateTransformer;
    ArcConverter arcConverter(&coordinateTransformer);

    arcConverter.configure(createMaxErrorInMmConfigItem());
    arcConverter.onTargetPositionChanged(Vector3<float>(1.f, 2.f, 0.f));
    TEST_ASSERT_EQUAL(CommandResultType::OK, arcConverter.setArc(toGCode("G3 X2 Y1 I1")).type());
    assertArc(arcConverter, {1.38f, 2.f}, {1.215398f, 1.f}, {0.f, 0.f});

    arcConverter.onTargetPositionChanged(Vector3<float>(2.f, 1.f, 0.f));
    TEST_ASSERT_EQUAL(CommandResultType::OK, arcConverter.setArc(toGCode("G3 X1 Y2 J1")).type());
    assertArc(
        arcConverter,
        {2.784602f, 2.972906f, 2.421801f, 1.550127f, 1.020357f, 1.f},
        {1.38f, 2.2312f, 2.906688f, 2.893093f, 2.200747f, 2.f},
        {0.f, 0.f, 0.f, 0.f, 0.f, 0.f});
}

void test_ArcConverter_helixXY()
{
    CoordinateTransformer coordinateTransformer;
    ArcConverter arcConverter(&coordinateTransformer);

    arcConverter.configure(createMaxErrorInMmConfigItem());
    arcConverter.onTargetPositionChanged(Vector3<float>(0.f, 0.f, 0.f));
    TEST_ASSERT_EQUAL(CommandResultType::OK, arcConverter.setArc(toGCode("G2 X1 Y1 Z2 I1")).type());
    assertArc(arcConverter, {0.38f, 1.f}, {0.7846018, 1.f}, {1.14853f, 2.f});
}

void test_ArcConverter_fullXY()
{
    CoordinateTransformer coordinateTransformer;
    ArcConverter arcConverter(&coordinateTransformer);

    arcConverter.configure(createMaxErrorInMmConfigItem());
    arcConverter.onTargetPositionChanged(Vector3<float>(-1.f, 0.f, 0.f));
    TEST_ASSERT_EQUAL(CommandResultType::OK, arcConverter.setArc(toGCode("G2 I1")).type());
    assertArc(
        arcConverter,
        {-0.62f, 0.2312f, 0.906688f, 0.893093f, 0.200747f, -0.644167f, -1.f},
        {0.784602f, 0.972906f, 0.421802f, -0.449872f, -0.979643f, -0.764885f, 0.f},
        {0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f});
}

void test_ArcConverter_rXY()
{
    CoordinateTransformer coordinateTransformer;
    ArcConverter arcConverter(&coordinateTransformer);

    arcConverter.configure(createMaxErrorInMmConfigItem());
    arcConverter.onTargetPositionChanged(Vector3<float>(-2.f, 0.f, 0.f));
    TEST_ASSERT_EQUAL(CommandResultType::OK, arcConverter.setArc(toGCode("G2 X2 R2")).type());

    assertArc(
        arcConverter,
        {-1.61f, -0.5921f, 0.656719f, 1.64942f, 2.f},
        {1.18655f, 1.91034f, 1.88911f, 1.13112f, 0.f},
        {0.f, 0.f, 0.f, 0.f, 0.f});

    arcConverter.onTargetPositionChanged(Vector3<float>(-4.f, 0.f, 0.f));
    TEST_ASSERT_EQUAL(CommandResultType::OK, arcConverter.setArc(toGCode("G3 X4 R4")).type());
    assertArc(
        arcConverter,
        {-3.605f, -2.49801f, -0.897671f, 0.879962f, 2.4838f, 3.59709f, 3.99996f, 4.f},
        {-1.7332f, -3.12409f, -3.89797f, -3.90201f, -3.1354f, -1.74955f, -0.0181648f, 0.f},
        {0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f});


    arcConverter.onTargetPositionChanged(Vector3<float>(0.f, 0.f, 0.f));
    TEST_ASSERT_EQUAL(CommandResultType::OK, arcConverter.setArc(toGCode("G2 X10 Y10 R10")).type());
    assertArc(
        arcConverter,
        {0.398003f, 1.56033f, 3.39445f, 5.75438f, 8.45227f, 10.f},
        {2.79314f, 5.36395f, 7.50778f, 9.05399f, 9.8795f, 10.f},
        {0.f, 0.f, 0.f, 0.f, 0.f, 0.f});

    arcConverter.onTargetPositionChanged(Vector3<float>(0.f, 0.f, 0.f));
    TEST_ASSERT_EQUAL(CommandResultType::OK, arcConverter.setArc(toGCode("G2 X10 Y10 R-10")).type());
    assertArc(
        arcConverter,
        {-2.79314f,
         -5.36395f,
         -7.50778f,
         -9.05399f,
         -9.8795f,
         -9.9186f,
         -9.16817f,
         -7.68796f,
         -5.59578f,
         -3.05817f,
         -0.277133f,
         2.52596f,
         5.12799f,
         7.32183f,
         8.93285f,
         9.83281f,
         10.f},
        {0.398002f,
         1.56033f,
         3.39445f,
         5.75438f,
         8.45226f,
         11.27335f,
         13.99307f,
         16.39495f,
         18.28778f,
         19.5209f,
         19.99616f,
         19.67572f,
         18.58508f,
         16.81108f,
         14.4949f,
         11.82093f,
         10.f},
        {0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f});


    arcConverter.onTargetPositionChanged(Vector3<float>(1.f, 0.f, 0.f));
    TEST_ASSERT_EQUAL(CommandResultType::OK, arcConverter.setArc(toGCode("G3 X0 Y1 R1")).type());
    assertArc(arcConverter, {0.62f, 0.f}, {0.784602f, 1.f}, {0.f, 0.f});

    arcConverter.onTargetPositionChanged(Vector3<float>(1.f, 0.f, 0.f));
    TEST_ASSERT_EQUAL(CommandResultType::OK, arcConverter.setArc(toGCode("G3 X0 Y1 R-1")).type());
    assertArc(
        arcConverter,
        {1.7846f, 1.97291f, 1.4218f, 0.550127f, 0.0203568f, 0.f},
        {0.38f, 1.2312f, 1.90669f, 1.89309f, 1.20075f, 1.f},
        {0.f, 0.f, 0.f, 0.f, 0.f, 0.f});
}

void test_ArcConverter_p2XY()
{
    CoordinateTransformer coordinateTransformer;
    ArcConverter arcConverter(&coordinateTransformer);

    arcConverter.configure(createMaxErrorInMmConfigItem());
    arcConverter.onTargetPositionChanged(Vector3<float>(-0.5f, 0.f, 0.f));
    TEST_ASSERT_EQUAL(CommandResultType::OK, arcConverter.setArc(toGCode("G2 X0 Y0.5 I0.5 P2")).type());
    assertArc(
        arcConverter,
        {-0.14f, 0.4216f, 0.376096f, -0.210986f, -0.494248f, -0.0657929f, 0.f},
        {0.48f, 0.2688f, -0.329472f, -0.453304f, 0.0756216f, 0.495652f, 0.5f},
        {0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f});

    arcConverter.onTargetPositionChanged(Vector3<float>(-0.f, 0.f, 0.f));
    TEST_ASSERT_EQUAL(CommandResultType::OK, arcConverter.setArc(toGCode("G2 Z1 I2 P2")).type());
    assertArc(
        arcConverter,
        {0.39f,     1.4079f,  2.65672f, 3.64942f, 3.99884f, 3.56872f, 2.5268f,  1.27942f, 0.313072f, 0.00462544f,
         0.474375f, 1.53912f, 2.7836f,  3.72249f, 3.9896f,  3.48077f, 2.39444f, 1.15427f, 0.243947f, 0.f},
        {1.18655f, 1.91034f, 1.88911f, 1.13112f, -0.0680103f, -1.24061f, -1.92937f, -1.86568f, -1.07437f,  0.135942f,
         1.29324f, 1.94617f, 1.8401f,  1.01639f, -0.203717f,  -1.34437f, -1.96072f, -1.81239f, -0.957224f, 0.f},
        {0.0505413f, 0.101083f, 0.151624f, 0.202165f, 0.252707f, 0.303248f, 0.353789f, 0.404331f, 0.454872f, 0.505413f,
         0.555955f,  0.606496f, 0.657037f, 0.707579f, 0.75812f,  0.808661f, 0.859203f, 0.909744f, 0.960285f, 1.f});
}

void test_ArcConverter_offsetAndAbsolute()
{
    CoordinateTransformer coordinateTransformer;
    coordinateTransformer.onGCodeCommandReceived(toGCode("G10 L2 P0 X1 Y1"), CommandSource::SERIAL_SOURCE, 0);
    ArcConverter arcConverter(&coordinateTransformer);

    arcConverter.configure(createMaxErrorInMmConfigItem());
    arcConverter.onTargetPositionChanged(Vector3<float>(0.f, 0.f, 0.f));
    TEST_ASSERT_EQUAL(CommandResultType::OK, arcConverter.setArc(toGCode("G2 X1 I1")).type());
    assertArc(
        arcConverter,
        {0.38f, 1.2312f, 1.90669f, 2.f},
        {0.784602f, 0.972906f, 0.421802f, 0.f},
        {0.f, 0.f, 0.f, 0.f});

    TEST_ASSERT_EQUAL(
        CommandResultType::OK,
        arcConverter.onGCodeCommandReceived(toGCode("G90.1"), CommandSource::SERIAL_SOURCE, 0).type());
    arcConverter.onTargetPositionChanged(Vector3<float>(0.f, 0.f, 0.f));
    TEST_ASSERT_EQUAL(CommandResultType::OK, arcConverter.setArc(toGCode("G2 X1 I0 F10 S5000")).type());
    assertArc(
        arcConverter,
        {0.38f, 1.2312f, 1.90669f, 2.f},
        {0.784602f, 0.972906f, 0.421802f, 0.f},
        {0.f, 0.f, 0.f, 0.f},
        10.f,
        5000.f);
}

void test_ArcConverter_g53()
{
    CoordinateTransformer coordinateTransformer;
    coordinateTransformer.onGCodeCommandReceived(toGCode("G10 L2 P0 X1 Y1"), CommandSource::SERIAL_SOURCE, 0);
    ArcConverter arcConverter(&coordinateTransformer);

    arcConverter.configure(createMaxErrorInMmConfigItem());
    arcConverter.onTargetPositionChanged(Vector3<float>(0.f, 0.f, 0.f));
    TEST_ASSERT_EQUAL(CommandResultType::OK, arcConverter.setArc(toGCode("G53 G2 X1 Y1 I1")).type());
    assertArc(arcConverter, {0.38, 1.f}, {0.784602f, 1.f}, {0.f, 0.f});

    TEST_ASSERT_EQUAL(
        CommandResultType::OK,
        arcConverter.onGCodeCommandReceived(toGCode("G90.1"), CommandSource::SERIAL_SOURCE, 0).type());
    arcConverter.onTargetPositionChanged(Vector3<float>(1.f, 1.f, 0.f));
    TEST_ASSERT_EQUAL(CommandResultType::OK, arcConverter.setArc(toGCode("G53 G3 X0 Y0 I1 J0")).type());
    assertArc(
        arcConverter,
        {
            0.215398f,
            0.f,
        },
        {0.62f, 0.f},
        {0.f, 0.f});
}

void test_ArcConverter_g2XZ()
{
    CoordinateTransformer coordinateTransformer;
    ArcConverter arcConverter(&coordinateTransformer);

    arcConverter.configure(createMaxErrorInMmConfigItem());
    TEST_ASSERT_EQUAL(
        CommandResultType::OK,
        arcConverter.onGCodeCommandReceived(toGCode("G18"), CommandSource::SERIAL_SOURCE, 0).type());

    arcConverter.onTargetPositionChanged(Vector3<float>(0.f, 1.f, 0.f));
    TEST_ASSERT_EQUAL(CommandResultType::OK, arcConverter.setArc(toGCode("G2 X1 Z1 I1")).type());
    assertArc(
        arcConverter,
        {0.38f, 1.2312f, 1.90669f, 1.89309f, 1.20075f, 1.f},
        {1.f, 1.f, 1.f, 1.f, 1.f, 1.f},
        {-0.784602f, -0.972906f, -0.421802f, 0.449872f, 0.979643f, 1.f});
}

void test_ArcConverter_g3XZ()
{
    CoordinateTransformer coordinateTransformer;
    ArcConverter arcConverter(&coordinateTransformer);

    arcConverter.configure(createMaxErrorInMmConfigItem());
    TEST_ASSERT_EQUAL(
        CommandResultType::OK,
        arcConverter.onGCodeCommandReceived(toGCode("G18"), CommandSource::SERIAL_SOURCE, 0).type());

    arcConverter.onTargetPositionChanged(Vector3<float>(-1.f, 1.f, -1.f));
    TEST_ASSERT_EQUAL(CommandResultType::OK, arcConverter.setArc(toGCode("G3 X0 Z0 I1")).type());
    assertArc(arcConverter, {-0.62, 0.f}, {1.f, 1.f}, {-0.215398, 0.f});
}

void test_ArcConverter_g2YZ()
{
    CoordinateTransformer coordinateTransformer;
    ArcConverter arcConverter(&coordinateTransformer);

    arcConverter.configure(createMaxErrorInMmConfigItem());
    TEST_ASSERT_EQUAL(
        CommandResultType::OK,
        arcConverter.onGCodeCommandReceived(toGCode("G19"), CommandSource::SERIAL_SOURCE, 0).type());

    arcConverter.onTargetPositionChanged(Vector3<float>(1.f, -1.f, -1.f));
    TEST_ASSERT_EQUAL(CommandResultType::OK, arcConverter.setArc(toGCode("G2 Y1 Z1 J1 K1")).type());
    assertArc(
        arcConverter,
        {1.f, 1.f, 1.f, 1.f, 1.f},
        {-1.41363f, -1.05586f, -0.121924f, 0.878544f, 1.f},
        {-0.0406861f, 0.94083f, 1.40895f, 1.10822f, 1.f});
}

void test_ArcConverter_g3YZ()
{
    CoordinateTransformer coordinateTransformer;
    ArcConverter arcConverter(&coordinateTransformer);

    arcConverter.configure(createMaxErrorInMmConfigItem());
    TEST_ASSERT_EQUAL(
        CommandResultType::OK,
        arcConverter.onGCodeCommandReceived(toGCode("G19"), CommandSource::SERIAL_SOURCE, 0).type());

    arcConverter.onTargetPositionChanged(Vector3<float>(1.f, 0.f, 0.f));
    TEST_ASSERT_EQUAL(CommandResultType::OK, arcConverter.setArc(toGCode("G3 Y10 Z10 R10")).type());
    assertArc(
        arcConverter,
        {1.f, 1.f, 1.f, 1.f, 1.f, 1.f},
        {2.79314f, 5.36395f, 7.50778f, 9.05399f, 9.8795f, 10.f},
        {0.398002f, 1.56033f, 3.39445f, 5.75438f, 8.45226f, 10.f});
}

void test_ArcConverter_g2Offset()
{
    CoordinateTransformer coordinateTransformer;
    ArcConverter arcConverter(&coordinateTransformer);

    coordinateTransformer.onTargetPositionChanged(Vector3<float>(10.f, 100.f, 5.f));
    arcConverter.onTargetPositionChanged(Vector3<float>(10.f, 100.f, 5.f));

    TEST_ASSERT_EQUAL(
        CommandResultType::OK,
        coordinateTransformer.onGCodeCommandReceived(toGCode("G10 L2 P0 X10 Y100 Z5"), CommandSource::SERIAL_SOURCE, 0)
            .type());

    arcConverter.configure(createMaxErrorInMmConfigItem());
    TEST_ASSERT_EQUAL(CommandResultType::OK, arcConverter.setArc(toGCode("G2 X1 Y1 I1")).type());
    assertArc(arcConverter, {10.38f, 11.f}, {100.784602f, 101.f}, {5.f, 5.f});
}

void test_ArcConverter_g2Rotation()
{
    CoordinateTransformer coordinateTransformer;
    ArcConverter arcConverter(&coordinateTransformer);

    coordinateTransformer.onTargetPositionChanged(Vector3<float>(0.f, 0.f, 0.f));
    arcConverter.onTargetPositionChanged(Vector3<float>(0.f, 0.f, 0.f));

    TEST_ASSERT_EQUAL(
        CommandResultType::OK,
        coordinateTransformer.onGCodeCommandReceived(toGCode("G10 L2 P0 R90"), CommandSource::SERIAL_SOURCE, 0).type());

    arcConverter.configure(createMaxErrorInMmConfigItem());
    TEST_ASSERT_EQUAL(CommandResultType::OK, arcConverter.setArc(toGCode("G2 X1 Y1 I1")).type());
    assertArc(arcConverter, {-0.784602f, -1.f}, {0.38f, 1.f}, {0.f, 0.f});
}

void test_ArcConverter_g2OffsetRotation()
{
    CoordinateTransformer coordinateTransformer;
    ArcConverter arcConverter(&coordinateTransformer);

    coordinateTransformer.onTargetPositionChanged(Vector3<float>(0.f, 0.f, 5.f));
    arcConverter.onTargetPositionChanged(Vector3<float>(0.f, 0.f, 5.f));

    TEST_ASSERT_EQUAL(
        CommandResultType::OK,
        coordinateTransformer.onGCodeCommandReceived(toGCode("G10 L2 P0 R90 Z5"), CommandSource::SERIAL_SOURCE, 0)
            .type());

    arcConverter.configure(createMaxErrorInMmConfigItem());
    TEST_ASSERT_EQUAL(CommandResultType::OK, arcConverter.setArc(toGCode("G2 X1 Y1 I1")).type());
    assertArc(arcConverter, {-0.784602f, -1.f}, {0.38f, 1.f}, {5.f, 5.f});
}

void test_ArcConverter_moveBack()
{
    CoordinateTransformer coordinateTransformer;
    ArcConverter arcConverter(&coordinateTransformer);

    arcConverter.configure(createMaxErrorInMmConfigItem());
    arcConverter.onTargetPositionChanged(Vector3<float>(0.f, 0.f, 0.f));
    TEST_ASSERT_EQUAL(CommandResultType::OK, arcConverter.setArc(toGCode("G2 X1 Y1 Z2 I1")).type());

    GCode code;
    TEST_ASSERT_TRUE(arcConverter.getNextSegment(code));
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.38f, *code.x());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.7846018f, *code.y());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.14853f, *code.z());
    TEST_ASSERT_TRUE(code.isMachineCoordinateSystem());

    arcConverter.moveBack();
    TEST_ASSERT_TRUE(arcConverter.getNextSegment(code));
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.38f, *code.x());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 0.7846018f, *code.y());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.14853f, *code.z());
    TEST_ASSERT_TRUE(code.isMachineCoordinateSystem());

    TEST_ASSERT_TRUE(arcConverter.getNextSegment(code));
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, *code.x());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 1.f, *code.y());
    TEST_ASSERT_FLOAT_WITHIN(MAX_DELTA, 2.f, *code.z());
    TEST_ASSERT_TRUE(code.isMachineCoordinateSystem());
}

void test_ArcConverter_clear()
{
    CoordinateTransformer coordinateTransformer;
    ArcConverter arcConverter(&coordinateTransformer);

    arcConverter.configure(createMaxErrorInMmConfigItem());
    arcConverter.onTargetPositionChanged(Vector3<float>(0.f, 0.f, 0.f));
    TEST_ASSERT_EQUAL(CommandResultType::OK, arcConverter.setArc(toGCode("G2 X1 Y1 I1")).type());

    TEST_ASSERT_FALSE(arcConverter.isFinished());
    arcConverter.clear();
    TEST_ASSERT_TRUE(arcConverter.isFinished());
}
