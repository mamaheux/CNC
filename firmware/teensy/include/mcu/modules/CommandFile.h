#ifndef MCU_MODULES_COMMAND_FILE_H
#define MCU_MODULES_COMMAND_FILE_H

#include <cnc/modules/Module.h>

#include <tl/optional.hpp>

#include <SD.h>

constexpr size_t COMMAND_FILE_LINE_BUFFER_SIZE = 256;
constexpr size_t MAX_COMMAND_FILE_RESPONSE_SIZE = 64;

class CommandFile : public Module
{
    File m_commandFile;
    size_t m_completedLineCount;
    size_t m_lineCount;
    File m_logFile;
    bool m_isStarted;

    size_t m_lineIndex;
    char m_lineBuffer[COMMAND_FILE_LINE_BUFFER_SIZE];
    tl::optional<uint32_t> m_pendingCommandId;

    char m_response[MAX_COMMAND_FILE_RESPONSE_SIZE];

public:
    CommandFile();
    ~CommandFile() override = default;

    DECLARE_NOT_COPYABLE(CommandFile);
    DECLARE_NOT_MOVABLE(CommandFile);

    void configure(const ConfigItem& item) override;
    void checkConfigErrors(std::function<void(const char*, const char*, const char*)> onMissingConfigItem) override;
    void begin() override;

    void update() override;

    CommandResult onMCodeCommandReceived(const MCode& mcode, CommandSource source, uint32_t commandId) override;
    void onCommandResponse(const char* response, CommandSource source, uint32_t commandId, bool isComplete) override;

private:
    CommandResult openFiles(const MCode& mcode);
    size_t countLines();
    void closeFiles();

    void sendProgress(CommandSource source, uint32_t commandId);
};

#endif
