EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "muRLi"
Date "2020-06-06"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_Module:WeMos_D1_mini U1
U 1 1 5EDB5792
P 1600 2100
F 0 "U1" H 1200 1550 50  0000 C CNN
F 1 "WeMos_D1_mini" H 1000 1450 50  0000 C CNN
F 2 "Module:WEMOS_D1_mini_light" H 1600 950 50  0001 C CNN
F 3 "https://wiki.wemos.cc/products:d1:d1_mini#documentation" H -250 950 50  0001 C CNN
	1    1600 2100
	1    0    0    -1  
$EndComp
$Comp
L muRLi:OLED_SH1106 J2
U 1 1 5EDD195F
P 3050 1950
F 0 "J2" H 3130 1992 50  0000 L CNN
F 1 "OLED_SH1106" H 3130 1901 50  0000 L CNN
F 2 "" H 3050 1950 50  0001 C CNN
F 3 "~" H 3050 1950 50  0001 C CNN
	1    3050 1950
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J1
U 1 1 5EDDB27B
P 2700 1150
F 0 "J1" H 2780 1192 50  0000 L CNN
F 1 "MAX4466" H 2780 1101 50  0000 L CNN
F 2 "" H 2700 1150 50  0001 C CNN
F 3 "~" H 2700 1150 50  0001 C CNN
	1    2700 1150
	1    0    0    -1  
$EndComp
$Comp
L Device:CP_Small C1
U 1 1 5EDDC226
P 3350 1150
F 0 "C1" H 3438 1196 50  0000 L CNN
F 1 "CP_Small" H 3438 1105 50  0000 L CNN
F 2 "" H 3350 1150 50  0001 C CNN
F 3 "~" H 3350 1150 50  0001 C CNN
	1    3350 1150
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x05 J4
U 1 1 5EDDD50B
P 3500 2750
F 0 "J4" H 3580 2792 50  0000 L CNN
F 1 "MOD Connector" H 3580 2701 50  0000 L CNN
F 2 "" H 3500 2750 50  0001 C CNN
F 3 "~" H 3500 2750 50  0001 C CNN
	1    3500 2750
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J3
U 1 1 5EDDE724
P 3050 3500
F 0 "J3" H 3130 3542 50  0000 L CNN
F 1 "LED" H 3130 3451 50  0000 L CNN
F 2 "" H 3050 3500 50  0001 C CNN
F 3 "~" H 3050 3500 50  0001 C CNN
	1    3050 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	1500 1300 1500 1100
Text Label 1500 1100 0    50   ~ 0
VERO1
Wire Wire Line
	2000 1600 2250 1600
Wire Wire Line
	2250 1600 2250 1150
Wire Wire Line
	2250 1150 2500 1150
Wire Wire Line
	2500 1050 2500 900 
Wire Wire Line
	2500 900  3350 900 
Wire Wire Line
	3350 900  3350 1050
Wire Wire Line
	2500 1250 2500 1400
Wire Wire Line
	2500 1400 3350 1400
Wire Wire Line
	3350 1400 3350 1250
Wire Wire Line
	3350 1400 3350 1500
Connection ~ 3350 1400
Wire Wire Line
	3350 900  3350 800 
Connection ~ 3350 900 
Text Label 3350 800  0    50   ~ 0
VERO1
Text Label 3350 1500 0    50   ~ 0
VERO2
Wire Wire Line
	1600 2900 1600 3100
Text Label 1600 3100 0    50   ~ 0
VERO2
Text Label 2500 1650 0    50   ~ 0
VERO2
Text Label 2500 1750 0    50   ~ 0
VERO1
Wire Wire Line
	2500 1750 2800 1750
Wire Wire Line
	2500 1650 2800 1650
Wire Wire Line
	3300 2550 2950 2550
Wire Wire Line
	3300 2650 2950 2650
Text Label 2950 2550 0    50   ~ 0
VERO2
Text Label 2950 2650 0    50   ~ 0
VERO1
Wire Wire Line
	3300 2850 2950 2850
Wire Wire Line
	3300 2950 2950 2950
Text Label 2950 2850 0    50   ~ 0
D2
Text Label 2950 2950 0    50   ~ 0
D1
Text Label 2000 1900 0    50   ~ 0
D2
Text Label 2000 1800 0    50   ~ 0
D1
Text Label 2000 2200 0    50   ~ 0
D5
Wire Wire Line
	2800 1850 2500 1850
Wire Wire Line
	2800 1950 2500 1950
Wire Wire Line
	2800 2050 2500 2050
Wire Wire Line
	2800 2150 2500 2150
Wire Wire Line
	2800 2250 2500 2250
Text Label 2500 1850 0    50   ~ 0
D5
Text Label 2500 1950 0    50   ~ 0
D7
Text Label 2000 2400 0    50   ~ 0
D7
Text Label 2500 2050 0    50   ~ 0
D3
Text Label 2000 2000 0    50   ~ 0
D3
Text Label 2500 2150 0    50   ~ 0
D0
Text Label 2000 1700 0    50   ~ 0
D0
Text Label 2500 2250 0    50   ~ 0
D8
Text Label 2000 2500 0    50   ~ 0
D8
Wire Wire Line
	2850 3500 2500 3500
Text Label 2500 3500 0    50   ~ 0
D4
Text Label 2000 2100 0    50   ~ 0
D4
Wire Wire Line
	2850 3400 2500 3400
Wire Wire Line
	2850 3600 2500 3600
Text Label 2500 3400 0    50   ~ 0
VERO2
Text Label 2500 3600 0    50   ~ 0
VERO1
$Comp
L Device:R R1
U 1 1 5EDF5086
P 2550 2900
F 0 "R1" V 2343 2900 50  0000 C CNN
F 1 "R1K" V 2434 2900 50  0000 C CNN
F 2 "" V 2480 2900 50  0001 C CNN
F 3 "~" H 2550 2900 50  0001 C CNN
	1    2550 2900
	1    0    0    -1  
$EndComp
Text Label 2550 3150 0    50   ~ 0
VERO2
Wire Wire Line
	2550 3050 2550 3150
Wire Wire Line
	2550 2300 2000 2300
Wire Wire Line
	2550 2300 2550 2750
Wire Wire Line
	3300 2750 2550 2750
Connection ~ 2550 2750
$EndSCHEMATC
