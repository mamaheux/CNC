#include "config.h"
#include "mcu/criticalError.h"

#include "mcu/modules/Kernel.h"
#include "mcu/modules/FileSystem.h"
#include "mcu/modules/CommandSerial.h"
#include "mcu/modules/Stepper.h"

#include <cnc/modules/CoordinateTransformer.h>

#include <SD.h>

FileSystem fileSystem;
CommandSerial commandSerial;

Stepper xStepper(StepperAxis::X);
Stepper yStepper(StepperAxis::Y);
Stepper zStepper(StepperAxis::Z);

CoordinateTransformer coordinateTransformer;

Kernel kernel;

void setupKernel() {
  DEBUG_SERIAL.println("Setup the kernel");

  kernel.addModule(&fileSystem);
  kernel.addModule(&commandSerial);

  kernel.addModule(&xStepper);
  kernel.addModule(&yStepper);
  kernel.addModule(&zStepper);

  kernel.addModule(&coordinateTransformer);
  kernel.begin();
}

void setup() {
  DEBUG_SERIAL.begin(DEBUG_SERIAL_BAUD_RATE);
  setupCriticalErrorCheck();
  SD.begin(BUILTIN_SDCARD);

  setupKernel();
}

void loop() {
  kernel.update();
}
