#include "config.h"
#include "mcu/criticalError.h"
#include "mcu/modules/Kernel.h"

#include <SD.h>

Kernel kernel;

void setupKernel() {
  DEBUG_SERIAL.println("Setup the kernel");
  kernel.begin();
}

void setup() {
  DEBUG_SERIAL.begin(DEBUG_SERIAL_BAUD_RATE);
  setupCriticalErrorCheck();
  SD.begin(BUILTIN_SDCARD);

  setupKernel();
}

void loop() {
  // put your main code here, to run repeatedly:
}
