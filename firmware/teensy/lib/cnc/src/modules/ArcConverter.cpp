#include <cnc/modules/ArcConverter.h>
#include <cnc/modules/ModuleKernel.h>
#include <cnc/space.h>

#include <cmath>
#include <cstring>

using namespace std;

constexpr const char* MAX_ERROR_KEY = "arc_converter.max_error_in_mm";

constexpr const char* CENTER_POINT_ERROR_COMMAND_ERROR_MESSAGE = "Unable to set the center of the arc.";
constexpr const char* RADIUS_ERROR_COMMAND_ERROR_MESSAGE = "The radiuses are not equals.";
constexpr const char* SEGMENT_ERROR_COMMAND_ERROR_MESSAGE = "Segments calculation failed.";

constexpr float RADIUS_TOLERANCE = 1e-1;
constexpr float MINIMUM_ARC_ANGLE = 1e-7;

constexpr float PI_VALUE = 3.1415926535897932384626433832795f;

FLASHMEM ArcConverter::ArcConverter(CoordinateTransformer* coordinateTransformer)
    : m_startOtherAxis(0.f),
      m_endOtherAxis(0.f),
      m_currentOtherAxis(0.f),
      m_otherAxisStep(0.f),
      m_radius(0.f),
      m_currentAngle(0.f),
      m_angleStep(0.f),
      m_segmentCount(0),
      m_segmentIndex(0),
      m_coordinateTransformer(coordinateTransformer),
      m_isIncrementalArcDistanceMode(true),
      m_currentPlan(ArcConverterPlan::XY)
{
}

FLASHMEM void ArcConverter::configure(const ConfigItem& item)
{
    if (strcmp(item.getKey(), MAX_ERROR_KEY) == 0)
    {
        m_maxError = item.getValueFloat();
    }
}

FLASHMEM void ArcConverter::checkConfigErrors(const MissingConfigCallback& onMissingConfigItem)
{
    CHECK_CONFIG_ERROR(onMissingConfigItem, m_maxError.has_value(), MAX_ERROR_KEY);
};

FLASHMEM void ArcConverter::begin()
{
    m_kernel->registerToEvent(ModuleEventType::GCODE_COMMAND, this);
    m_kernel->registerToEvent(ModuleEventType::TARGET_POSITION, this);
}

CommandResult ArcConverter::onGCodeCommandReceived(const GCode& gcode, CommandSource source, uint32_t commandId)
{
    if (gcode.code() == 17 && gcode.subcode() == tl::nullopt)
    {
        m_currentPlan = ArcConverterPlan::XY;
    }
    else if (gcode.code() == 18 && gcode.subcode() == tl::nullopt)
    {
        m_currentPlan = ArcConverterPlan::XZ;
    }
    else if (gcode.code() == 19 && gcode.subcode() == tl::nullopt)
    {
        m_currentPlan = ArcConverterPlan::YZ;
    }
    else if (gcode.code() == 90 && gcode.subcode() == 1u)
    {
        m_isIncrementalArcDistanceMode = false;
    }
    else if (gcode.code() == 91 && gcode.subcode() == 1u)
    {
        m_isIncrementalArcDistanceMode = true;
    }
    else
    {
        return CommandResult::notHandled();
    }

    return CommandResult::ok();
}

void ArcConverter::onTargetPositionChanged(const Vector3<float>& machinePosition)
{
    m_previousMachinePosition = machinePosition;
}

CommandResult ArcConverter::setArc(const GCode& gcode)
{
    if (gcode.code() != 2 && gcode.code() != 3)
    {
        return CommandResult::notHandled();
    }

    m_previousGcodePosition = m_coordinateTransformer->machineCoordinateToGcode(m_previousMachinePosition);

    m_feedrate = gcode.f();
    m_spindleRpm = gcode.s();

    setStartPoint();
    setEndPoint(gcode);
    if (!setCenterPoint(gcode))
    {
        clear();
        return CommandResult::error(CENTER_POINT_ERROR_COMMAND_ERROR_MESSAGE);
    }

    if (!setRadius())
    {
        clear();
        return CommandResult::error(RADIUS_ERROR_COMMAND_ERROR_MESSAGE);
    }

    if (!calculateSegments(gcode))
    {
        clear();
        return CommandResult::error(SEGMENT_ERROR_COMMAND_ERROR_MESSAGE);
    }
    return CommandResult::ok();
}

bool ArcConverter::getNextSegment(GCode& gcode)
{
    if (isFinished())
    {
        return false;
    }

    m_segmentIndex++;
    m_currentAngle += m_angleStep;
    m_currentOtherAxis += m_otherAxisStep;
    if (isFinished())
    {
        gcode = GCode::g1(fromPlan(m_endPoint, m_endOtherAxis), m_feedrate, m_spindleRpm, true);
    }
    else
    {
        Vector2<float> position = m_centerPoint + Vector2<float>(cos(m_currentAngle), sin(m_currentAngle)) * m_radius;
        gcode = GCode::g1(fromPlan(position, m_currentOtherAxis), m_feedrate, m_spindleRpm, true);
    }

    return true;
}

