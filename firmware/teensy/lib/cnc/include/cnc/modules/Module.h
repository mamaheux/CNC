#ifndef CNC_MODULES_MODULE_H
#define CNC_MODULES_MODULE_H

#include <cnc/config/ConfigItem.h>
#include <cnc/math/Vector3.h>
#include <cnc/parsing/SystemCommand.h>
#include <cnc/parsing/GCode.h>
#include <cnc/parsing/MCode.h>
#include <cnc/utils/macro.h>
#include <cnc/utils/ClassMacro.h>

#include <functional>

constexpr const char* OK_COMMAND_RESPONSE = "ok";
constexpr const char* DEFAULT_COMMAND_ERROR_MESSAGE = "";

enum class RawCommandResult
{
    HANDLED,
    NOT_HANDLED,
};

enum class CommandResultType
{
    OK,
    OK_RESPONSE_SENT,
    PENDING,
    ERROR,
    NOT_HANDLED,
};

class CommandResult
{
    CommandResultType m_type;
    const char* m_errorMessage;

    explicit CommandResult(CommandResultType type);
    explicit CommandResult(const char* errorMessage);

public:
    static CommandResult ok();
    static CommandResult okResponseSent();
    static CommandResult pending();
    static CommandResult error(const char* message);
    static CommandResult notHandled();

    CommandResultType type() const;
    const char* errorMessage() const;

    CommandResult agregate(const CommandResult& r) const;
};

inline CommandResult::CommandResult(CommandResultType type)
    : m_type(type),
      m_errorMessage(DEFAULT_COMMAND_ERROR_MESSAGE)
{
}

inline CommandResult::CommandResult(const char* errorMessage)
    : m_type(CommandResultType::ERROR),
      m_errorMessage(errorMessage)
{
}

inline CommandResult CommandResult::ok()
{
    return CommandResult(CommandResultType::OK);
}

inline CommandResult CommandResult::okResponseSent()
{
    return CommandResult(CommandResultType::OK_RESPONSE_SENT);
}

inline CommandResult CommandResult::pending()
{
    return CommandResult(CommandResultType::PENDING);
}

inline CommandResult CommandResult::error(const char* message)
{
    return CommandResult(message);
}

inline CommandResult CommandResult::notHandled()
{
    return CommandResult(CommandResultType::NOT_HANDLED);
}

inline CommandResultType CommandResult::type() const
{
    return m_type;
}

inline const char* CommandResult::errorMessage() const
{
    return m_errorMessage;
}

inline CommandResult CommandResult::agregate(const CommandResult& r) const
{
    if (type() == CommandResultType::ERROR)
    {
        return CommandResult::error(errorMessage());
    }
    else if (r.type() == CommandResultType::ERROR)
    {
        return CommandResult::error(r.errorMessage());
    }
    else if (type() == CommandResultType::OK_RESPONSE_SENT || r.type() == CommandResultType::OK_RESPONSE_SENT)
    {
        return CommandResult::okResponseSent();
    }
    else if (type() == CommandResultType::PENDING || r.type() == CommandResultType::PENDING)
    {
        return CommandResult::pending();
    }
    else if (type() == CommandResultType::OK || r.type() == CommandResultType::OK)
    {
        return CommandResult::ok();
    }
    return CommandResult::notHandled();
}

enum class ModuleEventType : size_t
{
    RAW_COMMAND,
    SYSTEM_COMMAND,
    GCODE_COMMAND,
    MCODE_COMMAND,
    TARGET_POSITION,
    COMMAND_RESPONSE,
    COUNT
};

constexpr size_t MODULE_EVENT_TYPE_COUNT = static_cast<size_t>(ModuleEventType::COUNT);

enum class CommandSource : size_t
{
    SERIAL_SOURCE,
    FILE_SOURCE,
    COUNT
};

constexpr size_t COMMAND_SOURCE_COUNT = static_cast<size_t>(CommandSource::COUNT);

class ModuleKernel;

typedef std::function<void(const char* key, const char* function, const char* filename)> MissingConfigCallback;

class Module
{
protected:
    ModuleKernel* m_kernel;

public:
    Module();
    virtual ~Module() = default;

    DECLARE_NOT_COPYABLE(Module);
    DECLARE_NOT_MOVABLE(Module);

    virtual void configure(const ConfigItem& item) = 0;
    virtual void checkConfigErrors(const MissingConfigCallback& onMissingConfigItem) = 0;
    virtual void begin() = 0;

    void setKernel(ModuleKernel* kernel);

    virtual RawCommandResult onRawCommandReceived(const char* line, CommandSource source, uint32_t commandId);

    virtual CommandResult
        onSystemCommandReceived(const SystemCommand& command, CommandSource source, uint32_t commandId);
    virtual CommandResult onGCodeCommandReceived(const GCode& gcode, CommandSource source, uint32_t commandId);
    virtual CommandResult onMCodeCommandReceived(const MCode& mcode, CommandSource source, uint32_t commandId);

    virtual void onTargetPositionChanged(const Vector3<float>& machinePosition);  // In mm

    virtual void onCommandResponse(const char* response, CommandSource source, uint32_t commandId, bool isComplete);

    virtual void update();
};

#define CHECK_CONFIG_ERROR(callback, condition, key)                                                                   \
    if (!(condition))                                                                                                  \
    {                                                                                                                  \
        (callback)(key, __FUNCTION_NAME__, __FILENAME__);                                                              \
    }

#endif
