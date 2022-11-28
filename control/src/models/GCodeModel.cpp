#include "control/models/GCodeModel.h"
#include "cnc/modules/ModuleKernel.h"
#include "cnc/modules/CoordinateTransformer.h"
#include "cnc/modules/ArcConverter.h"
#include "cnc/parsing/SystemCommand.h"
#include "cnc/parsing/GCode.h"
#include "cnc/parsing/MCode.h"

#include <QMap>
#include <QFile>
#include <QTextStream>

#include <clocale>

class GuiKernel : public ModuleKernel
{
    QList<Module*> m_modules;
    QMap<ModuleEventType, QList<Module*>> m_modulesByEventType;

    SystemCommandParser m_systemCommandParser;
    GCodeParser m_gcodeParser;
    GCode m_gcode;
    MCodeParser m_mcodeParser;
    MCode m_mcode;

    bool m_hasError;

public:
    GuiKernel();
    ~GuiKernel() override = default;

    DECLARE_NOT_COPYABLE(GuiKernel);
    DECLARE_NOT_MOVABLE(GuiKernel);

    bool hasError() const;

    void addModule(Module* module) override;
    void registerToEvent(ModuleEventType eventType, Module* module) override;

    void executeCommand(const char* line, CommandSource source, tl::optional<uint32_t>& commandId) override;
    void sendCommandResponse(const char* commandResponse, CommandSource source, uint32_t commandId, bool isComplete)
        override;

    void dispatchTargetPosition(const Vector3<float>& machinePosition) override;

private:
    void executeSystemCommand(const char* line, CommandSource source, uint32_t commandId);
    void executeGCodeCommand(const char* line, CommandSource source, uint32_t commandId);
    void executeMCodeCommand(const char* line, CommandSource source, uint32_t commandId);

    RawCommandResult dispatchRawCommand(const char* line, CommandSource source, uint32_t commandId);
    void dispatchSystemCommand(const SystemCommand& command, CommandSource source, uint32_t commandId);
    void dispatchGCodeCommand(const GCode& gcode, CommandSource source, uint32_t commandId);
    void dispatchMCodeCommand(const MCode& mcode, CommandSource source, uint32_t commandId);

    void handleAgregatedCommandResult(CommandResult result, CommandSource source, uint32_t commandId);
};

GuiKernel::GuiKernel() : m_hasError(false) {}

bool GuiKernel::hasError() const
{
    return m_hasError;
}

void GuiKernel::addModule(Module* module)
{
    m_modules.push_back(module);
    module->setKernel(this);
}

void GuiKernel::registerToEvent(ModuleEventType eventType, Module* module)
{
    m_modulesByEventType[eventType].push_back(module);
}

void GuiKernel::executeCommand(const char* line, CommandSource source, tl::optional<uint32_t>& commandId)
{
    if (dispatchRawCommand(line, source, *commandId) == RawCommandResult::HANDLED)
    {
        return;
    }

    char firstCharacter = line[0];
    for (size_t i = 0; line[i] == ' '; i++)
    {
        firstCharacter = line[i + 1];
    }

    if (firstCharacter == '\0' || firstCharacter == '(' || firstCharacter == ';')
    {
        return;
    }
    else if (firstCharacter == '$')
    {
        executeSystemCommand(line, source, *commandId);
    }
    else if (firstCharacter == 'M' || firstCharacter == 'm')
    {
        executeMCodeCommand(line, source, *commandId);
    }
    else
    {
        executeGCodeCommand(line, source, *commandId);
    }
}

void GuiKernel::sendCommandResponse(const char* response, CommandSource source, uint32_t commandId, bool isCompleted)
{
    for (auto module : m_modulesByEventType[ModuleEventType::COMMAND_RESPONSE])
    {
        module->onCommandResponse(response, source, commandId, isCompleted);
    }
}

void GuiKernel::dispatchTargetPosition(const Vector3<float>& machinePosition)
{
    for (auto module : m_modulesByEventType[ModuleEventType::TARGET_POSITION])
    {
        module->onTargetPositionChanged(machinePosition);
    }
}

