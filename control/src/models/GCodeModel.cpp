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

    uint32_t m_currentCommandId;

    SystemCommandParser m_systemCommandParser;
    GCodeParser m_gcodeParser;
    GCode m_gcode;
    MCodeParser m_mcodeParser;
    MCode m_mcode;

    QStringList& m_invalidCommands;

public:
    GuiKernel(QStringList& invalidCommands);
    ~GuiKernel() override = default;

    DECLARE_NOT_COPYABLE(GuiKernel);
    DECLARE_NOT_MOVABLE(GuiKernel);

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
    void dispatchSystemCommand(const char* line, const SystemCommand& command, CommandSource source, uint32_t commandId);
    void dispatchGCodeCommand(const char* line, const GCode& gcode, CommandSource source, uint32_t commandId);
    void dispatchMCodeCommand(const char* line, const MCode& mcode, CommandSource source, uint32_t commandId);

    void addInvalidCommand(const char* line, uint32_t commandId, const char* errorName);
};

GuiKernel::GuiKernel(QStringList& invalidCommands) : m_currentCommandId(1), m_invalidCommands(invalidCommands)
{
    std::setlocale(LC_ALL, "C");
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
    commandId = m_currentCommandId;
    m_currentCommandId++;

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
        dispatchSystemCommand(line, command, source, commandId);
    }
    else if (result == ParsingResult::ERROR)
    {
        addInvalidCommand(line, commandId, "Parsing Error");
    }
}

void GuiKernel::executeGCodeCommand(const char* line, CommandSource source, uint32_t commandId)
{
    ParsingResult result = m_gcodeParser.parse(line, m_gcode);

    if (result == ParsingResult::OK)
    {
        dispatchGCodeCommand(line, m_gcode, source, commandId);
    }
    else if (result == ParsingResult::ERROR)
    {
        addInvalidCommand(line, commandId, "Parsing Error");
    }
}

void GuiKernel::executeMCodeCommand(const char* line, CommandSource source, uint32_t commandId)
{
    ParsingResult result = m_mcodeParser.parse(line, m_mcode);

    if (result == ParsingResult::OK)
    {
        dispatchMCodeCommand(line, m_mcode, source, commandId);
    }
    else if (result == ParsingResult::ERROR)
    {
        addInvalidCommand(line, commandId, "Parsing Error");
    }
}

