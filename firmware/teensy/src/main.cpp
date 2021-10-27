#include "config.h"

#include <SD.h>

void setup() {
  DEBUG_SERIAL.begin(DEBUG_SERIAL_BAUD_RATE);
  SD.begin(BUILTIN_SDCARD);
}

void loop() {
  // put your main code here, to run repeatedly:
}
