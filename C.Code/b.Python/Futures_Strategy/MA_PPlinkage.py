#!/usr/bin/env python  
# encoding: utf-8  

""" 
@version: v1.0 
@author: xiaxianba 
@license: Apache Licence  
@contact: scuxia@gmail.com 
@site: http://weibo.com/xiaxianba 
@software: PyCharm 
@file: MA_PPlinkage.py 
@time: 2018/8/6 9:28 
@describe: 
"""

from WindPy import *

DATE_BEGIN = "2017-1-1"
DATE_END = "2018-7-31"


# 获取交易日列表
def get_trade_day(begin_date, end_date):

    list_trade_day = []
    result_trade_day = w.tdays(begin_date, end_date, "")

    for day in result_trade_day.Data[0]:
        str_date = day.strftime('%Y-%m-%d')
        list_trade_day.append(str_date)

    return list_trade_day

if __name__ == "__main__":

    w.start()

    list_trade_day = get_trade_day(DATE_BEGIN, DATE_END)

    sum_half = 0.0
    sum_one = 0.0
    sum_all = 0.0

    for day in list_trade_day:

        result = w.tdaysoffset(-1, day, "")
        date_last = result.Data[0][0].strftime('%Y-%m-%d')

        open_ma_last = w.wsi("MA.CZC", "open", date_last+" 21:00:00", date_last+" 21:01:00", "")
        close_ma_last = w.wsi("MA.CZC", "close", date_last+" 23:29:00", date_last+" 23:30:00", "")

        open_ma_half = w.wsi("PP.DCE", "open", day + " 9:00:00", day + " 9:01:00", "")
        close_ma_half = w.wsi("PP.DCE", "close", day + " 9:29:00", day + " 9:30:00", "")

        open_ma_one = w.wsi("PP.DCE", "open", day+" 9:00:00", day+" 9:01:00","periodstart=9:00:00;periodend=9:01:00")
        close_ma_one = w.wsi("PP.DCE", "close", day+" 9:59:00", day+" 10:00:00","periodstart=9:59:00;periodend=10:00:00")

        open_ma_all = w.wsi("PP.DCE", "open", day+" 9:00:00", day+" 9:01:00", "periodstart=9:00:00;periodend=9:01:00")
        close_ma_all = w.wsi("PP.DCE", "close", day+" 14:59:00", day+" 15:00:00","periodstart=14:59:00;periodend=15:00:00")

        range_last = (close_ma_last.Data[0][0]-open_ma_last.Data[0][0])/open_ma_last.Data[0][0]
        range_half = (close_ma_half.Data[0][0] - open_ma_half.Data[0][0]) / open_ma_half.Data[0][0]
        range_one = (close_ma_one.Data[0][0] - open_ma_one.Data[0][0]) / open_ma_one.Data[0][0]
        range_all = (close_ma_all.Data[0][0] - open_ma_all.Data[0][0]) / open_ma_all.Data[0][0]

        # 不加阈值情况下，时间越短收益越大，印证MA和PP有盈利联动性关系。接下来的难点在：如何最小化交易成本。
        if range_last > 0.005:
            sum_half += range_half
            sum_one += range_one
            sum_all += range_all
        elif range_last < -0.005:
            sum_half -= range_half
            sum_one -= range_one
            sum_all -= range_all

        # print date_last, range_last, range_half, range_one, range_all
        print date_last, range_last, sum_half, sum_one, sum_all




    # result = w.wset("sectorconstituent", "date=2018-07-26;sectorid=a001010100000000")
    # result = w.wset("tradesuspend","startdate=2018-08-02;enddate=2018-08-02;field=date,wind_code")
    # print result.Data[1]