#include <cnc/resources/MotionLock.h>

bool MotionLock::tryLock(const SystemCommand& command, CommandSource source)
{
    switch (command)
    {
        case SystemCommand::HOMING:
            return tryLock(source);
        default:
            return true;
    }
}

bool MotionLock::tryLock(const GCode& command, CommandSource source)
{
    return tryLock(source);
}

bool MotionLock::tryLock(const MCode& command, CommandSource source)
{
    switch (command.code())
    {
        case 3:
        case 5:
        case 17:
        case 18:
        case 24:
        case 32:
            return tryLock(source);
        default:
            return true;
    }
}

void MotionLock::unlock()
{
    m_owner = tl::nullopt;
}

bool MotionLock::tryLock(CommandSource source)
{
    if (m_owner == source || m_owner == tl::nullopt)
    {
        m_owner = source;
        return true;
    }

    return false;
}
