#include <cnc/modules/CoordinateTransformer.h>
#include <cnc/modules/ModuleKernel.h>
#include <cnc/space.h>

constexpr const char* INVALID_COORDINATE_SYSTEM_COMMAND_ERROR_MESSAGE =
    "The coordindate system (P) is invalid. It must be between 0 and 9.";
constexpr const char* MISSING_AXIS_COMMAND_ERROR_MESSAGE = "At least one axis must be specified.";

FLASHMEM CoordinateSystem::CoordinateSystem() : offset(0.f, 0.f, 0.f), rotation(0.f), rotationInv(0.f) {}

FLASHMEM CoordinateTransformer::CoordinateTransformer()
    : m_isIncrementalMode(false),
      m_scale(1.f),
      m_currentCoordinateSystemIndex(0),
      m_globalOffset(0.f, 0.f, 0.f)
{
}

FLASHMEM void CoordinateTransformer::configure(const ConfigItem& item) {}

FLASHMEM void CoordinateTransformer::checkConfigErrors(const MissingConfigCallback& onMissingConfigItem)
{
}

FLASHMEM void CoordinateTransformer::begin()
{
    m_kernel->registerToEvent(ModuleEventType::GCODE_COMMAND, this);
    m_kernel->registerToEvent(ModuleEventType::TARGET_POSITION, this);
}

CommandResult
    CoordinateTransformer::onGCodeCommandReceived(const GCode& gcode, CommandSource source, uint32_t commandId)
{
    if (gcode.code() == 10 && gcode.subcode() == tl::nullopt && gcode.l() == 2u)
    {
        return setCoordinateSystemL2(gcode);
    }
    else if (gcode.code() == 10 && gcode.subcode() == tl::nullopt && gcode.l() == 20u)
    {
        return setCoordinateSystemL20(gcode);
    }
    else if (gcode.code() == 20 && gcode.subcode() == tl::nullopt)
    {
        m_scale = INCH_TO_MM_SCALE;
    }
    else if (gcode.code() == 21 && gcode.subcode() == tl::nullopt)
    {
        m_scale = 1.f;
    }
    else if (gcode.code() == 54 && gcode.subcode() == tl::nullopt)
    {
        m_currentCoordinateSystemIndex = 0;
    }
    else if (gcode.code() == 55 && gcode.subcode() == tl::nullopt)
    {
        m_currentCoordinateSystemIndex = 1;
    }
    else if (gcode.code() == 56 && gcode.subcode() == tl::nullopt)
    {
        m_currentCoordinateSystemIndex = 2;
    }
    else if (gcode.code() == 57 && gcode.subcode() == tl::nullopt)
    {
        m_currentCoordinateSystemIndex = 3;
    }
    else if (gcode.code() == 58 && gcode.subcode() == tl::nullopt)
    {
        m_currentCoordinateSystemIndex = 4;
    }
    else if (gcode.code() == 59 && gcode.subcode() == tl::nullopt)
    {
        m_currentCoordinateSystemIndex = 5;
    }
    else if (gcode.code() == 59 && gcode.subcode() == 1u)
    {
        m_currentCoordinateSystemIndex = 6;
    }
    else if (gcode.code() == 59 && gcode.subcode() == 2u)
    {
        m_currentCoordinateSystemIndex = 7;
    }
    else if (gcode.code() == 59 && gcode.subcode() == 3u)
    {
        m_currentCoordinateSystemIndex = 8;
    }
    else if (gcode.code() == 90 && gcode.subcode() == tl::nullopt)
    {
        m_isIncrementalMode = false;
    }
    else if (gcode.code() == 91 && gcode.subcode() == tl::nullopt)
    {
        m_isIncrementalMode = true;
    }
    else if (gcode.code() == 92 && gcode.subcode() == tl::nullopt)
    {
        return setGlobalOffset(gcode);
    }
    else if (gcode.code() == 92 && gcode.subcode() == 1u)
    {
        m_globalOffset = Vector3<float>(0.f, 0.f, 0.f);
    }
    else
    {
        return CommandResult::notHandled();
    }

    return CommandResult::ok();
}

void CoordinateTransformer::onTargetPositionChanged(const Vector3<float>& machinePosition)
{
    m_targetMachinePosition = machinePosition;
}

Vector3<float> CoordinateTransformer::gcodeCoordinateToMachineCoordinate(const Vector3<float>& v)
{
    CoordinateSystem& coordinateSystem = m_coordinateSystems[m_currentCoordinateSystemIndex];
    Vector3<float> rotatedV = coordinateSystem.rotation.rotate(v * m_scale);
    if (m_isIncrementalMode)
    {
        return rotatedV + m_targetMachinePosition;
    }
    else
    {
        return rotatedV + coordinateSystem.offset + m_globalOffset;
    }
}

