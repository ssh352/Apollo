#!/usr/bin/env python  
# encoding: utf-8  

""" 
@version: v1.0 
@author: xiaxianba 
@license: Apache Licence  
@contact: scuxia@gmail.com 
@site: http://weibo.com/xiaxianba 
@software: PyCharm 
@file: Roll_Backtest.py 
@time: 2018/1/10 20:08 
@describe: 
方法一（本程序采用）：
  1.根据主力合约的展期收益率排名，转化成具体的合约字典 e.g. {'CU1803.SHF':2018-01-08}
  2.设置两个字典存放展期收益率，每次比较两个字典中的key，如果在新的字典中没有就平仓，如果在PRE字典中没有就加仓，否则不变
  3.最终获取百分比收益率，计算总的收益
方法二：
  1.根据字典嵌套 {'CU1803:{'2017-12-12':54000}}
"""

import csv
import datetime as pydt
import numpy as np

from WindPy import *

DICT_BUY = {}
DICT_SELL = {}

DICT_PRE_BUY = {}
DICT_PRE_SELL = {}

# 国内三大期货交易所共47个商品合约
list_item = ['CU.SHF', 'AL.SHF', 'ZN.SHF', 'PB.SHF', 'AU.SHF', 'AG.SHF', 'NI.SHF', 'SN.SHF',
             'RB.SHF', 'WR.SHF', 'HC.SHF', 'FU.SHF', 'BU.SHF', 'RU.SHF', 'M.DCE', 'Y.DCE',
             'A.DCE', 'B.DCE', 'P.DCE', 'C.DCE', 'J.DCE', 'V.DCE', 'I.DCE',
             'BB.DCE', 'FB.DCE', 'L.DCE',  'PP.DCE',  'JM.DCE', 'JD.DCE','CS.DCE', 'CY.CZC',
             'SR.CZC', 'CF.CZC', 'ZC.CZC', 'FG.CZC', 'TA.CZC', 'MA.CZC', 'WH.CZC', 'PM.CZC',
             'RI.CZC', 'LR.CZC', 'JR.CZC', 'RS.CZC', 'OI.CZC', 'RM.CZC', 'SF.CZC', 'SM.CZC', ]

# 商品合约对应的Wind板块id
dict_item = {'CU.SHF':'a599010202000000', 'AL.SHF':'a599010203000000', 'ZN.SHF':'a599010204000000',
             'PB.SHF':'1000002892000000', 'AU.SHF':'a599010205000000', 'AG.SHF':'1000006502000000',
             'NI.SHF':'1000011457000000', 'SN.SHF':'1000011458000000', 'RB.SHF':'a599010206000000',
             'WR.SHF':'a599010207000000', 'HC.SHF':'1000011455000000', 'FU.SHF':'a599010209000000',
             'BU.SHF':'1000011013000000', 'RU.SHF':'a599010208000000', 'M.DCE':'a599010304000000',
             'Y.DCE':'a599010306000000', 'A.DCE':'a599010302000000', 'B.DCE':'a599010303000000',
             'P.DCE':'a599010307000000', 'C.DCE':'a599010305000000', 'J.DCE':'1000002976000000',
             'V.DCE':'a599010309000000', 'I.DCE':'1000011439000000', 'BB.DCE':'1000011466000000',
             'FB.DCE':'1000011465000000', 'L.DCE':'a599010308000000',  'PP.DCE':'1000011468000000',
             'JM.DCE':'1000009338000000', 'JD.DCE':'1000011464000000','CS.DCE':'1000011469000000',
             'CY.CZC':'1000011479000000', 'SR.CZC':'a599010405000000', 'CF.CZC':'a599010404000000',
             'ZC.CZC':'1000011012000000', 'FG.CZC':'1000008549000000', 'TA.CZC':'a599010407000000',
             'MA.CZC':'1000005981000000', 'WH.CZC':'a599010403000000', 'PM.CZC':'1000006567000000',
             'RI.CZC':'a599010406000000', 'LR.CZC':'1000011476000000', 'JR.CZC':'1000011474000000',
             'RS.CZC':'1000008621000000', 'OI.CZC':'a599010408000000', 'RM.CZC':'1000008622000000',
             'SF.CZC':'1000011478000000', 'SM.CZC':'1000011477000000'}


# 获取交易日列表
def get_trade_day(begin_date, end_date):

    list_trade_day = []
    result_trade_day = w.tdays(begin_date, end_date, "")

    for day in result_trade_day.Data[0]:
        str_date = day.strftime('%Y-%m-%d')
        list_trade_day.append(str_date)

    return list_trade_day


