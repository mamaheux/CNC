#include "mcu/criticalError.h"
#include "mcu/io/DigitalOutput.h"

static DigitalOutput led;

void setupCriticalErrorCheck() {
  led.begin(LED_PIN, false, false);
}

void toogleErrorLed() {
  led.write(true);
  delay(ERROR_DELAY_MS);
  led.write(false);
}