#include "mcu/criticalError.h"
#include "mcu/io/DigitalOutput.h"

static DigitalOutput led;

FLASHMEM void setupCriticalErrorCheck()
{
    led.begin(DigitalOutputConfig(LED_PIN, false), false);
}

FLASHMEM void toogleErrorLed()
{
    led.write(true);
    delay(ERROR_DELAY_MS);
    led.write(false);
}