void ArcConverter::setStartPoint()
{
    pair<Vector2<float>, float> startPoint = toPlan(m_previousMachinePosition);
    m_startPoint = startPoint.first;
    m_startOtherAxis = startPoint.second;
}

void ArcConverter::setEndPoint(const GCode& gcode)
{
    Vector3<float> machinePosition =
        gcodePositionToMachinePosition(gcode.x(), gcode.y(), gcode.z(), gcode.isMachineCoordinateSystem());
    pair<Vector2<float>, float> endPoint = toPlan(machinePosition);
    m_endPoint = endPoint.first;
    m_endOtherAxis = endPoint.second;
}

bool ArcConverter::setCenterPoint(const GCode& gcode)
{
    if (gcode.r().has_value())
    {
        if (gcode.i().has_value() || gcode.j().has_value() || gcode.k().has_value() || !m_isIncrementalArcDistanceMode)
        {
            return false;
        }
        Vector2<double> centerPoint = getCenterPointFromRadius(
            m_startPoint.x,
            m_startPoint.y,
            m_endPoint.x,
            m_endPoint.y,
            *gcode.r(),
            isClockwise(gcode));
        m_centerPoint.x = static_cast<float>(centerPoint.x);
        m_centerPoint.y = static_cast<float>(centerPoint.y);
        return isfinite(centerPoint.x) && isfinite(centerPoint.y);
    }

    if (m_currentPlan == ArcConverterPlan::XY)
    {
        if ((gcode.i() == tl::nullopt && gcode.j() == tl::nullopt) || gcode.k().has_value())
        {
            return false;
        }
    }
    else if (m_currentPlan == ArcConverterPlan::XZ)
    {
        if ((gcode.i() == tl::nullopt && gcode.k() == tl::nullopt) || gcode.j().has_value())
        {
            return false;
        }
    }
    else if (m_currentPlan == ArcConverterPlan::YZ)
    {
        if ((gcode.j() == tl::nullopt && gcode.k() == tl::nullopt) || gcode.i().has_value())
        {
            return false;
        }
    }

    setCenterPointFromOffset(gcode);
    return true;
}

void ArcConverter::setCenterPointFromOffset(const GCode& gcode)
{
    if (m_isIncrementalArcDistanceMode)
    {
        Vector3<float> offset;
        if (gcode.i().has_value())
        {
            offset.x = *gcode.i();
        }
        if (gcode.j().has_value())
        {
            offset.y = *gcode.j();
        }
        if (gcode.k().has_value())
        {
            offset.z = *gcode.k();
        }

        auto centerPoint = gcodePositionToMachinePosition(
            m_previousGcodePosition.x + offset.x,
            m_previousGcodePosition.y + offset.y,
            m_previousGcodePosition.z + offset.z,
            false);
        m_centerPoint = toPlan(centerPoint).first;
    }
    else
    {
        Vector3<float> machinePosition =
            gcodePositionToMachinePosition(gcode.i(), gcode.j(), gcode.k(), gcode.isMachineCoordinateSystem());
        m_centerPoint = toPlan(machinePosition).first;
    }
}

bool ArcConverter::setRadius()
{
    float startRadius = (m_centerPoint - m_startPoint).norm();
    float endRadius = (m_centerPoint - m_endPoint).norm();

    if (abs(startRadius - endRadius) > RADIUS_TOLERANCE)
    {
        return false;
    }
    else
    {
        m_radius = startRadius;
        return true;
    }
}

bool ArcConverter::calculateSegments(const GCode& gcode)
{
    float maxAngleStep = abs(2.f * acos((m_radius - *m_maxError) / m_radius));
    bool isCw = isClockwise(gcode);

    Vector3<float> start(m_startPoint - m_centerPoint);
    Vector3<float> end(m_endPoint - m_centerPoint);
    float startAngle = atan2Pos(start.y, start.x);
    float endAngle = atan2Pos(end.y, end.x);
    float arcAngle = abs(endAngle - startAngle);

    if (arcAngle < MINIMUM_ARC_ANGLE)
    {
        arcAngle = 2 * PI_VALUE;
    }
    else if ((startAngle > endAngle && !isCw) || (startAngle < endAngle && isCw))
    {
        arcAngle = 2 * PI_VALUE - arcAngle;
    }

    if (gcode.p().has_value())
    {
        float p = ceil(*gcode.p());
        if (p < 0 || p != *gcode.p())
        {
            return false;
        }
        arcAngle += 2.f * PI_VALUE * (p - 1.f);
    }

    m_currentAngle = startAngle;
    m_angleStep = min(arcAngle, maxAngleStep);
    m_segmentCount = static_cast<size_t>(ceil(arcAngle / m_angleStep));
    m_segmentIndex = 0;

    m_currentOtherAxis = m_startOtherAxis;
    m_otherAxisStep = (m_endOtherAxis - m_startOtherAxis) * m_angleStep / arcAngle;

    if (isCw)
    {
        m_angleStep = -m_angleStep;
    }

    return true;
}

