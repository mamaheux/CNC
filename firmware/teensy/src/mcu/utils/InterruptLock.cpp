#include "mcu/utils/InterruptLock.h"

#include <Arduino.h>

InterruptLock::InterruptLock()
{
    noInterrupts();
}

InterruptLock::~InterruptLock()
{
    interrupts();
}

PinInterruptLock::PinInterruptLock()
{
    NVIC_DISABLE_IRQ(IRQ_GPIO1_0_15);
    NVIC_DISABLE_IRQ(IRQ_GPIO1_16_31);
    NVIC_DISABLE_IRQ(IRQ_GPIO2_0_15);
    NVIC_DISABLE_IRQ(IRQ_GPIO2_16_31);
    NVIC_DISABLE_IRQ(IRQ_GPIO3_0_15);
    NVIC_DISABLE_IRQ(IRQ_GPIO3_16_31);
    NVIC_DISABLE_IRQ(IRQ_GPIO4_0_15);
    NVIC_DISABLE_IRQ(IRQ_GPIO4_16_31);
    NVIC_DISABLE_IRQ(IRQ_GPIO5_0_15);
    NVIC_DISABLE_IRQ(IRQ_GPIO5_16_31);
}

PinInterruptLock::~PinInterruptLock()
{
    NVIC_ENABLE_IRQ(IRQ_GPIO1_0_15);
    NVIC_ENABLE_IRQ(IRQ_GPIO1_16_31);
    NVIC_ENABLE_IRQ(IRQ_GPIO2_0_15);
    NVIC_ENABLE_IRQ(IRQ_GPIO2_16_31);
    NVIC_ENABLE_IRQ(IRQ_GPIO3_0_15);
    NVIC_ENABLE_IRQ(IRQ_GPIO3_16_31);
    NVIC_ENABLE_IRQ(IRQ_GPIO4_0_15);
    NVIC_ENABLE_IRQ(IRQ_GPIO4_16_31);
    NVIC_ENABLE_IRQ(IRQ_GPIO5_0_15);
    NVIC_ENABLE_IRQ(IRQ_GPIO5_16_31);
}

TimerInterruptLock::TimerInterruptLock()
{
    NVIC_DISABLE_IRQ(IRQ_PIT);
}

TimerInterruptLock::~TimerInterruptLock()
{
    NVIC_ENABLE_IRQ(IRQ_PIT);
}
