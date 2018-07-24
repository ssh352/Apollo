#!/usr/bin/env python  
# encoding: utf-8  

""" 
@version: v1.0 
@author: xiaxianba 
@license: Apache Licence  
@contact: scuxia@gmail.com 
@site: http://weibo.com/xiaxianba 
@software: PyCharm 
@file: FootShape_XY.py
@time: 2017/12/26 8:57 
@describe: 

    20171213_兴业证券_陈新钢
    大盘日线："5+1"脚掌策略(交易逻辑：成立-确认-跟进)
      1.当天突破前5根K线高点(隐形条件：K线收到60日均线上方)
      2.近5天K线波动率(波幅[(高-低)/低])缩小
      3.跌破5天K线，需要减仓，或者出场
      4.K7破K6低点
      *5.证券、银行、保险行业指数是市场风向指标
      
    个股日线：(交易逻辑是跟踪主力动向)
      1.均线策略：小周期-3,5,8；大周期-50,55,60
      2.交易量倍量(大于1.9倍)信号 
      3.平台突破，收盘价突破前期所有实体柱的开收盘价
      4.根据脚掌策略判断大盘，同时要熟悉个股的基本面
"""
import csv
import datetime as pydt

from WindPy import *

w.start()

date_today_line = pydt.date.today().strftime('%Y-%m-%d')
result_line_day1 = w.tdaysoffset(-1, date_today_line, "")
result_line_day5 = w.tdaysoffset(-5, date_today_line, "")

date_line_day1 = result_line_day1.Data[0][0].strftime('%Y-%m-%d')
date_start_day1 = result_line_day1.Data[0][0].strftime('%Y%m%d')
date_start_day5 = result_line_day5.Data[0][0].strftime('%Y%m%d')
date_today = pydt.date.today().strftime('%Y%m%d')

result_stock = w.wset("sectorconstituent", "date="+date_today_line+";sectorid=a001010100000000")

stock_list = result_stock.Data[1]

for stock in stock_list:
    # 1.前5根K线高低点
    result_per = w.wss(stock, "high_per,low_per", "startDate="+date_start_day5+";endDate="+date_today+";priceAdj=U")
    High5 = result_per.Data[0][0]
    Low5 = result_per.Data[1][0]

    # 2.前收盘价
    result_close = w.wsd(stock, "pre_close", "ED-1TD", date_line_day1, "")
    pre_close = result_close.Data[0][0]

    stock_price = w.wsq(stock, "rt_last")

    # 3.60日均线
    result_ma = w.wss(stock, "MA", "tradeDate="+date_start_day1+";MA_N=60;priceAdj=U;cycle=D")
    MA60 = result_ma.Data[0][0]

    # 4.成交量
    result_volume = w.wss(stock, "volume", "tradeDate="+date_start_day1+";cycle=D")
    volume_day1 = result_volume.Data[0][0]

    result = w.tdaysoffset(-2, date_today_line, "")
    date_pre = result.Data[0][0].strftime('%Y-%m-%d')

    result_volume_pre = w.wss(stock, "volume", "tradeDate=" + date_pre + ";cycle=D")
    volume_day2 = result_volume_pre.Data[0][0]

    print stock, volume_day1, volume_day2

    if(stock_price > High5 and pre_close > MA60 and volume_day2 > 0 and float(volume_day1)/float(volume_day2) >= 1.9):
        with open(date_today_line+".txt", "ab+") as txtfile:
            txtfile.write(stock+'\n')