void GuiKernel::executeSystemCommand(const char* line, CommandSource source, uint32_t commandId)
{
    SystemCommand command;
    ParsingResult result = m_systemCommandParser.parse(line, command);

    if (result == ParsingResult::OK)
    {
        dispatchSystemCommand(command, source, commandId);
    }
    else if (result == ParsingResult::ERROR)
    {
        m_hasError = true;
    }
}

void GuiKernel::executeGCodeCommand(const char* line, CommandSource source, uint32_t commandId)
{
    ParsingResult result = m_gcodeParser.parse(line, m_gcode);

    if (result == ParsingResult::OK)
    {
        dispatchGCodeCommand(m_gcode, source, commandId);
    }
    else if (result == ParsingResult::ERROR)
    {
        m_hasError = true;
    }
}

void GuiKernel::executeMCodeCommand(const char* line, CommandSource source, uint32_t commandId)
{
    ParsingResult result = m_mcodeParser.parse(line, m_mcode);

    if (result == ParsingResult::OK)
    {
        dispatchMCodeCommand(m_mcode, source, commandId);
    }
    else if (result == ParsingResult::ERROR)
    {
        m_hasError = true;
    }
}

RawCommandResult GuiKernel::dispatchRawCommand(const char* line, CommandSource source, uint32_t commandId)
{
    constexpr size_t EVENT_INDEX = static_cast<size_t>(ModuleEventType::RAW_COMMAND);
    for (auto module : m_modulesByEventType[ModuleEventType::RAW_COMMAND])
    {
        RawCommandResult result = module->onRawCommandReceived(line, source, commandId);
        if (result == RawCommandResult::HANDLED)
        {
            return RawCommandResult::HANDLED;
        }
    }
    return RawCommandResult::NOT_HANDLED;
}

void GuiKernel::dispatchSystemCommand(const SystemCommand& command, CommandSource source, uint32_t commandId)
{
    CommandResult agregatedResult = CommandResult::notHandled();
    for (auto module : m_modulesByEventType[ModuleEventType::SYSTEM_COMMAND])
    {
        CommandResult result = module->onSystemCommandReceived(command, source, commandId);
        agregatedResult = agregatedResult.agregate(result);
    }
    handleAgregatedCommandResult(agregatedResult, source, commandId);
}

void GuiKernel::dispatchGCodeCommand(const GCode& gcode, CommandSource source, uint32_t commandId)
{
    CommandResult agregatedResult = CommandResult::notHandled();
    for (auto module : m_modulesByEventType[ModuleEventType::GCODE_COMMAND])
    {
        CommandResult result = module->onGCodeCommandReceived(gcode, source, commandId);
        agregatedResult = agregatedResult.agregate(result);
    }
    handleAgregatedCommandResult(agregatedResult, source, commandId);
}

void GuiKernel::dispatchMCodeCommand(const MCode& mcode, CommandSource source, uint32_t commandId)
{
    CommandResult agregatedResult = CommandResult::notHandled();
    for (auto module : m_modulesByEventType[ModuleEventType::MCODE_COMMAND])
    {
        CommandResult result = module->onMCodeCommandReceived(mcode, source, commandId);
        agregatedResult = agregatedResult.agregate(result);
    }
    handleAgregatedCommandResult(agregatedResult, source, commandId);
}

void GuiKernel::handleAgregatedCommandResult(CommandResult result, CommandSource source, uint32_t commandId)
{
    if (result.type() == CommandResultType::ERROR)
    {
        m_hasError = true;
    }
}


class LineCreator : public Module
{
    Vector3<float> m_startPoint;
    int m_fileLine;
    QList<GCodeLine>& m_lines;

    CoordinateTransformer* m_coordinateTransformer;
    ArcConverter* m_arcConverter;

public:
    LineCreator(CoordinateTransformer* coordinateTransformer, ArcConverter* arcConverter, QList<GCodeLine>& lines);
    ~LineCreator() override = default;

    DECLARE_NOT_COPYABLE(LineCreator);
    DECLARE_NOT_MOVABLE(LineCreator);

    void configure(const ConfigItem& item) override;
    void checkConfigErrors(std::function<void(const char*, const char*, const char*)> onMissingConfigItem) override;
    void begin() override;

    CommandResult onGCodeCommandReceived(const GCode& gcode, CommandSource source, uint32_t commandId) override;
};

