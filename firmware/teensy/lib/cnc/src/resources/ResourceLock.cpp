#include <cnc/resources/ResourceLock.h>
#include <cnc/resources/resources.h>

#include <cmath>

using namespace std;

enum class LockState
{
    UNLOCKED,
    LOCKED,
};

static volatile LockState pinLockStates[PIN_COUNT] = {LockState::UNLOCKED};
static volatile uint8_t pwmGroupPinCount[PWM_GROUP_COUNT] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                             0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static volatile float pwmGroupFrequency[PWM_GROUP_COUNT] = {NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN,
                                                            NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN};

PinLock::PinLock() : m_pin(0), m_hasLock(false) {}

PinLock::~PinLock()
{
    if (m_hasLock)
    {
        pinLockStates[m_pin] = LockState::UNLOCKED;
        m_hasLock = false;
    }
}

bool PinLock::tryLock(uint8_t pin)
{
    m_pin = pin;

    if (pinLockStates[m_pin] == LockState::LOCKED)
    {
        return false;
    }
    else
    {
        pinLockStates[m_pin] = LockState::LOCKED;
        m_hasLock = true;
        return true;
    }
}

PwmLock::PwmLock() : PinLock(), m_frequency(nan("")) {}

PwmLock::~PwmLock()
{
    uint8_t pwmGroup = PWM_GROUP_BY_PIN[m_pin];
    if (m_hasLock && pwmGroup != INVALID_PWM_GROUP)
    {
        pwmGroupPinCount[pwmGroup]--;
        if (pwmGroupPinCount[pwmGroup] == 0)
        {
            pwmGroupFrequency[pwmGroup] = nanf("");
        }
    }
}

bool PwmLock::tryLock(uint8_t pin, float frequency)
{
    m_pin = pin;
    m_frequency = frequency;

    uint8_t pwmGroup = PWM_GROUP_BY_PIN[m_pin];
    if (pwmGroup == INVALID_PWM_GROUP)
    {
        return false;
    }

    float otherFrequency = pwmGroupFrequency[pwmGroup];
    if (!isnan(otherFrequency) && otherFrequency != m_frequency)
    {
        return false;
    }
    else if (!PinLock::tryLock(pin))
    {
        return false;
    }
    else
    {
        pwmGroupPinCount[pwmGroup]++;
        pwmGroupFrequency[pwmGroup] = m_frequency;
        return true;
    }
}
