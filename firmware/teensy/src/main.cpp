#include "config.h"
#include "mcu/criticalError.h"

#include "mcu/modules/Kernel.h"
#include "mcu/modules/FileSystem.h"
#include "mcu/modules/CommandSerial.h"
#include "mcu/modules/CommandFile.h"
#include "mcu/modules/McuPlanner.h"
#include "mcu/modules/StepperController.h"
#include "mcu/modules/Endstops.h"
#include "mcu/modules/Spindle.h"
#include "mcu/modules/LinearBlockExecutor.h"

#include <cnc/modules/CoordinateTransformer.h>
#include <cnc/modules/ArcConverter.h>

#include <SD.h>

FileSystem fileSystem;
CommandSerial commandSerial;
CommandFile commandFile;

CoordinateTransformer coordinateTransformer;
ArcConverter arcConverter(&coordinateTransformer);
McuPlanner planner(&coordinateTransformer, &arcConverter);

StepperController stepperController(&coordinateTransformer, &planner);
Endstops endstops(&planner, &stepperController);
Spindle spindle;
LinearBlockExecutor linearBlockExecutor(&stepperController, &spindle);

Kernel kernel;

FLASHMEM void setupKernel()
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
    kernel.addModule(&linearBlockExecutor);
    kernel.begin();
}

FLASHMEM void setup()
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
