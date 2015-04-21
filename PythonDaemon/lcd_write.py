#!/usr/bin/python
# Example using a character LCD connected to a Raspberry Pi or BeagleBone Black.
import math
import time

import Adafruit_CharLCD as LCD
from sys import stdin

# BeagleBone Black configuration:
lcd_rs        = 'P8_27'
lcd_en        = 'P8_29'
lcd_d4        = 'P8_31'
lcd_d5        = 'P8_39'
lcd_d6        = 'P8_43'
lcd_d7        = 'P8_41'

# Define LCD column and row size for 16x2 LCD.
lcd_columns = 16
lcd_rows    = 2

# Define read last lines function
def follow(thefile):
	thefile.seek(0,2)
	while True:
		line = thefile.readline()
		if not line:
			time.sleep(0.1)
			continue
		yield line

# Initialize the LCD using the pins above.
lcd = LCD.Adafruit_CharLCD(lcd_rs, lcd_en, lcd_d4, lcd_d5, lcd_d6, lcd_d7, 
							lcd_columns, lcd_rows)

# Print a two line message
logfile = open("PythonDaemon/lcd-log","r")
loglines = follow(logfile)
lline1 = ""
lline2 = ""
for line in loglines:
        line1 = line.rstrip()[0:15]
        line2 = line.rstrip()[16:31]
        if(line1 != lline1):
		lcd.set_cursor(0,0)
		lcd.message(line1.ljust(16))
	if(line2 != lline2):
		lcd.set_cursor(0,1)
		lcd.message(line2.ljust(16))