RawCommandResult GuiKernel::dispatchRawCommand(const char* line, CommandSource source, uint32_t commandId)
{
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

void GuiKernel::dispatchSystemCommand(const char* line, const SystemCommand& command, CommandSource source, uint32_t commandId)
{
    for (auto module : m_modulesByEventType[ModuleEventType::SYSTEM_COMMAND])
    {
        CommandResult result = module->onSystemCommandReceived(command, source, commandId);
        if (result.type() == CommandResultType::ERROR)
        {
            addInvalidCommand(line, commandId, result.errorMessage());
        }
    }
}

void GuiKernel::dispatchGCodeCommand(const char* line, const GCode& gcode, CommandSource source, uint32_t commandId)
{
    for (auto module : m_modulesByEventType[ModuleEventType::GCODE_COMMAND])
    {
        CommandResult result = module->onGCodeCommandReceived(gcode, source, commandId);
        if (result.type() == CommandResultType::ERROR)
        {
            addInvalidCommand(line, commandId, result.errorMessage());
        }
    }
}

void GuiKernel::dispatchMCodeCommand(const char* line, const MCode& mcode, CommandSource source, uint32_t commandId)
{
    for (auto module : m_modulesByEventType[ModuleEventType::MCODE_COMMAND])
    {
        CommandResult result = module->onMCodeCommandReceived(mcode, source, commandId);
        if (result.type() == CommandResultType::ERROR)
        {
            addInvalidCommand(line, commandId, result.errorMessage());
        }
    }
}

void GuiKernel::addInvalidCommand(const char* line, uint32_t commandId, const char* errorName)
{
    m_invalidCommands.push_back(QString(errorName) + ":" + QString::number(commandId) + ": " + line);
}


#define CHECK_MISSING_PARAMETER(code, parameter, parameterName) \
    do                                                        \
    {                                                         \
        if (!(code).parameter().has_value())                   \
        {                                                     \
            return CommandResult::error("Missing " parameterName " parameter"); \
        }                                                     \
    } while(false)

#define CHECK_EXTRA_SUBCODE(code) \
    do                                                        \
    {                                                         \
        if ((code).subcode().has_value())                   \
        {                                                     \
            return CommandResult::error("Extra subcode"); \
        }                                                     \
    } while(false)

#define CHECK_EXTRA_PARAMETER(code, parameter, parameterName) \
    do                                                        \
    {                                                         \
        if ((code).parameter().has_value())                   \
        {                                                     \
            return CommandResult::error("Extra " parameterName " parameter"); \
        }                                                     \
    } while(false)

#define CHECK_EXTRA_PARAMETER_NULLPTR(code, parameter, parameterName) \
    do                                                        \
    {                                                         \
        if ((code).parameter() != nullptr)                   \
        {                                                     \
            return CommandResult::error("Extra " parameterName " parameter"); \
        }                                                     \
    } while(false)

#define CHECK_GCODE_NO_PARAMETER(gcode) \
    CHECK_EXTRA_PARAMETER(gcode, x, "X"); \
    CHECK_EXTRA_PARAMETER(gcode, y, "Y"); \
    CHECK_EXTRA_PARAMETER(gcode, z, "Z"); \
    CHECK_EXTRA_PARAMETER(gcode, f, "F"); \
    CHECK_EXTRA_PARAMETER(gcode, i, "I"); \
    CHECK_EXTRA_PARAMETER(gcode, j, "J"); \
    CHECK_EXTRA_PARAMETER(gcode, k, "K"); \
    CHECK_EXTRA_PARAMETER(gcode, s, "S"); \
    CHECK_EXTRA_PARAMETER(gcode, p, "P"); \
    CHECK_EXTRA_PARAMETER(gcode, r, "R"); \
    CHECK_EXTRA_PARAMETER(gcode, l, "L");

#define CHECK_MCODE_NO_PARAMETER(mcode) \
    CHECK_EXTRA_PARAMETER(mcode, x, "X"); \
    CHECK_EXTRA_PARAMETER(mcode, y, "Y"); \
    CHECK_EXTRA_PARAMETER(mcode, z, "Z"); \
    CHECK_EXTRA_PARAMETER(mcode, s, "S"); \
    CHECK_EXTRA_PARAMETER_NULLPTR(mcode, path, "path"); \
    CHECK_EXTRA_PARAMETER(mcode, p, "P"); \
    CHECK_EXTRA_PARAMETER(mcode, i, "I"); \
    CHECK_EXTRA_PARAMETER(mcode, d, "D")

class CommandValidator : public Module
{
public:
    CommandValidator() = default;
    ~CommandValidator() override = default;

    DECLARE_NOT_COPYABLE(CommandValidator);
    DECLARE_NOT_MOVABLE(CommandValidator);

    void configure(const ConfigItem& item) override;
    void checkConfigErrors(std::function<void(const char*, const char*, const char*)> onMissingConfigItem) override;
    void begin() override;

    CommandResult onSystemCommandReceived(const SystemCommand& command, CommandSource source, uint32_t commandId) override;
    CommandResult onGCodeCommandReceived(const GCode& gcode, CommandSource source, uint32_t commandId) override;
    CommandResult onMCodeCommandReceived(const MCode& mcode, CommandSource source, uint32_t commandId) override;

private:
    static CommandResult validateG0G1(const GCode& gcode);
    static CommandResult validateG2G3(const GCode& gcode);
    static CommandResult validateG4(const GCode& gcode);
    static CommandResult validateG17G18G19(const GCode& gcode);
    static CommandResult validateG20G21(const GCode& gcode);
    static CommandResult validateG90G91(const GCode& gcode);

    static CommandResult validateM3(const MCode& mcode);
    static CommandResult validateM5(const MCode& mcode);
    static CommandResult validateM17(const MCode& mcode);
    static CommandResult validateM18(const MCode& mcode);
    static CommandResult validateM220(const MCode& mcode);
};

void CommandValidator::configure(const ConfigItem& item) {}

void CommandValidator::checkConfigErrors(std::function<void(const char*, const char*, const char*)> onMissingConfigItem) {}

void CommandValidator::begin()
{
    m_kernel->registerToEvent(ModuleEventType::SYSTEM_COMMAND, this);
    m_kernel->registerToEvent(ModuleEventType::GCODE_COMMAND, this);
    m_kernel->registerToEvent(ModuleEventType::MCODE_COMMAND, this);
}

CommandResult CommandValidator::onSystemCommandReceived(const SystemCommand& command, CommandSource source, uint32_t commandId)
{
    if (command != SystemCommand::HOMING)
    {
        return CommandResult::error("Invalid Command");
    }

    return CommandResult::ok();
}

CommandResult CommandValidator::onGCodeCommandReceived(const GCode& gcode, CommandSource source, uint32_t commandId)
{
    switch (gcode.code())
    {
        case 0:
        case 1:
            return validateG0G1(gcode);
            return validateG0G1(gcode);
        case 2:
        case 3:
            return validateG2G3(gcode);
            return validateG2G3(gcode);
        case 4:
            return validateG4(gcode);
        case 17:
        case 18:
        case 19:
            return validateG17G18G19(gcode);
        case 20:
        case 21:
            return validateG20G21(gcode);
        case 90:
        case 91:
            return validateG90G91(gcode);
    }
    return CommandResult::error("Invalid Command");
}

CommandResult CommandValidator::onMCodeCommandReceived(const MCode& mcode, CommandSource source, uint32_t commandId)
{
    switch (mcode.code())
    {
        case 3:
            return validateM3(mcode);
        case 5:
            return validateM5(mcode);
        case 17:
            return validateM17(mcode);
        case 18:
            return validateM18(mcode);
        case 220:
            return validateM220(mcode);
    }
    return CommandResult::error("Invalid Command");
}

CommandResult CommandValidator::validateG0G1(const GCode& gcode)
{
    CHECK_EXTRA_SUBCODE(gcode);

    CHECK_EXTRA_PARAMETER(gcode, i, "I");
    CHECK_EXTRA_PARAMETER(gcode, j, "J");
    CHECK_EXTRA_PARAMETER(gcode, k, "K");
    CHECK_EXTRA_PARAMETER(gcode, p, "P");
    CHECK_EXTRA_PARAMETER(gcode, r, "R");
    CHECK_EXTRA_PARAMETER(gcode, l, "L");

    return CommandResult::ok();
}

CommandResult CommandValidator::validateG2G3(const GCode& gcode)
{
    CHECK_EXTRA_PARAMETER(gcode, l, "L");
    return CommandResult::ok();
}

CommandResult CommandValidator::validateG4(const GCode& gcode)
{
    CHECK_EXTRA_SUBCODE(gcode);

    if (gcode.s().has_value() && gcode.p().has_value())
    {
        return CommandResult::error("P and S is specified");
    }
    else if (!gcode.s().has_value() && !gcode.p().has_value())
    {
        return CommandResult::error("P or S must be specified");
    }

    return CommandResult::ok();
}

CommandResult CommandValidator::validateG17G18G19(const GCode& gcode)
{
    CHECK_EXTRA_SUBCODE(gcode);
    CHECK_GCODE_NO_PARAMETER(gcode);
    return CommandResult::ok();
}

CommandResult CommandValidator::validateG20G21(const GCode& gcode)
{
    CHECK_EXTRA_SUBCODE(gcode);
    CHECK_GCODE_NO_PARAMETER(gcode);
    return CommandResult::ok();
}

CommandResult CommandValidator::validateG90G91(const GCode& gcode)
{
    CHECK_EXTRA_SUBCODE(gcode);
    CHECK_GCODE_NO_PARAMETER(gcode);
    return CommandResult::ok();
}

CommandResult CommandValidator::validateM3(const MCode& mcode)
{
    CHECK_EXTRA_SUBCODE(mcode);
    CHECK_MISSING_PARAMETER(mcode, s, "S");

    CHECK_EXTRA_PARAMETER(mcode, x, "X");
    CHECK_EXTRA_PARAMETER(mcode, y, "Y");
    CHECK_EXTRA_PARAMETER(mcode, z, "Z");
    CHECK_EXTRA_PARAMETER_NULLPTR(mcode, path, "path");
    CHECK_EXTRA_PARAMETER(mcode, p, "P");
    CHECK_EXTRA_PARAMETER(mcode, i, "I");
    CHECK_EXTRA_PARAMETER(mcode, d, "D");

    return CommandResult::ok();
}

CommandResult CommandValidator::validateM5(const MCode& mcode)
{
    CHECK_EXTRA_SUBCODE(mcode);
    CHECK_MCODE_NO_PARAMETER(mcode);
    return CommandResult::ok();
}

CommandResult CommandValidator::validateM17(const MCode& mcode)
{
    CHECK_EXTRA_SUBCODE(mcode);
    CHECK_MCODE_NO_PARAMETER(mcode);
    return CommandResult::ok();
}

CommandResult CommandValidator::validateM18(const MCode& mcode)
{
    CHECK_EXTRA_SUBCODE(mcode);
    CHECK_MCODE_NO_PARAMETER(mcode);
    return CommandResult::ok();
}

CommandResult CommandValidator::validateM220(const MCode& mcode)
{
    CHECK_EXTRA_SUBCODE(mcode);
    CHECK_MCODE_NO_PARAMETER(mcode);
    return CommandResult::ok();
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

    const QList<GCodeLine>& lines() const;

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

const QList<GCodeLine>& LineCreator::lines() const
{
    return m_lines;
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
            static_cast<int>(commandId),
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
                static_cast<int>(commandId),
                fast});
            m_startPoint = endPoint;
        }
        m_kernel->dispatchTargetPosition(m_startPoint);
    }

    return CommandResult::ok();
}


GCodeModel::GCodeModel() : m_completedCommandCount(0) {}

void GCodeModel::load(const QString& path)
{
    m_commands.clear();
    m_lines.clear();
    m_completedCommandCount = 0;

    readCommands(path);
    QStringList invalidCommands;
    if (!validateCommands(invalidCommands) || !calculateLines(invalidCommands))
    {
        m_commands.clear();
        m_lines.clear();
        emit invalidGCode(invalidCommands);
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

bool GCodeModel::validateCommands(QStringList& invalidCommands)
{
    GuiKernel kernel(invalidCommands);
    CommandValidator commandValidator;

    kernel.addModule(&commandValidator);

    commandValidator.begin();

    tl::optional<uint32_t> commandId;
    for (auto& command : m_commands)
    {
        kernel.executeCommand(command.toStdString().c_str(), CommandSource::FILE_SOURCE, commandId);
    }

    return invalidCommands.empty();
}

bool GCodeModel::calculateLines(QStringList& invalidCommands)
{
    GuiKernel kernel(invalidCommands);
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
    for (auto& command : m_commands)
    {
        kernel.executeCommand(command.toStdString().c_str(), CommandSource::FILE_SOURCE, commandId);
    }

    return invalidCommands.empty();
}
