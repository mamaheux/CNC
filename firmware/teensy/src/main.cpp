#include "config.h"
#include "mcu/criticalError.h"

#include "mcu/modules/Kernel.h"
#include "mcu/modules/FileSystem.h"
#include "mcu/modules/CommandSerial.h"
#include "mcu/modules/CommandFile.h"
#include "mcu/modules/Endstops.h"
#include "mcu/modules/StepperController.h"
#include "mcu/modules/Spindle.h"

#include <cnc/modules/CoordinateTransformer.h>
#include <cnc/modules/ArcConverter.h>
#include <cnc/modules/Planner.h>

#include <SD.h>

FileSystem fileSystem;
CommandSerial commandSerial;
CommandFile commandFile;

CoordinateTransformer coordinateTransformer;
ArcConverter arcConverter(&coordinateTransformer);
Planner planner(&coordinateTransformer, &arcConverter);

Endstops endstops;
StepperController stepperController(&coordinateTransformer, &planner);
Spindle spindle;

Kernel kernel;

void setupKernel()
{
    DEBUG_SERIAL.println("Setup the kernel");

    kernel.addModule(&fileSystem);
    kernel.addModule(&commandSerial);
    kernel.addModule(&commandFile);

    kernel.addModule(&coordinateTransformer);
    kernel.addModule(&arcConverter);
    kernel.addModule(&planner);

    kernel.addModule(&endstops);
    kernel.addModule(&stepperController);
    kernel.addModule(&spindle);
    kernel.begin();
}

void setup()
{
    DEBUG_SERIAL.begin(DEBUG_SERIAL_BAUD_RATE);
    setupCriticalErrorCheck();
    SD.begin(BUILTIN_SDCARD);

    setupKernel();
}

void loop()
{
    kernel.update();
}