Vector3<float> CoordinateTransformer::machineCoordinateToGcode(const Vector3<float>& v)
{
    CoordinateSystem& coordinateSystem = m_coordinateSystems[m_currentCoordinateSystemIndex];
    if (m_isIncrementalMode)
    {
        return coordinateSystem.rotationInv.rotate(v - m_targetMachinePosition) / m_scale;
    }
    else
    {
        return coordinateSystem.rotationInv.rotate(v - m_globalOffset - coordinateSystem.offset) / m_scale;
    }
}

Vector3<float> CoordinateTransformer::machineCoordinateToUserCurrentCoordinate(const Vector3<float>& v)
{
    CoordinateSystem& coordinateSystem = m_coordinateSystems[m_currentCoordinateSystemIndex];
    return coordinateSystem.rotationInv.rotate(v - m_globalOffset - coordinateSystem.offset) / m_scale;
}

Vector3<float> CoordinateTransformer::machineCoordinateToUserMachineCoordinate(const Vector3<float>& v)
{
    return v / m_scale;
}

CommandResult CoordinateTransformer::setCoordinateSystemL2(const GCode& gcode)
{
    tl::optional<size_t> coordinateSystemIndex = getCoordinateSystemIndex(gcode);
    if (!coordinateSystemIndex.has_value())
    {
        return CommandResult::error(INVALID_COORDINATE_SYSTEM_COMMAND_ERROR_MESSAGE);
    }

    CoordinateSystem& coordinateSystem = m_coordinateSystems[*coordinateSystemIndex];
    if (gcode.x().has_value())
    {
        coordinateSystem.offset.x = *gcode.x() * m_scale;
    }
    if (gcode.y().has_value())
    {
        coordinateSystem.offset.y = *gcode.y() * m_scale;
    }
    if (gcode.z().has_value())
    {
        coordinateSystem.offset.z = *gcode.z() * m_scale;
    }
    if (gcode.r().has_value())
    {
        float rad = degToRad(*gcode.r());
        coordinateSystem.rotation.setAngleRad(rad);
        coordinateSystem.rotationInv.setAngleRad(-rad);
    }

    return CommandResult::ok();
}

CommandResult CoordinateTransformer::setCoordinateSystemL20(const GCode& gcode)
{
    tl::optional<size_t> coordinateSystemIndex = getCoordinateSystemIndex(gcode);
    if (!coordinateSystemIndex.has_value())
    {
        return CommandResult::error(INVALID_COORDINATE_SYSTEM_COMMAND_ERROR_MESSAGE);
    }

    CoordinateSystem& coordinateSystem = m_coordinateSystems[*coordinateSystemIndex];
    if (gcode.x().has_value())
    {
        coordinateSystem.offset.x = m_targetMachinePosition.x - m_globalOffset.x - (*gcode.x() * m_scale);
    }
    if (gcode.y().has_value())
    {
        coordinateSystem.offset.y = m_targetMachinePosition.y - m_globalOffset.y - (*gcode.y() * m_scale);
    }
    if (gcode.z().has_value())
    {
        coordinateSystem.offset.z = m_targetMachinePosition.z - m_globalOffset.z - (*gcode.z() * m_scale);
    }

    return CommandResult::ok();
}

tl::optional<size_t> CoordinateTransformer::getCoordinateSystemIndex(const GCode& gcode)
{
    if (!gcode.p().has_value())
    {
        return tl::nullopt;
    }
    size_t p = static_cast<size_t>(*gcode.p());

    if (p == 0)
    {
        return m_currentCoordinateSystemIndex;
    }
    else if (1 <= p && p <= 9)
    {
        return p - 1;
    }

    return tl::nullopt;
}

CommandResult CoordinateTransformer::setGlobalOffset(const GCode& gcode)
{
    if (gcode.x() == tl::nullopt && gcode.y() == tl::nullopt && gcode.z() == tl::nullopt)
    {
        return CommandResult::error(MISSING_AXIS_COMMAND_ERROR_MESSAGE);
    }

    if (gcode.x().has_value())
    {
        m_globalOffset.x = m_targetMachinePosition.x - (*gcode.x() * m_scale);
    }
    if (gcode.y().has_value())
    {
        m_globalOffset.y = m_targetMachinePosition.y - (*gcode.y() * m_scale);
    }
    if (gcode.z().has_value())
    {
        m_globalOffset.z = m_targetMachinePosition.z - (*gcode.z() * m_scale);
    }

    return CommandResult::ok();
}
