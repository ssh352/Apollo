#!/usr/bin/env python  
# encoding: utf-8  

""" 
@version: v1.0 
@author: xiaxianba 
@license: Apache Licence  
@contact: scuxia@gmail.com 
@site: http://weibo.com/xiaxianba 
@software: PyCharm 
@file: PickStock.py 
@time: 2018/1/30 9:33 
@describe: 
  1.最近三天价格上升
  2.最近三天的3日成交量均价上升
  3*.大盘符合脚掌交易策略
  
"""

import csv
import datetime as pydt
from WindPy import *

stock_list = ['000002.SZ', '000039.SZ', '000063.SZ', '000100.SZ', '000333.SZ', '000488.SZ', '000726.SZ', '000876.SZ',
              '000901.SZ', '002008.SZ', '002027.SZ', '002241.SZ', '002340.SZ', '002352.SZ', '002415.SZ', '002450.SZ',
              '002701.SZ', '002714.SZ', '300003.SZ', '300070.SZ', '300072.SZ', '300408.SZ', '600018.SH', '600019.SH',
              '600028.SH', '600030.SH', '600050.SH', '600104.SH', '600196.SH', '600201.SH', '600271.SH', '600276.SH',
              '600297.SH', '600309.SH', '600398.SH', '600518.SH', '600519.SH', '600583.SH', '600585.SH', '600598.SH',
              '600611.SH', '600612.SH', '600663.SH', '600703.SH', '600704.SH', '600739.SH', '600741.SH', '600754.SH',
              '600811.SH', '600835.SH', '600887.SH', '600893.SH', '600900.SH', '601006.SH', '601088.SH', '601111.SH',
              '601158.SH', '601318.SH', '601398.SH', '601607.SH', '601668.SH', '601727.SH', '601766.SH', '601857.SH',
              '601888.SH', '603993.SH', ]

w.start()

datenow = pydt.date.today().strftime('%Y-%m-%d')
result1 = w.tdaysoffset(-1, datenow, "")
date1 = result1.Data[0][0].strftime('%Y-%m-%d')

result2 = w.tdaysoffset(-2, datenow, "")
date2 = result2.Data[0][0].strftime('%Y-%m-%d')

result3 = w.tdaysoffset(-3, datenow, "")
date3 = result3.Data[0][0].strftime('%Y-%m-%d')

result4 = w.tdaysoffset(-4, datenow, "")
date4 = result4.Data[0][0].strftime('%Y-%m-%d')

result5 = w.tdaysoffset(-5, datenow, "")
date5 = result5.Data[0][0].strftime('%Y-%m-%d')

for stock in stock_list:
    result_p1 = w.wsd(stock, "close,volume", date1, date1, "")
    price1 = result_p1.Data[0][0]
    volume1 = result_p1.Data[1][0]

    result_p2 = w.wsd(stock, "close,volume", date2, date2, "")
    price2 = result_p2.Data[0][0]
    volume2 = result_p2.Data[1][0]

    result_p3 = w.wsd(stock, "close,volume", date3, date3, "")
    price3 = result_p3.Data[0][0]
    volume3 = result_p3.Data[1][0]

    result_p4 = w.wsd(stock, "close,volume", date4, date4, "")
    volume4 = result_p4.Data[1][0]

    result_p5 = w.wsd(stock, "close,volume", date5, date5, "")
    volume5 = result_p5.Data[1][0]

    avg_v1 = volume1 + volume2 + volume3
    avg_v2 = volume2 + volume3 + volume4
    avg_v3 = volume3 + volume4 + volume5

    if price1 > price2 > price3 and avg_v1 > avg_v2 > avg_v3:
        with open(datenow+"_stocklist.csv", "ab+") as csvfile:
            writer = csv.writer(csvfile)
            writer.writerow([stock, price1])



