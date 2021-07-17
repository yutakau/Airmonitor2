#!/usr/bin/python
# -*- coding:utf-8 -*-

import pprint
#import numpy as np

from bokeh.io import output_file, show
from bokeh.models import DatetimeTickFormatter
from datetime import datetime as dt
from bokeh.plotting import figure


from tinydb import TinyDB,Query

db = TinyDB('db.json')
qwy = Query()


dates = []
data = []
#year 2021
#for item in db.search((qwy.year==2021) & (qwy.mon==6 ) & (qwy.hour==12) ):
for item in db.search((qwy.year==2021) & (qwy.mon==6 ) & (qwy.hour==9) ):
    print('***')
    pprint.pprint(item)

    dates.append( "{}-{}-{}".format(item['year'], item['mon'], item['day']) )
    #data.append( int(item['data']['CO2']) )
    data.append( float(item['data']['temp']) )
    
#dates = ['2021-1-1','2021-2-1','2021-3-1','2021-4-1','2021-5-1']
#counts = [5, 3, 4, 2, 4, 6]
print(dates)
print(data)

xlist = [dt.strptime(d, '%Y-%m-%d') for d in dates]

p = figure(plot_height=350, toolbar_location=None, title="temperature",x_axis_type="datetime")
p.xaxis.axis_label = "Date"
x_format = "%Y/%m/%d"
p.xaxis.formatter = DatetimeTickFormatter(days=[x_format],months=[x_format],years=[x_format])
p.line( xlist , data )

p.legend.orientation = "horizontal"
p.legend.location = "top_center"

show(p)