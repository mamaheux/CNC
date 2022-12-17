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
- `G0` Feed Rate
- `G1`, `G2`, `G3` Feed Rate
- Global Offset (`G92`)


## Units
| Name           | Units               |
| -------------- | ------------------- |
| Axis X, Y, Z   | mm or in            |
| Feedrate       | mm or in per minute |
| Spindle Speed  | RPM                 |


## System Commands (GRBL)
| Name   | Command | Handled | Tested |
| ------ | ------- | ------- | ------ |
| Homing | `$H`    | Yes     |        |


## G-Codes

| G-Code     | Documentation                                                                                                         | Parameters                                                                                     | Example               | Handled | Tested |
| ---------- | --------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------- | --------------------- | ------- | ------ |
| `G0`       | [Link 1](https://smoothieware.org/g0), [Link 2](http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g0)             | Axes, feedrate (mm/min), spindle speed                                                         | `G0 X5 F50`           | Yes     |        |
| `G1`       | [Link 1](https://smoothieware.org/g1), [Link 2](http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g1)             | Axes, feedrate (mm/min), spindle speed                                                         | `G1 X5 F50`           | Yes     |        |
| `G2`       | [Link 1](https://smoothieware.org/g2), [Link 2](http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g2-g3)          | Axes, center offsets (I, J, K) or radius (R), turn count (P), feedrate (mm/min), spindle speed | `G2 X5 Y5 I-5 J5 F10` | Yes     |        |
| `G3`       | [Link 1](https://smoothieware.org/g3), [Link 2](http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g2-g3)          | Axes, center offsets (I, J, K) or radius (R), turn count (P), feedrate (mm/min), spindle speed | `G3 X5 Y5 I-5 J5 F10` | Yes     |        |
| `G10 L2`   | [Link](http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g10-l2)                                                  | Coordinate System (P), axes, R                                                                 | `G10 L2 P2 X1 Y2`     | Yes     |        |
| `G10 L20`  | [Link](http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g10-l20)                                                 | Coordinate System (P), axes                                                                    | `G10 L20 P3 X1`       | Yes     |        |
| `G17`      | [Link 1](https://smoothieware.org/g17), [Link 2](http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g17-g19.1)     |                                                                                                | `G17`                 | Yes     |        |
| `G18`      | [Link 1](https://smoothieware.org/g18), [Link 2](http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g17-g19.1)     |                                                                                                | `G18`                 | Yes     |        |
| `G19`      | [Link 1](https://smoothieware.org/g19), [Link 2](http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g17-g19.1)     |                                                                                                | `G19`                 | Yes     |        |
| `G20`      | [Link 1](https://smoothieware.org/g20), [Link 2](http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g20-g21)       |                                                                                                | `G20`                 | Yes     |        |
| `G21`      | [Link 1](https://smoothieware.org/g21), [Link 2](http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g20-g21)       |                                                                                                | `G21`                 | Yes     |        |
| `G53`      | [Link](http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g53)                                                     | G1 or G1 (modal commands are permitted)                                                        | `G53 G0 X0 Y0 Z0`     | Yes     |        |
| `G54-59.3` | [Link](http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g54-g59.3)                                               |                                                                                                | `G54`                 | Yes     |        |
| `G90`      | [Link 1](https://smoothieware.org/g90), [Link 2](http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g90-g91)       |                                                                                                | `G90`                 | Yes     |        |
| `G90.1`    | [Link 2](http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g90.1-g91.1)                                           |                                                                                                | `G90.1`               | Yes     |        |
| `G91`      | [Link 1](https://smoothieware.org/g91), [Link 2](http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g90-g91)       |                                                                                                | `G91`                 | Yes     |        |
| `G91.1`    | [Link 2](http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g90.1-g91.1)                                           |                                                                                                | `G91.1`               | Yes     |        |
| `G92`      | [Link 1](https://smoothieware.org/g92), [Link 2](http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g92)           | Axes (optional)                                                                                | `G92 X1`              | Yes     |        |
| `G92.1`    | [Link](http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g92.1-g92.2)                                             | Axes (optional)                                                                                | `G92.1`               | Yes     |        |


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
- R
- L


## M-Codes

| M-Code     | Documentation                                                              | Parameters        | Example          | Handled | Tested |
| ---------- | -------------------------------------------------------------------------- | ----------------- | ---------------- | ------- | ------ |
| `M3`       | [Link](https://smoothieware.org/m3)                                        | Spindle Speed (S) | `M3 S500`        | Yes     |        |
| `M5`       | [Link](https://smoothieware.org/m5)                                        |                   | `M5`             | Yes     |        |
| `M17`      | Enable all stepper motors.                                                 |                   | `M17`            | Yes     |        |
| `M18`      | Disable all stepper motors.                                                |                   | `M18`            | Yes     |        |
| `M20`      | List all files.                                                            |                   | `M20`            | Yes     | Yes    |
| `M23`      | Select a G-Code file.                                                      | File path         | `M23 file.gcode` | Yes     | Yes    |
| `M24`      | Start or resume the selected file.                                         |                   | `M24`            | Yes     |        |
| `M25`      | Pause the selected file.                                                   |                   | `M25`            | Yes     |        |
| `M26`      | Abort the selected file.                                                   |                   | `M26`            | Yes     | Yes    |
| `M27`      | Report the progress of the selected file (line count).                     |                   | `M27`            | Yes     | Yes    |
| `M28`      | Start to write the specified file.                                         | File path         | `M28 file.gcode` | Yes     | Yes    |
| `M29`      | End to write the current file.                                             |                   | `M29`            | Yes     | Yes    |
| `M30`      | Delete the specified file.                                                 | File path         | `M30 file.gcode` | Yes     | Yes    |
| `M32`      | Select a G-Code file and start the selected file.                          | File path         | `M32 file.gcode` | Yes     |        |
| `M114`     | Print the last requested position in the selected coordinate system        |                   | `M114`           | Yes     |        |
| `M114.1`   | Print the real time position in the selected coordinate system             |                   | `M114.1`         | Yes     |        |
| `M114.2`   | Print the last requested position in the machine coordinate system         |                   | `M114.2`         | Yes     |        |
| `M114.3`   | Print the real time position in the machine coordinate system              |                   | `M114.3`         | Yes     |        |
| `M203`     | Print the maximum feed rate values and set the maximum feed rate (mm/sec). | Feed rate (S)     | `M203 S200`      | Yes     | Yes    |
| `M204`     | Print the acceleration values and set the acceleration (mm^2/sec).         | Acceleration (S)  | `M204 S200`      | Yes     | Yes    |
| `M220`     | Print the speed factor and set the speed factor (percent).                 | Percent (S)       | `M220 S25`       | Yes     | Yes    |
| `M957`     | Print the spindle speed (RPM)                                              |                   | `M957`           | Yes     | Yes    |
| `M958`     | Print the spindle PID gain values and set the spindle PID gain values.     | P, I, D           | `M958 P0.1 I0.2` | Yes     | Yes    |

## Supported M-Code Parameter Names
- S
- File parameter
- P
- I
- D
