#!/usr/bin/env python  
# encoding: utf-8  

""" 
@version: v1.0 
@author: xiaxianba 
@license: Apache Licence  
@contact: scuxia@gmail.com 
@site: http://weibo.com/xiaxianba 
@software: PyCharm 
@file: frame_backtest.py 
@time: 2018/5/28 10:17 
@describe: 
"""

from WindPy import *


# 根据平仓记录和时间，计算平仓收益
def get_profit(date, dict_stock):

    profit = 0.0

    if not isinstance(dict_stock, dict):
        return
    elif not dict_stock:
        return 0.0
    else:
        for stock in dict_stock.keys():
            result_open = w.wsd(stock, "close", dict_stock[stock], dict_stock[stock], "")
            result_close = w.wsd(stock, "close", date, date, "")

            if result_open.ErrorCode != 0 or result_close.ErrorCode != 0:
                return
            else:
                profit += (float(result_close.Data[0][0]) - float(result_open.Data[0][0]))/float(result_open.Data[0][0])

    return profit


# 从所有A股中获取市值排名前5的股票
def get_marketvalue_rank(date):

    dict_return = {}
    dict_stock = {}

    prefix = "date="
    # suffix = ";sectorid=a001010100000000"  # 全部A股
    suffix = ";sectorid=a00103010b000000"  # 上证50指数成份
    scope = prefix + date + suffix

    result = w.wset("sectorconstituent", scope)
    if result.ErrorCode != 0:
        return
    else:
        list_stocks = result.Data[1]

    for stock in list_stocks:
        result_mv = w.wsd(stock, "mkt_cap_ard", date, date, "unit=1")
        if result_mv.ErrorCode != 0:
            continue
        else:
            dict_stock[stock] = result_mv.Data[0][0]

    sort_dict_stock = sorted(dict_stock.items(), key=lambda item: item[1], reverse=True)[30:35]

    for dict_key in dict(sort_dict_stock).keys():
        dict_return[dict_key] = date

    print dict_return
    return dict_return


# 获取交易日列表
def get_trade_day(begin_date, end_date):

    list_trade_day = []
    result_trade_day = w.tdays(begin_date, end_date, "")

    for day in result_trade_day.Data[0]:
        str_date = day.strftime('%Y-%m-%d')
        list_trade_day.append(str_date)

    return list_trade_day

# 函数入口
if __name__ == "__main__":

    total_profit = 0.0
    dict_open = {}
    dict_close = {}
    dict_hold = {}

    BEGIN_DATE = "2017-04-06"
    END_DATE = "2018-05-18"

    w.start()

    list_day = get_trade_day(BEGIN_DATE, END_DATE)
    list_step_day = list_day[::20]

    for day in list_step_day:
        dict_temp = get_marketvalue_rank(day)
        if not dict_hold:
            dict_hold = dict_temp
        else:
            list_open = list(set(dict_temp).difference(set(dict_hold)))
            print list_open
            for open in list_open:
                if open in dict_temp.keys():
                    dict_open[open] = dict_temp[open]

            list_close = list(set(dict_hold).difference(set(dict_temp)))
            print list_close
            for close in list_close:
                if close in dict_hold.keys():
                    dict_close[close] = dict_hold[close]

            print get_profit(day, dict_close)
            total_profit += get_profit(day, dict_close)

    print total_profit
