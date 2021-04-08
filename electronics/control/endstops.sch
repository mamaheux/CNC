EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 5
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector:Conn_01x03_Female J?
U 1 1 606CC137
P 2150 1900
F 0 "J?" H 2042 1575 50  0000 C CNN
F 1 "JST_X_MIN" H 2042 1666 50  0000 C CNN
F 2 "" H 2150 1900 50  0001 C CNN
F 3 "~" H 2150 1900 50  0001 C CNN
	1    2150 1900
	-1   0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 606CFBCE
P 3200 1800
F 0 "R?" V 3407 1800 50  0000 C CNN
F 1 "5.62k" V 3316 1800 50  0000 C CNN
F 2 "" V 3130 1800 50  0001 C CNN
F 3 "~" H 3200 1800 50  0001 C CNN
	1    3200 1800
	0    -1   -1   0   
$EndComp
$Comp
L power:GNDD #PWR0151
U 1 1 606D1153
P 2400 2050
F 0 "#PWR0151" H 2400 1800 50  0001 C CNN
F 1 "GNDD" H 2404 1895 50  0000 C CNN
F 2 "" H 2400 2050 50  0001 C CNN
F 3 "" H 2400 2050 50  0001 C CNN
	1    2400 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	2400 2050 2400 2000
Wire Wire Line
	2400 2000 2350 2000
Wire Wire Line
	2400 1900 2350 1900
$Comp
L Device:R R?
U 1 1 606D3772
P 3200 1500
F 0 "R?" V 3407 1500 50  0000 C CNN
F 1 "1k" V 3316 1500 50  0000 C CNN
F 2 "" V 3130 1500 50  0001 C CNN
F 3 "~" H 3200 1500 50  0001 C CNN
	1    3200 1500
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3450 1500 3350 1500
Wire Wire Line
	2350 1800 2900 1800
Wire Wire Line
	3050 1500 2900 1500
Wire Wire Line
	2900 1500 2900 1800
Connection ~ 2900 1800
Wire Wire Line
	2900 1800 3050 1800
$Comp
L Device:C C?
U 1 1 606D427E
P 3400 2000
F 0 "C?" H 3515 2046 50  0000 L CNN
F 1 "0.1µF/10V" H 3515 1955 50  0000 L CNN
F 2 "" H 3438 1850 50  0001 C CNN
F 3 "~" H 3400 2000 50  0001 C CNN
	1    3400 2000
	1    0    0    -1  
$EndComp
$Comp
L power:GNDD #PWR0152
U 1 1 606D48C0
P 3400 2200
F 0 "#PWR0152" H 3400 1950 50  0001 C CNN
F 1 "GNDD" H 3404 2045 50  0000 C CNN
F 2 "" H 3400 2200 50  0001 C CNN
F 3 "" H 3400 2200 50  0001 C CNN
	1    3400 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 2200 3400 2150
Wire Wire Line
	3400 1850 3400 1800
Wire Wire Line
	3400 1800 3350 1800
Wire Wire Line
	3400 1800 3700 1800
Connection ~ 3400 1800
$Comp
L Device:Varistor RV?
U 1 1 606D59A1
P 3200 1150
F 0 "RV?" V 2942 1150 50  0000 C CNN
F 1 "VC080503A100DP" V 3033 1150 50  0000 C CNN
F 2 "" V 3130 1150 50  0001 C CNN
F 3 "~" H 3200 1150 50  0001 C CNN
	1    3200 1150
	0    1    1    0   
$EndComp
Wire Wire Line
	3050 1150 2900 1150
Wire Wire Line
	2900 1150 2900 1500
Connection ~ 2900 1500
Wire Wire Line
	3350 1150 3500 1150
$Comp
L power:GNDD #PWR0153
U 1 1 606DC0C5
P 3500 1200
F 0 "#PWR0153" H 3500 950 50  0001 C CNN
F 1 "GNDD" H 3504 1045 50  0000 C CNN
F 2 "" H 3500 1200 50  0001 C CNN
F 3 "" H 3500 1200 50  0001 C CNN
	1    3500 1200
	1    0    0    -1  
$EndComp
Wire Wire Line
	3500 1200 3500 1150
