EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 6
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 850  750  900  500 
U 6176DC1F
F0 "power" 50
F1 "power.sch" 50
$EndSheet
$Sheet
S 850  1500 900  900 
U 617EEDF3
F0 "endstops" 50
F1 "endstops.sch" 50
F2 "X_MIN" O R 1750 1600 50 
F3 "X_MAX" O R 1750 1700 50 
F4 "Y_MIN" O R 1750 1800 50 
F5 "Y_MAX" O R 1750 1900 50 
F6 "Z_MIN" O R 1750 2000 50 
F7 "Z_MAX" O R 1750 2100 50 
F8 "Other1" O R 1750 2200 50 
F9 "Other2" O R 1750 2300 50 
$EndSheet
$Sheet
S 850  2650 900  1100
U 6177BB14
F0 "steppers" 50
F1 "steppers.sch" 50
F2 "X_EN" I R 1750 2750 50 
F3 "X_DIR" I R 1750 2850 50 
F4 "X_STEP" I R 1750 2950 50 
F5 "Y_EN" I R 1750 3100 50 
F6 "Y_DIR" I R 1750 3200 50 
F7 "Y_STEP" I R 1750 3300 50 
F8 "Z_EN" I R 1750 3450 50 
F9 "Z_DIR" I R 1750 3550 50 
F10 "Z_STEP" I R 1750 3650 50 
$EndSheet
$Sheet
S 850  4000 900  800 
U 617D972F
F0 "spindle" 50
F1 "spindle.sch" 50
F2 "BLDC_PWM" I R 1750 4100 50 
F3 "BLDC_FEEDBACK" O R 1750 4200 50 
F4 "BLDC_EN" I R 1750 4300 50 
F5 "DC_PWM_1" I R 1750 4500 50 
F6 "DC_PWM_2" I R 1750 4600 50 
$EndSheet
$Sheet
S 1950 1500 1050 3300
U 61943338
F0 "mcu" 50
F1 "mcu.sch" 50
F2 "X_MIN_ENDSTOP" I L 1950 1600 50 
F3 "X_MAX_ENDSTOP" I L 1950 1700 50 
F4 "Y_MIN_ENDSTOP" I L 1950 1800 50 
F5 "Y_MAX_ENDSTOP" I L 1950 1900 50 
F6 "Z_MIN_ENDSTOP" I L 1950 2000 50 
F7 "Z_MAX_ENDSTOP" I L 1950 2100 50 
F8 "OTHER_1_ENDSTOP" I L 1950 2200 50 
F9 "OTHER_2_ENDSTOP" I L 1950 2300 50 
F10 "X_STEPPER_EN" O L 1950 2750 50 
F11 "X_STEPPER_DIR" O L 1950 2850 50 
F12 "X_STEPPER_STEP" O L 1950 2950 50 
F13 "Y_STEPPER_EN" O L 1950 3100 50 
F14 "Y_STEPPER_DIR" O L 1950 3200 50 
F15 "Y_STEPPER_STEP" O L 1950 3300 50 
F16 "Z_STEPPER_EN" O L 1950 3450 50 
F17 "Z_STEPPER_DIR" O L 1950 3550 50 
F18 "Z_STEPPER_STEP" O L 1950 3650 50 
F19 "BLDC_MOTOR_PWM" O L 1950 4100 50 
F20 "BLDC_MOTOR_EN" O L 1950 4300 50 
F21 "BLDC_MOTOR_FEEDBACK" I L 1950 4200 50 
F22 "DC_MOTOR_PWM_1" O L 1950 4500 50 
F23 "DC_MOTOR_PWM_2" O L 1950 4600 50 
$EndSheet
Wire Wire Line
	1950 1600 1750 1600
Wire Wire Line
	1750 1700 1950 1700
Wire Wire Line
	1950 1800 1750 1800
Wire Wire Line
	1750 1900 1950 1900
Wire Wire Line
	1950 2000 1750 2000
Wire Wire Line
	1750 2100 1950 2100
Wire Wire Line
	1950 2200 1750 2200
Wire Wire Line
	1750 2300 1950 2300
Wire Wire Line
	1950 2750 1750 2750
Wire Wire Line
	1750 2850 1950 2850
Wire Wire Line
	1950 2950 1750 2950
Wire Wire Line
	1750 3100 1950 3100
Wire Wire Line
	1950 3200 1750 3200
Wire Wire Line
	1750 3300 1950 3300
Wire Wire Line
	1750 3450 1950 3450
Wire Wire Line
	1950 3550 1750 3550
Wire Wire Line
	1750 3650 1950 3650
Wire Wire Line
	1950 4100 1750 4100
Wire Wire Line
	1750 4200 1950 4200
Wire Wire Line
	1950 4300 1750 4300
Wire Wire Line
	1750 4500 1950 4500
Wire Wire Line
	1750 4600 1950 4600
NoConn ~ 3500 3650
$EndSCHEMATC