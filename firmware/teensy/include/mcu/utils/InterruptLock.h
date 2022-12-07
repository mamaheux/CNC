#ifndef MCU_UTILS_INTERRUPT_LOCK
#define MCU_UTILS_INTERRUPT_LOCK

class InterruptLock
{
public:
    InterruptLock();
    ~InterruptLock();
};

class PinInterruptLock
{
public:
    PinInterruptLock();
    ~PinInterruptLock();
};

class TimerInterruptLock
{
public:
    TimerInterruptLock();
    ~TimerInterruptLock();
};

#endif