Text HLabel 3700 1800 2    50   Output ~ 0
X_MIN
$Comp
L Connector:Conn_01x03_Female J?
U 1 1 606E6743
P 2150 3650
F 0 "J?" H 2042 3325 50  0000 C CNN
F 1 "JST_X_MAX" H 2042 3416 50  0000 C CNN
F 2 "" H 2150 3650 50  0001 C CNN
F 3 "~" H 2150 3650 50  0001 C CNN
	1    2150 3650
	-1   0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 606E6749
P 3200 3550
F 0 "R?" V 3407 3550 50  0000 C CNN
F 1 "5.62k" V 3316 3550 50  0000 C CNN
F 2 "" V 3130 3550 50  0001 C CNN
F 3 "~" H 3200 3550 50  0001 C CNN
	1    3200 3550
	0    -1   -1   0   
$EndComp
$Comp
L power:GNDD #PWR0154
U 1 1 606E674F
P 2400 3800
F 0 "#PWR0154" H 2400 3550 50  0001 C CNN
F 1 "GNDD" H 2404 3645 50  0000 C CNN
F 2 "" H 2400 3800 50  0001 C CNN
F 3 "" H 2400 3800 50  0001 C CNN
	1    2400 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2400 3800 2400 3750
Wire Wire Line
	2400 3750 2350 3750
Wire Wire Line
	2400 3650 2350 3650
$Comp
L Device:R R?
U 1 1 606E6759
P 3200 3250
F 0 "R?" V 3407 3250 50  0000 C CNN
F 1 "1k" V 3316 3250 50  0000 C CNN
F 2 "" V 3130 3250 50  0001 C CNN
F 3 "~" H 3200 3250 50  0001 C CNN
	1    3200 3250
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3450 3250 3350 3250
Wire Wire Line
	2350 3550 2900 3550
Wire Wire Line
	3050 3250 2900 3250
Wire Wire Line
	2900 3250 2900 3550
Connection ~ 2900 3550
Wire Wire Line
	2900 3550 3050 3550
$Comp
L Device:C C?
U 1 1 606E6766
P 3400 3750
F 0 "C?" H 3515 3796 50  0000 L CNN
F 1 "0.1µF/10V" H 3515 3705 50  0000 L CNN
F 2 "" H 3438 3600 50  0001 C CNN
F 3 "~" H 3400 3750 50  0001 C CNN
	1    3400 3750
	1    0    0    -1  
$EndComp
$Comp
L power:GNDD #PWR0155
U 1 1 606E676C
P 3400 3950
F 0 "#PWR0155" H 3400 3700 50  0001 C CNN
F 1 "GNDD" H 3404 3795 50  0000 C CNN
F 2 "" H 3400 3950 50  0001 C CNN
F 3 "" H 3400 3950 50  0001 C CNN
	1    3400 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 3950 3400 3900
Wire Wire Line
	3400 3600 3400 3550
Wire Wire Line
	3400 3550 3350 3550
Wire Wire Line
	3400 3550 3700 3550
Connection ~ 3400 3550
$Comp
L Device:Varistor RV?
U 1 1 606E6777
P 3200 2900
F 0 "RV?" V 2942 2900 50  0000 C CNN
F 1 "VC080503A100DP" V 3033 2900 50  0000 C CNN
F 2 "" V 3130 2900 50  0001 C CNN
F 3 "~" H 3200 2900 50  0001 C CNN
	1    3200 2900
	0    1    1    0   
$EndComp
Wire Wire Line
	3050 2900 2900 2900
Wire Wire Line
	2900 2900 2900 3250
Connection ~ 2900 3250
Wire Wire Line
	3350 2900 3500 2900
$Comp
L power:GNDD #PWR0156
U 1 1 606E6781
P 3500 2950
F 0 "#PWR0156" H 3500 2700 50  0001 C CNN
F 1 "GNDD" H 3504 2795 50  0000 C CNN
F 2 "" H 3500 2950 50  0001 C CNN
F 3 "" H 3500 2950 50  0001 C CNN
	1    3500 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	3500 2950 3500 2900
Text HLabel 3700 3550 2    50   Output ~ 0
X_MAX
$Comp
L Connector:Conn_01x03_Female J?
U 1 1 606E9E17
P 2150 5300
F 0 "J?" H 2042 4975 50  0000 C CNN
F 1 "JST_Y_MIN" H 2042 5066 50  0000 C CNN
F 2 "" H 2150 5300 50  0001 C CNN
F 3 "~" H 2150 5300 50  0001 C CNN
	1    2150 5300
	-1   0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 606E9E1D
