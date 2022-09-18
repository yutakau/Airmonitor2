#!/usr/bin/python
# -*- coding: utf-8 -*-

import serial
import time, datetime
from tinydb import TinyDB, Query

db = TinyDB("/media/usb0/db.json")

ser = serial.Serial('/dev/ttyUSB0',115200)

while(1):
	if ser.in_waiting > 0:
		line = ser.readline()
		data = line.strip()

		temp,humid,pressure,co2,tvoc = data.split(',')
		timestr = time.strftime("%Y/%m/%d %H:%M")

		u = time.localtime()
		if u.tm_min == "00" or u.tm_min=="15" or u.tm_min=="30" or u.tm_min=="45":

			print(timestr)
			print(temp,"C")
			print(humid,"%")
			print(pressure,"mbh")
			print(co2,"ppm")
			print(tvoc,"ppb")

			sensordata = {"temp":temp, "humid":humid, "pressure":pressure, "CO2":co2, "TVOC":tvoc }

			db.insert({"year":u.tm_year, "mon":u.tm_mon, "day":u.tm_mday, "hour":u.tm_hour, "min":u.tm_min, "data":sensordata })
			time.sleep(100)  # 100 sec
