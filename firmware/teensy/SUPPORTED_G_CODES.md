# Supported G-Codes

## G-Code Axis Names
- X
- Y
- Z
- A (not supported)
- B (not supported)
- C (not supported)
- E (not supported)
- U (not supported)
- V (not supported)
- W (not supported)


## Modal State
- Machine Coordinates
- Worskspace Coordinates (9)
- `G0` Feedrate
- `G1`, `G2`, `G3` Feedrate
- Predefined Position (`G28.1`)
- Global Offset (`G92`)


## Units
| Name           | Units               |
| -------------- | ------------------- |
| Axis X, Y, Z   | mm or in            |
| Feedrate       | mm or in per minute |
| Spindle Speed  | RPM                 |


## System Commands (GRBL)
| Name   | Command |
| ------ | ------- |
| Homing | `$H`    |


## G-Codes

| G-Code     | Documentation                                                                                                         | Parameters                                     | Example                |
| ---------- | --------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------- | ---------------------- |
| `G0`       | [Link 1](https://smoothieware.org/g0), [Link 2](http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g0)             | Axes, feedrate                                 | `G0 X5 F50`            |
| `G1`       | [Link 1](https://smoothieware.org/g1), [Link 2](http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g1)             | Axes, feedrate                                 | `G0 X5 F50`            |
| `G2`       | [Link 1](https://smoothieware.org/g2), [Link 2](http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g2-g3)          | Axes, center offsets (I, J, K), feedrate       | `G2 X5 Y5 I-5 J5 F10`  |
| `G3`       | [Link 1](https://smoothieware.org/g3), [Link 2](http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g2-g3)          | Axes, center offsets (I, J, K), feedrate       | `G3 X5 Y5 I-5 J5 F10`  |
| `G4`       | [Link 1](https://smoothieware.org/g4), [Link 2](http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g4)             | Second (S), milisecondes (P)                   | `G4 S2`                |
| `G10 L2`   | [Link](http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g10-l2)                                                  | Coordinate System (P), axes, R (not supported) | `G10 L2 P2 X1 Y2`      |
| `G10 L20`  | [Link](http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g10-l20)                                                 | Coordinate System (P), axes                    | `G10 L20 P3 X1`        |
| `G17`      | [Link 1](https://smoothieware.org/g17), [Link 2](http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g17-g19.1)     |                                                | `G17`                  |
| `G18`      | [Link 1](https://smoothieware.org/g18), [Link 2](http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g17-g19.1)     |                                                | `G18`                  |
| `G19`      | [Link 1](https://smoothieware.org/g18), [Link 2](http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g17-g19.1)     |                                                | `G19`                  |
| `G20`      | [Link 1](https://smoothieware.org/g20), [Link 2](http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g20-g21)       |                                                | `G20`                  |
| `G21`      | [Link 1](https://smoothieware.org/g21), [Link 2](http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g20-g21)       |                                                | `G21`                  |
| `G28`      | [Link 1](https://smoothieware.org/g28-cnc), [Link 2](http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g28-g28.1) | Axes (optional)                                | `G28`                  |
| `G28.1`    | [Link](http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g28-g28.1)                                               |                                                | `G28.1`                |
| `G53`      | [Link](http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g53)                                                     | G1 or G1 (modal command is permitted)          | `G53 G0 X0 Y0 Z0`      |
| `G54-59.3` | [Link](http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g54-g59.3)                                               |                                                | `G54`                  |
| `G90`      | [Link 1](https://smoothieware.org/g90), [Link 2](http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g90-g91)       |                                                | `G90`                  |
| `G91`      | [Link 1](https://smoothieware.org/g91), [Link 2](http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g90-g91)       |                                                | `G91`                  |
| `G92`      | [Link 1](https://smoothieware.org/g92), [Link 2](http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g92)           | Axes (optional)                                | `G92 X1`               |
| `G92.1`    | [Link](http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g92.1-g92.2)                                             | Axes (optional)                                | `G92.1`                |


## Supported G-Code Parameter Names
- X
- Y
- Z
- F
- I
- J
- K
- S
- P
- L


## M-Codes

| M-Code     | Documentation                                                                                           | Parameters        | Example               |
| ---------- | ------------------------------------------------------------------------------------------------------- | ----------------- | --------------------- |
| `M3`       | [Link](https://smoothieware.org/m3)                                                                     | Spindle Speed (S) | `M3 S500`             |
| `M4`       | [Link](https://smoothieware.org/m5)                                                                     |                   | `M5`                  |
| `M17`      | Enable all stepper motors.                                                                              |                   | `M17`                 |
| `M18`      | Disable all stepper motors.                                                                             |                   | `M18`                 |
| `M20`      | List all G-Code files.                                                                                  |                   | `M20`                 |
| `M23`      | Select a G-Code file.                                                                                   | File path         | `M23 path/file.gcode` |
| `M24`      | Start or resume the selected file.                                                                      |                   | `M24`                 |
| `M25`      | Pause the selected file.                                                                                |                   | `M25`                 |
| `M26`      | Abort the selected file.                                                                                |                   | `M26`                 |
| `M27`      | Report the progress of the selected file (line count).                                                  |                   | `M27`                 |
| `M28`      | Start to write the specified file.                                                                      | File path         | `M28 path/file.gcode` |
| `M29`      | End to write the current file.                                                                          |                   | `M29`                 |
| `M30`      | Delete the specified file.                                                                              | File path         | `M30 path/file.gcode` |
| `M32`      | Select a G-Code file and start the selected file.                                                       | File path         | `M32 path/file.gcode` |
| `M84`      | Disable all stepper motors.                                                                             |                   | `M84`                 |
| `M114`     | Print the last requested position in the selected coordinate system                                     |                   | `M114`                |
| `M114.1`   | Print the real time position in the selected coordinate system                                          |                   | `M114.1`              |
| `M114.2`   | Print the last requested position in the machine coordinate system                                      |                   | `M114.2`              |
| `M114.3`   | Print the real time position in the machine coordinate system                                           |                   | `M114.3`              |
| `M203`     | Set the maximum feedrate (current unit / sec).                                                          | Axes              | `M203 X200`           |
| `M204`     | Set the maximum feedrate (current unit^2 / sec).                                                        | Axes              | `M204 X200`           |
| `M220`     | Set speed factor (percent).                                                                             | Percent (S)       | `M220 S25`            |
| `M957`     | Print the spindle speed (RPM)                                                                           |                   | `M957`                |
| `M958`     | Print the spindle PID gain values.                                                                      |                   | `M958`                |
| `M958`     | Set the spindle PID gain values.                                                                        |                   | `M958 P0.1 I0.2 D0.2` |

## Supported G-Code Parameter Names
- S
- File parameter
- P
- I
- D
