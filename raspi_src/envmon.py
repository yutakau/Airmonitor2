#!/usr/bin/python
# -*- coding: utf-8 -*-

import serial
import time, datetime
from tinydb import TinyDB, Query

db = TinyDB("/media/usb0/db.json")


ser = serial.Serial('/dev/ttyUSB0',115200)
line = ser.readline()
data = line.strip()

temp,humid,pressure,co2,tvoc = data.split(',')
timestr = time.strftime("%Y/%m/%d %H:%M")

print(timestr)
print(temp,"C")
print(humid,"%")
print(pressure,"mbh")
print(co2,"ppm")
print(tvoc,"ppb")

sensordata = {"temp":temp, "humid":humid, "pressure":pressure, "CO2":co2, "TVOC":tvoc }

db.insert({"time": timestr, "data":sensordata })