# 计算某一天展期收益率的排序
def get_roll_sort(date_base, ext_list):

    dict_rate = {}
    dict_current_buy = {}
    dict_current_sell = {}
    prefix = "date="
    suffix = ";sectorid="

    for item in ext_list:
        list_contract = []
        dicts = {}
        scope = prefix + date_base + suffix + dict_item[item]
        result = w.wset("sectorconstituent", scope)

        if result.ErrorCode == 0 and result.Data:
            list_contract = result.Data[1]

            for contract in list_contract:
                result_volume = w.wsd(contract, "volume", date_base, date_base, "")

                if result_volume.ErrorCode == 0:
                    dicts[contract] = result_volume.Data[0][0]

            result_main = w.wsd(item, "trade_hiscode", date_base, date_base, "")

            if result_main.ErrorCode == 0 and result_main.Data[0][0]:
                main_contract = result_main.Data[0][0]
                main_contract_price = w.wsd(main_contract, "close", date_base, date_base, "")
                # main_contract_price = w.wsi(main_contract, "close", date_base + " 11:00:00",
                #                            date_base + " 11:30:00", "BarSize=30")
                main_contract_delivery = w.wsd(main_contract, "lastdelivery_date", date_base, date_base, "")

                second_contract = sorted(dicts.items(), key=lambda item: item[1], reverse=True)[:1]
                second_contract_price = w.wsd(dict(second_contract).keys(), "close", date_base, date_base, "")
                # second_contract_price = w.wsi(dict(second_contract).keys(), "close", date_base + " 11:00:00",
                #                              date_base + " 11:30:00", "BarSize=30")
                second_contract_delivery = w.wsd(dict(second_contract).keys(), "lastdelivery_date", date_base, date_base, "")

                if isinstance(main_contract_price.Data[0][0],float) and isinstance(second_contract_price.Data[0][0],float) \
                    and type(main_contract_delivery.Data[0][0]) is not unicode and type(second_contract_delivery.Data[0][0]) is not unicode:
                    diff_price = np.log(float(main_contract_price.Data[0][0])) - np.log(float(second_contract_price.Data[0][0]))
                    diff_date = (second_contract_delivery.Data[0][0] - main_contract_delivery.Data[0][0]).days
                    dict_rate[item] = 365 * diff_price / diff_date

    long_extlist = sorted(dict_rate.items(), key=lambda item: item[1], reverse=True)[:len(dict_rate)//4]
    short_extlist = sorted(dict_rate.items(), key=lambda item: item[1])[:len(dict_rate) // 4]

    for key in dict(long_extlist).keys():
        result_main_buy = w.wsd(key, "trade_hiscode", date_base, date_base, "")
        dict_current_buy[result_main_buy.Data[0][0]] = date_base

    for key in dict(short_extlist).keys():
        result_main_sell = w.wsd(key, "trade_hiscode", date_base, date_base, "")
        dict_current_sell[result_main_sell.Data[0][0]] = date_base

    return dict_current_buy, dict_current_sell


# 计算同一个标的不同日期的价格差
def get_roll_profit(item, open_date, close_date):

    data_file = "Roll_Backtest30_0413.csv"

    main_openprice = w.wsd(item, "close", open_date, open_date, "")
    main_closeprice = w.wsd(item, "close", close_date, close_date, "")

    if not main_closeprice.Data[0][0] or not main_openprice.Data[0][0]:
        profit = 0
    else:
        profit = (main_closeprice.Data[0][0] - main_openprice.Data[0][0])/main_openprice.Data[0][0]

    with open(data_file, "ab+") as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow([item, open_date, main_openprice.Data[0][0], close_date, main_closeprice.Data[0][0], profit])

    return profit

if __name__ == "__main__":

    total_profit = 0.0

    w.start()

    list_day = get_trade_day("2018-02-06", "2018-04-13")
    list_step_day = list_day[::5]

    for day in list_step_day:
        DICT_BUY, DICT_SELL = get_roll_sort(day, list_item)

        if not DICT_PRE_BUY or not DICT_PRE_SELL:
            DICT_PRE_BUY = DICT_BUY
            DICT_PRE_SELL = DICT_SELL
            continue

        list_open_buy = list(set(DICT_BUY.keys()).difference(set(DICT_PRE_BUY.keys())))
        list_close_buy = list(set(DICT_PRE_BUY.keys()).difference(set(DICT_BUY.keys())))
        list_open_sell = list(set(DICT_SELL.keys()).difference(set(DICT_PRE_SELL.keys())))
        list_close_sell = list(set(DICT_PRE_SELL.keys()).difference(set(DICT_SELL.keys())))

        for close_item in list_close_buy:
            total_profit += get_roll_profit(close_item, DICT_PRE_BUY[close_item], day)

        for close_buy in list_close_buy:
            del DICT_PRE_BUY[close_buy]

        for buy in list_open_buy:
            DICT_PRE_BUY[buy] = day