LineCreator::LineCreator(
    CoordinateTransformer* coordinateTransformer,
    ArcConverter* arcConverter,
    QList<GCodeLine>& lines)
    : m_fileLine(0),
      m_coordinateTransformer(coordinateTransformer),
      m_arcConverter(arcConverter),
      m_lines(lines)
{
}

void LineCreator::configure(const ConfigItem& item) {}

void LineCreator::checkConfigErrors(std::function<void(const char*, const char*, const char*)> onMissingConfigItem) {}

void LineCreator::begin()
{
    m_kernel->registerToEvent(ModuleEventType::GCODE_COMMAND, this);
}

CommandResult LineCreator::onGCodeCommandReceived(const GCode& gcode, CommandSource source, uint32_t commandId)
{
    bool fast = gcode.code() == 0;

    if (gcode.code() == 0 || gcode.code() == 1)
    {
        float x = *gcode.x().or_else([this]() { return m_startPoint.x; });
        float y = *gcode.y().or_else([this]() { return m_startPoint.y; });
        float z = *gcode.z().or_else([this]() { return m_startPoint.z; });
        auto endPoint = m_coordinateTransformer->gcodeCoordinateToMachineCoordinate(Vector3<float>(x, y, z));
        m_lines.push_back(GCodeLine{
            QVector3D(m_startPoint.x, m_startPoint.y, m_startPoint.z),
            QVector3D(endPoint.x, endPoint.y, endPoint.z),
            m_fileLine,
            fast});
        m_startPoint = endPoint;
        m_kernel->dispatchTargetPosition(endPoint);
    }
    else if (gcode.code() == 2 || gcode.code() == 3)
    {
        m_arcConverter->setArc(gcode);
        GCode lineGCode;
        while (m_arcConverter->getNextSegment(lineGCode))
        {
            float x = *lineGCode.x().or_else([this]() { return m_startPoint.x; });
            float y = *lineGCode.y().or_else([this]() { return m_startPoint.y; });
            float z = *lineGCode.z().or_else([this]() { return m_startPoint.z; });
            auto endPoint = m_coordinateTransformer->gcodeCoordinateToMachineCoordinate(Vector3<float>(x, y, z));

            m_lines.push_back(GCodeLine{
                QVector3D(m_startPoint.x, m_startPoint.y, m_startPoint.z),
                QVector3D(endPoint.x, endPoint.y, endPoint.z),
                m_fileLine,
                fast});
            m_startPoint = endPoint;
        }
        m_kernel->dispatchTargetPosition(m_startPoint);
    }

    m_fileLine++;
    return CommandResult::ok();
}


GCodeModel::GCodeModel() : m_completedCommandCount(0) {}

void GCodeModel::load(const QString& path)
{
    m_commands.clear();
    m_lines.clear();
    m_completedCommandCount = 0;

    readCommands(path);
    QString lastLine;
    if (!calculateLines(lastLine))
    {
        m_commands.clear();
        m_lines.clear();
        emit invalidLine(lastLine);
    }
    emit gcodeChanged();
}

void GCodeModel::readCommands(const QString& path)
{
    QFile inputFile(path);
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream inputStream(&inputFile);
        while (!inputStream.atEnd())
        {
            m_commands.push_back(inputStream.readLine());
        }
        inputFile.close();
    }
}

bool GCodeModel::calculateLines(QString& lastLine)
{
    std::setlocale(LC_ALL, "C");

    GuiKernel kernel;
    CoordinateTransformer coordinateTransformer;
    ArcConverter arcConverter(&coordinateTransformer);
    arcConverter.configure(ConfigItem("arc_converter.max_error_in_mm", "0.1"));
    LineCreator lineCreator(&coordinateTransformer, &arcConverter, m_lines);

    kernel.addModule(&coordinateTransformer);
    kernel.addModule(&arcConverter);
    kernel.addModule(&lineCreator);

    coordinateTransformer.begin();
    arcConverter.begin();
    lineCreator.begin();

    tl::optional<uint32_t> commandId;
    for (auto command : m_commands)
    {
        lastLine = command;
        kernel.executeCommand(command.toStdString().c_str(), CommandSource::FILE_SOURCE, commandId);

        if (kernel.hasError())
        {
            return false;
        }
    }

    return true;
}