P 3200 5200
F 0 "R?" V 3407 5200 50  0000 C CNN
F 1 "5.62k" V 3316 5200 50  0000 C CNN
F 2 "" V 3130 5200 50  0001 C CNN
F 3 "~" H 3200 5200 50  0001 C CNN
	1    3200 5200
	0    -1   -1   0   
$EndComp
$Comp
L power:GNDD #PWR0157
U 1 1 606E9E23
P 2400 5450
F 0 "#PWR0157" H 2400 5200 50  0001 C CNN
F 1 "GNDD" H 2404 5295 50  0000 C CNN
F 2 "" H 2400 5450 50  0001 C CNN
F 3 "" H 2400 5450 50  0001 C CNN
	1    2400 5450
	1    0    0    -1  
$EndComp
Wire Wire Line
	2400 5450 2400 5400
Wire Wire Line
	2400 5400 2350 5400
Wire Wire Line
	2400 5300 2350 5300
$Comp
L Device:R R?
U 1 1 606E9E2D
P 3200 4900
F 0 "R?" V 3407 4900 50  0000 C CNN
F 1 "1k" V 3316 4900 50  0000 C CNN
F 2 "" V 3130 4900 50  0001 C CNN
F 3 "~" H 3200 4900 50  0001 C CNN
	1    3200 4900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3450 4900 3350 4900
Wire Wire Line
	2350 5200 2900 5200
Wire Wire Line
	3050 4900 2900 4900
Wire Wire Line
	2900 4900 2900 5200
Connection ~ 2900 5200
Wire Wire Line
	2900 5200 3050 5200
$Comp
L Device:C C?
U 1 1 606E9E3A
P 3400 5400
F 0 "C?" H 3515 5446 50  0000 L CNN
F 1 "0.1µF/10V" H 3515 5355 50  0000 L CNN
F 2 "" H 3438 5250 50  0001 C CNN
F 3 "~" H 3400 5400 50  0001 C CNN
	1    3400 5400
	1    0    0    -1  
$EndComp
$Comp
L power:GNDD #PWR0158
U 1 1 606E9E40
P 3400 5600
F 0 "#PWR0158" H 3400 5350 50  0001 C CNN
F 1 "GNDD" H 3404 5445 50  0000 C CNN
F 2 "" H 3400 5600 50  0001 C CNN
F 3 "" H 3400 5600 50  0001 C CNN
	1    3400 5600
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 5600 3400 5550
Wire Wire Line
	3400 5250 3400 5200
Wire Wire Line
	3400 5200 3350 5200
Wire Wire Line
	3400 5200 3700 5200
Connection ~ 3400 5200
$Comp
L Device:Varistor RV?
U 1 1 606E9E4B
P 3200 4550
F 0 "RV?" V 2942 4550 50  0000 C CNN
F 1 "VC080503A100DP" V 3033 4550 50  0000 C CNN
F 2 "" V 3130 4550 50  0001 C CNN
F 3 "~" H 3200 4550 50  0001 C CNN
	1    3200 4550
	0    1    1    0   
$EndComp
Wire Wire Line
	3050 4550 2900 4550
Wire Wire Line
	2900 4550 2900 4900
Connection ~ 2900 4900
Wire Wire Line
	3350 4550 3500 4550
$Comp
L power:GNDD #PWR0159
U 1 1 606E9E55
P 3500 4600
F 0 "#PWR0159" H 3500 4350 50  0001 C CNN
F 1 "GNDD" H 3504 4445 50  0000 C CNN
F 2 "" H 3500 4600 50  0001 C CNN
F 3 "" H 3500 4600 50  0001 C CNN
	1    3500 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	3500 4600 3500 4550
Text HLabel 3700 5200 2    50   Output ~ 0
Y_MIN
$Comp
L Connector:Conn_01x03_Female J?
U 1 1 606F960E
P 4350 1900
F 0 "J?" H 4242 1575 50  0000 C CNN
F 1 "JST_Y_MAX" H 4242 1666 50  0000 C CNN
F 2 "" H 4350 1900 50  0001 C CNN
F 3 "~" H 4350 1900 50  0001 C CNN
	1    4350 1900
	-1   0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 606F9614
