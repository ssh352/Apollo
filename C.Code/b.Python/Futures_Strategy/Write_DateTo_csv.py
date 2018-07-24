#!/usr/bin/env python  
# encoding: utf-8  

""" 
@version: v1.0 
@author: xiaxianba
@license: Apache Licence  
@contact: 778757080@qq.com 
@site: http://xianba.github.com/ 
@software: PyCharm 
@file: rollover.py.py 
@time: 2017/10/15 09:23 
"""

import csv
import datetime as pydt

from WindPy import *

BEGIN_DATE = "2017-01-01"
END_DATE = "2017-12-31"


def get_futures_data():

    w.start()

    data_file = "output/date2017.csv"

    trade_day_list = w.tdays(BEGIN_DATE, END_DATE, "")

    for day in trade_day_list.Data[0]:
        str_date = day.strftime('%Y-%m-%d')

        with open(data_file, "ab+") as csvfile:
            writer = csv.writer(csvfile)
            writer.writerow([str_date])

def get_day_offset():

    datenow = pydt.date.today().strftime('%Y-%m-%d')
    result = w.tdaysoffset(-1, datenow, "")
    result.Data[0][0].strftime('%Y-%m-%d')


if __name__ == "__main__":

    get_futures_data()

