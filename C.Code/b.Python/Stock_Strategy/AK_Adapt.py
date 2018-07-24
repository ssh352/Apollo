#!/usr/bin/env python  
# encoding: utf-8  

""" 
@version: v1.0 
@author: xiaxianba 
@license: Apache Licence  
@contact: scuxia@gmail.com 
@site: http://weibo.com/xiaxianba 
@software: PyCharm 
@file: AK_Adapt.py 
@time: 2018/3/22 11:09 
@describe: 
"""

import csv
import datetime as pydt
import fractions
import numpy as np

from WindPy import *

# 中信行业指数
list_item = ['CI005001.WI', 'CI005002.WI', 'CI005003.WI', 'CI005004.WI', 'CI005005.WI', 'CI005006.WI',
             'CI005007.WI', 'CI005008.WI', 'CI005009.WI', 'CI005010.WI', 'CI005011.WI', 'CI005012.WI',
             'CI005013.WI', 'CI005014.WI', 'CI005015.WI', 'CI005016.WI', 'CI005017.WI', 'CI005018.WI',
             'CI005019.WI', 'CI005020.WI', 'CI005021.WI', 'CI005022.WI', 'CI005023.WI', 'CI005024.WI',
             'CI005025.WI', 'CI005026.WI', 'CI005027.WI', 'CI005028.WI', 'CI005029.WI']


# 计算轨迹效率系数ER,并返回平滑系数
def get_smooth(target, date_now, period=10):

    fast = fractions.Fraction(2, 3)
    slow = fractions.Fraction(2, 31)
    # date_now = pydt.date.today() - pydt.timedelta(days=1)
    str_period = "ED-" + str(period-1) + "TD"

    result = w.wsd(target, "close", str_period, date_now, "")

    if result.ErrorCode == 0:
        list_price = result.Data[0]

        ER = abs(list_price[period-1] - list_price[0]) / np.sum([abs(list_price[i + 1] - list_price[i]) for i in range(0, period-1)])
        smooth = ER*(fast - slow) + slow

        return smooth*smooth


# 获取自适应均线
def get_ama(target, date_now, cycle):

    if cycle == 1:
        result_1 = w.wsd(target, "close", "ED0TD", date_now, "")
        close_1 = result_1.Data[0][0]
        return close_1
    else:
        smooth = get_smooth(target, date_now, period=10)
        result = w.wsd(target, "close", "ED0TD", date_now, "")
        close_price = result.Data[0][0]

        return smooth*close_price + (1-smooth)*get_ama(target, date_now, cycle-1)


# 获取自适应均线做多品种
def get_items(date_now):

    dict_short = {}
    dict_long = {}
    list_index = []

    for item in list_item:
        dict_short[item] = get_ama(item, date_now, 5)
        dict_long[item] = get_ama(item, date_now, 10)

    for item in list_item:
        if dict_short[item] > dict_long[item]:
            list_index.append(item)

    return list_index


if __name__ == "__main__":

    BEGIN_DATE = "2017-12-01"
    END_DATE = "2017-12-15"

    list_record = []

    w.start()

    trade_day_list = w.tdays(BEGIN_DATE, END_DATE, "")

    for day in trade_day_list.Data[0]:
        str_date = day.strftime('%Y-%m-%d')

        list_current = get_items(str_date)

        print "list_current=", list_current

        list_close = list(set(list_record).difference(set(list_current)))
        list_open = list(set(list_current).difference(set(list_record)))

        print "list_record=", list_record
        print "list_close=", list_close
        print "list_open=", list_open

        list_record = list_open

        for close_item in list_close:
            result_close = w.wsd(close_item, "close", "ED0TD", str_date, "")
            close_price = result_close.Data[0][0]

            with open("ak_strategy.csv", "ab+") as csvfile:
                writer = csv.writer(csvfile)
                writer.writerow([str_date, close_item, "CLOSE", close_price])

        for open_item in list_open:
            result_open = w.wsd(open_item, "close", "ED0TD", str_date, "")
            open_price = result_open.Data[0][0]

            with open("ak_strategy.csv", "ab+") as csvfile:
                writer = csv.writer(csvfile)
                writer.writerow([str_date, open_item, "OPEN", open_price])






