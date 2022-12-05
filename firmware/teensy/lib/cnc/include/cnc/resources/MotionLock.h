#ifndef CNC_RESOURCES_MOTION_LOCK_H
#define CNC_RESOURCES_MOTION_LOCK_H

#include <cnc/modules/Module.h>
#include <cnc/parsing/SystemCommand.h>
#include <cnc/parsing/GCode.h>
#include <cnc/parsing/MCode.h>
#include <cnc/utils/ClassMacro.h>

class MotionLock
{
protected:
    tl::optional<CommandSource> m_owner;

public:
    MotionLock() = default;
    ~MotionLock() = default;

    DECLARE_NOT_COPYABLE(MotionLock);
    DECLARE_NOT_MOVABLE(MotionLock);

    bool tryLock(const SystemCommand& command, CommandSource source);
    bool tryLock(const GCode& command, CommandSource source);
    bool tryLock(const MCode& command, CommandSource source);

    void unlock();

private:
    bool tryLock(CommandSource source);
};

#endif
