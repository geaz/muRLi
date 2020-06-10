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
Connection ~ 2450 2750
Wire Wire Line
	3200 2750 2450 2750
Wire Wire Line
	2450 2300 2450 2750
Wire Wire Line
	2450 2300 1900 2300
Wire Wire Line
	2450 3050 2450 3150
Text Label 2450 3150 0    50   ~ 0
VERO2
$Comp
L Device:R R?
U 1 1 5EE032EB
P 2450 2900
F 0 "R?" V 2243 2900 50  0000 C CNN
F 1 "R1K" V 2334 2900 50  0000 C CNN
F 2 "" V 2380 2900 50  0001 C CNN
F 3 "~" H 2450 2900 50  0001 C CNN
	1    2450 2900
	1    0    0    -1  
$EndComp
Text Label 2400 3400 0    50   ~ 0
VERO2
Wire Wire Line
	2750 3600 2400 3600
Wire Wire Line
	2750 3400 2400 3400
Text Label 1900 2100 0    50   ~ 0
D4
Text Label 2400 3500 0    50   ~ 0
D4
Wire Wire Line
	2750 3500 2400 3500
Text Label 1900 2500 0    50   ~ 0
D8
Text Label 2400 2250 0    50   ~ 0
D8
Text Label 1900 1700 0    50   ~ 0
D0
Text Label 2400 2150 0    50   ~ 0
D0
Text Label 1900 2000 0    50   ~ 0
D3
Text Label 2400 2050 0    50   ~ 0
D3
Text Label 1900 2400 0    50   ~ 0
D7
Text Label 2400 1950 0    50   ~ 0
D7
Text Label 2400 1850 0    50   ~ 0
D5
Wire Wire Line
	2700 2250 2400 2250
Wire Wire Line
	2700 2150 2400 2150
Wire Wire Line
	2700 2050 2400 2050
Wire Wire Line
	2700 1950 2400 1950
Wire Wire Line
	2700 1850 2400 1850
Text Label 1900 2200 0    50   ~ 0
D5
Text Label 1900 1800 0    50   ~ 0
D1
Text Label 1900 1900 0    50   ~ 0
D2
Text Label 2850 2950 0    50   ~ 0
D1
Text Label 2850 2850 0    50   ~ 0
D2
Wire Wire Line
	3200 2950 2850 2950
Wire Wire Line
	3200 2850 2850 2850
Text Label 2850 2650 0    50   ~ 0
VERO1
Text Label 2850 2550 0    50   ~ 0
VERO2
Wire Wire Line
	3200 2650 2850 2650
Wire Wire Line
	3200 2550 2850 2550
Wire Wire Line
	2400 1650 2700 1650
Wire Wire Line
	2400 1750 2700 1750
Text Label 2400 1750 0    50   ~ 0
VERO1
Text Label 2400 1650 0    50   ~ 0
VERO2
Text Label 1500 3100 0    50   ~ 0
VERO2
Wire Wire Line
	1500 2900 1500 3100
Text Label 3250 1500 0    50   ~ 0
VERO2
Text Label 3250 800  0    50   ~ 0
VERO1
Connection ~ 3250 900 
Wire Wire Line
	3250 900  3250 800 
Connection ~ 3250 1400
Wire Wire Line
	3250 1400 3250 1500
Wire Wire Line
	3250 1400 3250 1250
Wire Wire Line
	2400 1400 3250 1400
Wire Wire Line
	2400 1250 2400 1400
Wire Wire Line
	3250 900  3250 1050
Wire Wire Line
	2400 900  3250 900 
Wire Wire Line
	2400 1050 2400 900 
Wire Wire Line
	2150 1150 2400 1150
Wire Wire Line
	2150 1600 2150 1150
Wire Wire Line
	1900 1600 2150 1600
Text Label 1600 1100 0    50   ~ 0
VERO1
Wire Wire Line
	1400 1300 1400 1100
$Comp
L Connector_Generic:Conn_01x03 J?
U 1 1 5EE03328
P 2950 3500
F 0 "J?" H 3030 3542 50  0000 L CNN
F 1 "LED" H 3030 3451 50  0000 L CNN
F 2 "" H 2950 3500 50  0001 C CNN
F 3 "~" H 2950 3500 50  0001 C CNN
	1    2950 3500
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x05 J?
U 1 1 5EE0332E
P 3400 2750
F 0 "J?" H 3480 2792 50  0000 L CNN
F 1 "MOD Connector" H 3480 2701 50  0000 L CNN
F 2 "" H 3400 2750 50  0001 C CNN
F 3 "~" H 3400 2750 50  0001 C CNN
	1    3400 2750
	1    0    0    -1  
$EndComp
$Comp
L Device:CP_Small C?
U 1 1 5EE03334
P 3250 1150
F 0 "C?" H 3338 1196 50  0000 L CNN
F 1 "CP_Small" H 3338 1105 50  0000 L CNN
F 2 "" H 3250 1150 50  0001 C CNN
F 3 "~" H 3250 1150 50  0001 C CNN
	1    3250 1150
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J?
U 1 1 5EE0333A
P 2600 1150
F 0 "J?" H 2680 1192 50  0000 L CNN
F 1 "MAX4466" H 2680 1101 50  0000 L CNN
F 2 "" H 2600 1150 50  0001 C CNN
F 3 "~" H 2600 1150 50  0001 C CNN
	1    2600 1150
	1    0    0    -1  
$EndComp
$Comp
L muRLi:OLED_SH1106 J?
U 1 1 5EE03340
P 2950 1950
F 0 "J?" H 3030 1992 50  0000 L CNN
F 1 "OLED_SH1106" H 3030 1901 50  0000 L CNN
F 2 "" H 2950 1950 50  0001 C CNN
F 3 "~" H 2950 1950 50  0001 C CNN
	1    2950 1950
	1    0    0    -1  
$EndComp
$Comp
L MCU_Module:WeMos_D1_mini U?
U 1 1 5EE03346
P 1500 2100
F 0 "U?" H 1100 1550 50  0000 C CNN
F 1 "WeMos_D1_mini" H 900 1450 50  0000 C CNN
F 2 "Module:WEMOS_D1_mini_light" H 1500 950 50  0001 C CNN
F 3 "https://wiki.wemos.cc/products:d1:d1_mini#documentation" H -350 950 50  0001 C CNN
	1    1500 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	1600 1300 1600 1100
Wire Wire Line
	1400 1100 1200 1100
Text Label 1200 1100 0    50   ~ 0
LED_VCC
Text Label 2400 3600 0    50   ~ 0
LED_VCC
$EndSCHEMATC