P 5400 1800
F 0 "R?" V 5607 1800 50  0000 C CNN
F 1 "5.62k" V 5516 1800 50  0000 C CNN
F 2 "" V 5330 1800 50  0001 C CNN
F 3 "~" H 5400 1800 50  0001 C CNN
	1    5400 1800
	0    -1   -1   0   
$EndComp
$Comp
L power:GNDD #PWR0160
U 1 1 606F961A
P 4600 2050
F 0 "#PWR0160" H 4600 1800 50  0001 C CNN
F 1 "GNDD" H 4604 1895 50  0000 C CNN
F 2 "" H 4600 2050 50  0001 C CNN
F 3 "" H 4600 2050 50  0001 C CNN
	1    4600 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 2050 4600 2000
Wire Wire Line
	4600 2000 4550 2000
Wire Wire Line
	4600 1900 4550 1900
$Comp
L Device:R R?
U 1 1 606F9624
P 5400 1500
F 0 "R?" V 5607 1500 50  0000 C CNN
F 1 "1k" V 5516 1500 50  0000 C CNN
F 2 "" V 5330 1500 50  0001 C CNN
F 3 "~" H 5400 1500 50  0001 C CNN
	1    5400 1500
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5650 1500 5550 1500
Wire Wire Line
	4550 1800 5100 1800
Wire Wire Line
	5250 1500 5100 1500
Wire Wire Line
	5100 1500 5100 1800
Connection ~ 5100 1800
Wire Wire Line
	5100 1800 5250 1800
$Comp
L Device:C C?
U 1 1 606F9631
P 5600 2000
F 0 "C?" H 5715 2046 50  0000 L CNN
F 1 "0.1µF/10V" H 5715 1955 50  0000 L CNN
F 2 "" H 5638 1850 50  0001 C CNN
F 3 "~" H 5600 2000 50  0001 C CNN
	1    5600 2000
	1    0    0    -1  
$EndComp
$Comp
L power:GNDD #PWR0161
U 1 1 606F9637
P 5600 2200
F 0 "#PWR0161" H 5600 1950 50  0001 C CNN
F 1 "GNDD" H 5604 2045 50  0000 C CNN
F 2 "" H 5600 2200 50  0001 C CNN
F 3 "" H 5600 2200 50  0001 C CNN
	1    5600 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 2200 5600 2150
Wire Wire Line
	5600 1850 5600 1800
Wire Wire Line
	5600 1800 5550 1800
Wire Wire Line
	5600 1800 5900 1800
Connection ~ 5600 1800
$Comp
L Device:Varistor RV?
U 1 1 606F9642
P 5400 1150
F 0 "RV?" V 5142 1150 50  0000 C CNN
F 1 "VC080503A100DP" V 5233 1150 50  0000 C CNN
F 2 "" V 5330 1150 50  0001 C CNN
F 3 "~" H 5400 1150 50  0001 C CNN
	1    5400 1150
	0    1    1    0   
$EndComp
Wire Wire Line
	5250 1150 5100 1150
Wire Wire Line
	5100 1150 5100 1500
Connection ~ 5100 1500
Wire Wire Line
	5550 1150 5700 1150
$Comp
L power:GNDD #PWR0162
U 1 1 606F964C
P 5700 1200
F 0 "#PWR0162" H 5700 950 50  0001 C CNN
F 1 "GNDD" H 5704 1045 50  0000 C CNN
F 2 "" H 5700 1200 50  0001 C CNN
F 3 "" H 5700 1200 50  0001 C CNN
	1    5700 1200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5700 1200 5700 1150
Text HLabel 5900 1800 2    50   Output ~ 0
Y_MAX
$Comp
L Connector:Conn_01x03_Female J?
U 1 1 6070020E
P 4350 3650
F 0 "J?" H 4242 3325 50  0000 C CNN
F 1 "JST_Z_MIN" H 4242 3416 50  0000 C CNN
F 2 "" H 4350 3650 50  0001 C CNN
F 3 "~" H 4350 3650 50  0001 C CNN
	1    4350 3650
	-1   0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 60700214
P 5400 3550
F 0 "R?" V 5607 3550 50  0000 C CNN
F 1 "5.62k" V 5516 3550 50  0000 C CNN
F 2 "" V 5330 3550 50  0001 C CNN
F 3 "~" H 5400 3550 50  0001 C CNN
	1    5400 3550
	0    -1   -1   0   
