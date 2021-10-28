#include "config.h"
#include "core/criticalError.h"
#include "Kernel.h"

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
}

void loop() {
  // put your main code here, to run repeatedly:
}
