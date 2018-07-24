#!/usr/bin/env python  
# encoding: utf-8  

""" 
@version: v1.0 
@author: xiaxianba 
@license: Apache Licence  
@contact: scuxia@gmail.com 
@site: http://weibo.com/xiaxianba 
@software: PyCharm 
@file: Mock_Trade.py 
@time: 2018/6/5 14:35 
@describe: 
"""

from WindPy import *
import csv
import datetime
import json


# 从上证50中获取市值排名的股票
def get_marketvalue_rank(date):

    int_position = 1
    dict_return = {}
    dict_return['BUY'] = {}
    dict_return['SELL'] = {}
    dict_stock = {}

    prefix = "date="
    # suffix = ";sectorid=a001010100000000"  # 全部A股
    suffix = ";sectorid=a00103010b000000"  # 上证50指数成份
    scope = prefix + date + suffix

    result_s = w.wset("sectorconstituent", scope)
    if result_s.ErrorCode != 0:
        return
    else:
        list_stocks = result_s.Data[1]

    for stock in list_stocks:
        result_mv = w.wsd(stock, "mkt_cap_ard", date, date, "unit=1")
        if result_mv.ErrorCode != 0:
            continue
        else:
            dict_stock[stock] = result_mv.Data[0][0]

    long_dict_stock = sorted(dict_stock.items(), key=lambda item: item[1], reverse=True)[5:10]
    # short_dict_stock = sorted(dict_stock.items(), key=lambda item: item[1], reverse=True)[-10:-5]

    for long_dict_key in dict(long_dict_stock).keys():
        long_result_price = w.wsd(long_dict_key, "close", date, date, "")
        long_price = long_result_price.Data[0][0]
        dict_return['BUY'][long_dict_key] = [date, int_position, long_price]

    # for short_dict_key in dict(short_dict_stock).keys():
    #     short_result_price = w.wsd(short_dict_key, "close", date, date, "")
    #     short_price = short_result_price.Data[0][0]
    #     dict_return['SELL'][short_dict_key] = [date, int_position, short_price]

    # 将开仓字典写入JSON文件
    with open("open.json", "w") as jsonfile:
        jsonfile.write(json.dumps(dict_return))

    # 从JSON文件中获取开仓字典
    # with open("open.json",'r') as jsonfile:
    #     dict_open = json.load(jsonfile)
    #     print dict_open

    return dict_return


# 获取持仓信息
def get_hold():

    count = 0
    dict_hold = {}
    dict_hold['BUY'] = {}
    dict_hold['SELL'] = {}

    result_login = w.tlogon('0000', 0, 'W100435100501', 'hx123456', 'SHSZ')
    if result_login.ErrorCode != 0:
        return

    result_position = w.tquery('Position', 'LogonID=1')
    if result_position.ErrorCode != 0:
        return

    for variety in result_position.Data[0]:
        dict_hold['BUY'][variety] = [result_position.Data[8][count], result_position.Data[9][count]]
        count += 1

    return dict_hold


# 下单交易函数
def trade_order(dict_order):

    if not dict_order:
        return

    for key in dict_order.keys():
        if key == "BUY":
            for variety in dict_order['BUY'].keys():
                result_last = w.wsq(variety, "rt_latest")
                if result_last.ErrorCode != 0:
                    continue
                order_price = result_last.Data[0][0]
                result_order = w.torder(variety, 'Buy', order_price, dict_order['BUY'][variety][1]*100, 'OrderType=LMT;LogonID=1')
                if result_order.ErrorCode != 0:
                    continue
        # elif key == "SELL":
        #     for variety in dict_order['SELL'].keys():
        #         result_last = w.wsq(variety, "rt_latest")
        #         if result_last.ErrorCode != 0:
        #             continue
        #         order_price = result_last.Data[0][0]
        #         result_order = w.torder(variety, 'Sell', order_price, dict_order['SELL'][variety][1]*100, 'OrderType=LMT;LogonID=1')
        #         if result_order.ErrorCode != 0:
        #             continue

if __name__ == "__main__":

    dict_open = {}
    dict_close = {}
    dict_hold = {}

    w.start()
    # result_login = w.tlogon('0000', 0, 'W100435100502', 'hx123456', ['SHF','CFE','DCE','CZC'])
    result_login = w.tlogon('0000', 0, 'W100435100501', 'hx123456', 'SHSZ')
    # print result_login

    # result_order = w.torder('600000.SH','Sell','10.56','100','OrderType=LMT;LogonID=1')
    # result_order = w.torder('600000.SH', 'Buy', '10.56', '100', 'OrderType=LMT;LogonID=1')
    # print result_order

    # result_cpaital = w.tquery('Capital','LogonID=1')
    # print result_cpaital

    result_position = w.tquery('Position','LogonID=1')
    for field in range(len(result_position.Fields)):
        print result_position.Fields[field]

    # 前一天，可能是交易日也可能是休息日
    # today = datetime.date.today()
    # yesterday = today - datetime.timedelta(days=1)
    date_today = datetime.date.today().strftime('%Y-%m-%d')
    result = w.tdaysoffset(-1, date_today, "")
    date_yesterday = result.Data[0][0].strftime('%Y-%m-%d')

    dict_hold = get_hold()

    dict_open = get_marketvalue_rank(date_yesterday)
    if not dict_hold:
        trade_order(dict_open)
    else:
        for key in dict_open['BUY'].keys():
            if key not in dict_hold['BUY'].keys():
                # 开仓
                result_last = w.wsq(key, "rt_latest")
                order_price = result_last.Data[0][0]
                result_order = w.torder(key, 'Buy', order_price, 100,
                                        'OrderType=LMT;LogonID=1')

        for key in dict_hold['BUY'].keys():
            if key not in dict_open['BUY'].keys():
                # 平仓
                result_last = w.wsq(key, "rt_latest")
                order_price = result_last.Data[0][0]
                result_order = w.torder(key, 'Sell', order_price, 100,
                                        'OrderType=LMT;LogonID=1')




