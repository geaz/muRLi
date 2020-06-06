EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
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
L Memory_EEPROM:24LC32 U1
U 1 1 5EDB8834
P 1900 1300
F 0 "U1" H 2150 1750 50  0000 C CNN
F 1 "24LC32" H 2250 1650 50  0000 C CNN
F 2 "" H 1900 1300 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/21072G.pdf" H 1900 1300 50  0001 C CNN
	1    1900 1300
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x05 J1
U 1 1 5EDBA866
P 1900 2650
F 0 "J1" V 1772 2930 50  0000 L CNN
F 1 "Conn_01x05" V 1863 2930 50  0000 L CNN
F 2 "" H 1900 2650 50  0001 C CNN
F 3 "~" H 1900 2650 50  0001 C CNN
	1    1900 2650
	0    1    1    0   
$EndComp
Wire Wire Line
	2300 1400 2450 1400
Wire Wire Line
	1800 2450 1800 2300
Wire Wire Line
	1800 2300 1850 2300
Wire Wire Line
	1900 2300 1900 2450
Wire Wire Line
	1850 2300 1850 2200
Connection ~ 1850 2300
Wire Wire Line
	1850 2300 1900 2300
Text Label 1850 2200 0    50   ~ 0
VCC
Text Label 1700 2050 0    50   ~ 0
GND
Wire Wire Line
	1700 2050 1700 2450
Wire Wire Line
	1900 1600 1900 1700
Wire Wire Line
	1900 1000 1900 850 
Wire Wire Line
	1300 1400 1500 1400
Wire Wire Line
	1300 1300 1500 1300
Wire Wire Line
	1300 1200 1500 1200
Text Label 1300 1200 0    50   ~ 0
GND
Text Label 1300 1300 0    50   ~ 0
GND
Text Label 1300 1400 0    50   ~ 0
GND
Text Label 1900 1700 0    50   ~ 0
GND
Text Label 1900 850  0    50   ~ 0
VCC
Text Label 2450 1400 0    50   ~ 0
GND
Wire Wire Line
	2300 1200 2800 1200
Wire Wire Line
	2800 1200 2800 2300
Wire Wire Line
	2800 2300 2000 2300
Wire Wire Line
	2000 2300 2000 2450
Wire Wire Line
	2300 1300 2700 1300
Wire Wire Line
	2700 1300 2700 2200
Wire Wire Line
	2700 2200 2100 2200
Wire Wire Line
	2100 2200 2100 2450
$EndSCHEMATC
