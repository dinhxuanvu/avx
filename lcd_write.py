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

# Initialize the LCD using the pins above.
lcd = LCD.Adafruit_CharLCD(lcd_rs, lcd_en, lcd_d4, lcd_d5, lcd_d6, lcd_d7, 
							lcd_columns, lcd_rows)

# Print a two line message
lcd.message("Write\n here")
while True:
	userinput = stdin.readline()
	lcd.clear()
	lcd.message(userinput)
	time.sleep(1)
