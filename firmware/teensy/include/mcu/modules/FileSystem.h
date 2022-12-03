#ifndef MCU_MODULES_FILE_SYSTEM_H
#define MCU_MODULES_FILE_SYSTEM_H

#include <cnc/modules/Module.h>
#include <cnc/parsing/MCode.h>

#include <SD.h>

constexpr size_t MAX_FILE_SYSTEM_RESPONSE_SIZE = 512;

class FileSystem : public Module
{
    File m_newFile;
    MCodeParser m_mcodeParser;

    char m_response[MAX_FILE_SYSTEM_RESPONSE_SIZE];

public:
    FileSystem();
    ~FileSystem() override = default;

    DECLARE_NOT_COPYABLE(FileSystem);
    DECLARE_NOT_MOVABLE(FileSystem);

    void configure(const ConfigItem& item) override;
    void checkConfigErrors(std::function<void(const char*, const char*, const char*)> onMissingConfigItem) override;
    void begin() override;

    RawCommandResult onRawCommandReceived(const char* line, CommandSource source, uint32_t commandId) override;
    CommandResult onMCodeCommandReceived(const MCode& mcode, CommandSource source, uint32_t commandId) override;

private:
    void listFiles(File& directory, size_t spaceCount, CommandSource source, uint32_t commandId);
    void sendFileEntry(File& file, size_t spaceCount, CommandSource source, uint32_t commandId);

    CommandResult startNewFile(const char* path);
    void writeOrStopNewFile(const char* line, CommandSource source, uint32_t commandId);
    CommandResult deleteFile(const char* path);
};

#endif