bool ArcConverter::isClockwise(const GCode& gcode)
{
    bool isClockwise = false;
    if (gcode.code() == 2)
    {
        isClockwise = true;
    }
    if (m_currentPlan == ArcConverterPlan::XZ)
    {
        isClockwise = !isClockwise;
    }
    return isClockwise;
}

Vector3<float> ArcConverter::gcodePositionToMachinePosition(
    const tl::optional<float>& x,
    const tl::optional<float>& y,
    const tl::optional<float>& z,
    bool isMachineCoordinateSystem)
{
    Vector3<float> machinePosition;

    if (isMachineCoordinateSystem)
    {
        if (x.has_value())
        {
            machinePosition.x = *x;
        }
        else
        {
            machinePosition.x = m_previousMachinePosition.x;
        }
        if (y.has_value())
        {
            machinePosition.y = *y;
        }
        else
        {
            machinePosition.y = m_previousMachinePosition.y;
        }
        if (z.has_value())
        {
            machinePosition.z = *z;
        }
        else
        {
            machinePosition.z = m_previousMachinePosition.z;
        }
    }
    else
    {
        Vector3<float> gcodePosition;
        if (x.has_value())
        {
            gcodePosition.x = *x;
        }
        else
        {
            gcodePosition.x = m_previousGcodePosition.x;
        }
        if (y.has_value())
        {
            gcodePosition.y = *y;
        }
        else
        {
            gcodePosition.y = m_previousGcodePosition.y;
        }
        if (z.has_value())
        {
            gcodePosition.z = *z;
        }
        else
        {
            gcodePosition.z = m_previousGcodePosition.z;
        }
        machinePosition = m_coordinateTransformer->gcodeCoordinateToMachineCoordinate(gcodePosition);
    }

    return machinePosition;
}

pair<Vector2<float>, float> ArcConverter::toPlan(const Vector3<float>& position)
{
    switch (m_currentPlan)
    {
        case ArcConverterPlan::XY:
            return {Vector2<float>(position.x, position.y), position.z};
        case ArcConverterPlan::XZ:
            return {Vector2<float>(position.x, position.z), position.y};
        case ArcConverterPlan::YZ:
            return {Vector2<float>(position.y, position.z), position.x};
    }

    return {Vector2<float>(), 0.0};
}

Vector3<float> ArcConverter::fromPlan(Vector2<float> position, float otherAxis)
{
    switch (m_currentPlan)
    {
        case ArcConverterPlan::XY:
            return {position.x, position.y, otherAxis};
        case ArcConverterPlan::XZ:
            return {position.x, otherAxis, position.y};
        case ArcConverterPlan::YZ:
            return {otherAxis, position.x, position.y};
    }

    return {};
}

float atan2Pos(float a, float b)
{
    float angle = atan2(a, b);
    if (angle < 0.f)
    {
        angle = 2 * PI_VALUE + angle;
    }
    return angle;
}

Vector2<double> getCenterPointFromRadius(double x1, double y1, double x2, double y2, float radius, bool isClockwise)
{
    // double is required here to prevent to have the wrong center from rounding errors
    // See : https://stackoverflow.com/questions/36211171/finding-center-of-a-circle-given-two-points-and-radius
    double dx = x2 - x1;
    double dy = y1 - y2;
    double q = sqrt(dx * dx + dy * dy);
    double qHalf = q / 2.0;
    double x3 = (x1 + x2) / 2.0;
    double y3 = (x1 + x2) / 2.0;

    if (abs(qHalf - abs(radius)) < RADIUS_TOLERANCE)
    {
        return {x3, y3};
    }

    double r = radius;
    double rSquared = r * r;
    double qHalfSquared = qHalf * qHalf;
    double bx = sqrt(rSquared - qHalfSquared) * dy / q;
    double by = sqrt(rSquared - qHalfSquared) * dx / q;

    Vector2<double> centerPoint1(x3 + bx, y3 + by);
    Vector2<double> centerPoint2(x3 - bx, y3 - by);

    Vector3<double> start1(x1 - centerPoint1.x, y1 - centerPoint1.y, 0.f);
    Vector3<double> end1(x2 - centerPoint1.x, y2 - centerPoint1.y, 0.f);
    bool is1Clockwise = start1.cross(end1).z < 0.f;
    if (radius < 0.f)
    {
        is1Clockwise = !is1Clockwise;
    }

    if (isClockwise == is1Clockwise)
    {
        return centerPoint1;
    }
    else
    {
        return centerPoint2;
    }
}
