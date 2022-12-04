#include <cnc/modules/Planner.h>
#include <cnc/modules/ModuleKernel.h>

#include <cstring>

constexpr const char* X_STEP_COUNT_PER_MM_KEY = "planner.x.step_count_per_mm";
constexpr const char* Y_STEP_COUNT_PER_MM_KEY = "planner.y.step_count_per_mm";
constexpr const char* Z_STEP_COUNT_PER_MM_KEY = "planner.z.step_count_per_mm";

constexpr const char* ACCELERATION_IN_MM_PER_SS_KEY = "planner.acceleration_in_mm_per_ss";
constexpr const char* JUNCTION_DEVIATION_KEY = "planner.junction_deviation";

Planner::Planner(CoordinateTransformer* coordinateTransformer, ArcConverter* arcConverter)
    : m_coordinateTransformer(coordinateTransformer),
      m_arcConverter(arcConverter),
      m_machineRange(Vector3<float>(), Vector3<float>())
{
}

void Planner::configure(const ConfigItem& item)
{
    if (strcmp(item.getKey(), X_STEP_COUNT_PER_MM_KEY) == 0)
    {
        m_xStepCountPerMm = item.getValueFloat();
    }
    else if (strcmp(item.getKey(), Y_STEP_COUNT_PER_MM_KEY) == 0)
    {
        m_yStepCountPerMm = item.getValueFloat();
    }
    else if (strcmp(item.getKey(), Z_STEP_COUNT_PER_MM_KEY) == 0)
    {
        m_zStepCountPerMm = item.getValueFloat();
    }
    else if (strcmp(item.getKey(), ACCELERATION_IN_MM_PER_SS_KEY) == 0)
    {
        m_accelerationInMmPerSS = item.getValueFloat();
    }
    else if (strcmp(item.getKey(), JUNCTION_DEVIATION_KEY) == 0)
    {
        m_junctionDeviation = item.getValueFloat();
    }
}

void Planner::checkConfigErrors(const MissingConfigCallback& onMissingConfigItem)
{
    CHECK_CONFIG_ERROR(onMissingConfigItem, m_xStepCountPerMm.has_value(), X_STEP_COUNT_PER_MM_KEY)
    CHECK_CONFIG_ERROR(onMissingConfigItem, m_yStepCountPerMm.has_value(), Y_STEP_COUNT_PER_MM_KEY)
    CHECK_CONFIG_ERROR(onMissingConfigItem, m_zStepCountPerMm.has_value(), Z_STEP_COUNT_PER_MM_KEY)
    CHECK_CONFIG_ERROR(onMissingConfigItem, m_accelerationInMmPerSS.has_value(), ACCELERATION_IN_MM_PER_SS_KEY)
    CHECK_CONFIG_ERROR(onMissingConfigItem, m_junctionDeviation.has_value(), JUNCTION_DEVIATION_KEY)
}

void Planner::begin()
{
    m_kernel->registerToEvent(ModuleEventType::GCODE_COMMAND, this);
    m_kernel->registerToEvent(ModuleEventType::MCODE_COMMAND, this);
}

CommandResult Planner::onGCodeCommandReceived(const GCode& gcode, CommandSource source, uint32_t commandId)
{
    // TODO
    return CommandResult::notHandled();
}

CommandResult Planner::onMCodeCommandReceived(const MCode& gcode, CommandSource source, uint32_t commandId)
{
    // TODO
    return CommandResult::notHandled();
}

void Planner::update()
{
    // TODO
}

void Planner::reset(const InclusiveRange3<float>& range)
{
    m_machineRange = range;
    m_lastRequestedPosition = Vector3<int32_t>(0, 0, 0);
}