$EndComp
$Comp
L power:GNDD #PWR0163
U 1 1 6070021A
P 4600 3800
F 0 "#PWR0163" H 4600 3550 50  0001 C CNN
F 1 "GNDD" H 4604 3645 50  0000 C CNN
F 2 "" H 4600 3800 50  0001 C CNN
F 3 "" H 4600 3800 50  0001 C CNN
	1    4600 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 3800 4600 3750
Wire Wire Line
	4600 3750 4550 3750
Wire Wire Line
	4600 3650 4550 3650
$Comp
L Device:R R?
U 1 1 60700224
P 5400 3250
F 0 "R?" V 5607 3250 50  0000 C CNN
F 1 "1k" V 5516 3250 50  0000 C CNN
F 2 "" V 5330 3250 50  0001 C CNN
F 3 "~" H 5400 3250 50  0001 C CNN
	1    5400 3250
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5650 3250 5550 3250
Wire Wire Line
	4550 3550 5100 3550
Wire Wire Line
	5250 3250 5100 3250
Wire Wire Line
	5100 3250 5100 3550
Connection ~ 5100 3550
Wire Wire Line
	5100 3550 5250 3550
$Comp
L Device:C C?
U 1 1 60700231
P 5600 3750
F 0 "C?" H 5715 3796 50  0000 L CNN
F 1 "0.1µF/10V" H 5715 3705 50  0000 L CNN
F 2 "" H 5638 3600 50  0001 C CNN
F 3 "~" H 5600 3750 50  0001 C CNN
	1    5600 3750
	1    0    0    -1  
$EndComp
$Comp
L power:GNDD #PWR0164
U 1 1 60700237
P 5600 3950
F 0 "#PWR0164" H 5600 3700 50  0001 C CNN
F 1 "GNDD" H 5604 3795 50  0000 C CNN
F 2 "" H 5600 3950 50  0001 C CNN
F 3 "" H 5600 3950 50  0001 C CNN
	1    5600 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 3950 5600 3900
Wire Wire Line
	5600 3600 5600 3550
Wire Wire Line
	5600 3550 5550 3550
Wire Wire Line
	5600 3550 5900 3550
Connection ~ 5600 3550
$Comp
L Device:Varistor RV?
U 1 1 60700242
P 5400 2900
F 0 "RV?" V 5142 2900 50  0000 C CNN
F 1 "VC080503A100DP" V 5233 2900 50  0000 C CNN
F 2 "" V 5330 2900 50  0001 C CNN
F 3 "~" H 5400 2900 50  0001 C CNN
	1    5400 2900
	0    1    1    0   
$EndComp
Wire Wire Line
	5250 2900 5100 2900
Wire Wire Line
	5100 2900 5100 3250
Connection ~ 5100 3250
Wire Wire Line
	5550 2900 5700 2900
$Comp
L power:GNDD #PWR0165
U 1 1 6070024C
P 5700 2950
F 0 "#PWR0165" H 5700 2700 50  0001 C CNN
F 1 "GNDD" H 5704 2795 50  0000 C CNN
F 2 "" H 5700 2950 50  0001 C CNN
F 3 "" H 5700 2950 50  0001 C CNN
	1    5700 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	5700 2950 5700 2900
Text HLabel 5900 3550 2    50   Output ~ 0
Z_MIN
$Comp
L Connector:Conn_01x03_Female J?
U 1 1 6070468F
P 4350 5300
F 0 "J?" H 4242 4975 50  0000 C CNN
F 1 "JST_Z_MAX" H 4242 5066 50  0000 C CNN
F 2 "" H 4350 5300 50  0001 C CNN
F 3 "~" H 4350 5300 50  0001 C CNN
	1    4350 5300
	-1   0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 60704695
P 5400 5200
F 0 "R?" V 5607 5200 50  0000 C CNN
F 1 "5.62k" V 5516 5200 50  0000 C CNN
F 2 "" V 5330 5200 50  0001 C CNN
F 3 "~" H 5400 5200 50  0001 C CNN
	1    5400 5200
	0    -1   -1   0   
$EndComp
$Comp
L power:GNDD #PWR0166
U 1 1 6070469B
P 4600 5450
F 0 "#PWR0166" H 4600 5200 50  0001 C CNN
F 1 "GNDD" H 4604 5295 50  0000 C CNN
F 2 "" H 4600 5450 50  0001 C CNN
F 3 "" H 4600 5450 50  0001 C CNN
	1    4600 5450
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 5450 4600 5400
Wire Wire Line
	4600 5400 4550 5400
Wire Wire Line
	4600 5300 4550 5300
$Comp
L Device:R R?
U 1 1 607046A5
P 5400 4900
F 0 "R?" V 5607 4900 50  0000 C CNN
F 1 "1k" V 5516 4900 50  0000 C CNN
F 2 "" V 5330 4900 50  0001 C CNN
F 3 "~" H 5400 4900 50  0001 C CNN
	1    5400 4900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5650 4900 5550 4900
Wire Wire Line
	4550 5200 5100 5200
Wire Wire Line
	5250 4900 5100 4900
Wire Wire Line
	5100 4900 5100 5200
Connection ~ 5100 5200
Wire Wire Line
	5100 5200 5250 5200
$Comp
L Device:C C?
U 1 1 607046B2
P 5600 5400
F 0 "C?" H 5715 5446 50  0000 L CNN
F 1 "0.1µF/10V" H 5715 5355 50  0000 L CNN
F 2 "" H 5638 5250 50  0001 C CNN
F 3 "~" H 5600 5400 50  0001 C CNN
	1    5600 5400
	1    0    0    -1  
$EndComp
$Comp
L power:GNDD #PWR0167
U 1 1 607046B8
P 5600 5600
F 0 "#PWR0167" H 5600 5350 50  0001 C CNN
F 1 "GNDD" H 5604 5445 50  0000 C CNN
F 2 "" H 5600 5600 50  0001 C CNN
F 3 "" H 5600 5600 50  0001 C CNN
	1    5600 5600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 5600 5600 5550
Wire Wire Line
	5600 5250 5600 5200
Wire Wire Line
	5600 5200 5550 5200
Wire Wire Line
	5600 5200 5900 5200
Connection ~ 5600 5200
$Comp
L Device:Varistor RV?
U 1 1 607046C3
P 5400 4550
F 0 "RV?" V 5142 4550 50  0000 C CNN
F 1 "VC080503A100DP" V 5233 4550 50  0000 C CNN
F 2 "" V 5330 4550 50  0001 C CNN
F 3 "~" H 5400 4550 50  0001 C CNN
	1    5400 4550
	0    1    1    0   
$EndComp
Wire Wire Line
	5250 4550 5100 4550
Wire Wire Line
	5100 4550 5100 4900
Connection ~ 5100 4900
Wire Wire Line
	5550 4550 5700 4550
$Comp
L power:GNDD #PWR0168
U 1 1 607046CD
P 5700 4600
F 0 "#PWR0168" H 5700 4350 50  0001 C CNN
F 1 "GNDD" H 5704 4445 50  0000 C CNN
F 2 "" H 5700 4600 50  0001 C CNN
F 3 "" H 5700 4600 50  0001 C CNN
	1    5700 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5700 4600 5700 4550
Text HLabel 5900 5200 2    50   Output ~ 0
Z_MAX
Text GLabel 3450 3250 2    50   UnSpc ~ 0
ENDSTOPS_3.3V
Text GLabel 3450 1500 2    50   UnSpc ~ 0
ENDSTOPS_3.3V
Text GLabel 3450 4900 2    50   UnSpc ~ 0
ENDSTOPS_3.3V
Text GLabel 5650 4900 2    50   UnSpc ~ 0
ENDSTOPS_3.3V
Text GLabel 5650 3250 2    50   UnSpc ~ 0
ENDSTOPS_3.3V
Text GLabel 5650 1500 2    50   UnSpc ~ 0
ENDSTOPS_3.3V
Text GLabel 2400 3650 2    50   UnSpc ~ 0
ENDSTOPS_3.3V
Text GLabel 4600 3650 2    50   UnSpc ~ 0
ENDSTOPS_3.3V
Text GLabel 4600 5300 2    50   UnSpc ~ 0
ENDSTOPS_3.3V
Text GLabel 2400 5300 2    50   UnSpc ~ 0
ENDSTOPS_3.3V
Text GLabel 2400 1900 2    50   UnSpc ~ 0
ENDSTOPS_3.3V
Text GLabel 4600 1900 2    50   UnSpc ~ 0
ENDSTOPS_3.3V
$EndSCHEMATC
