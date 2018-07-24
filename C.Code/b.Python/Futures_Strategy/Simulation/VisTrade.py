#!/usr/bin/env python  
# encoding: utf-8  

""" 
@version: v1.0 
@author: xiaxianba 
@license: Apache Licence  
@contact: scuxia@gmail.com 
@site: http://weibo.com/xiaxianba 
@software: PyCharm 
@file: VisTrade.py 
@time: 2018/3/27 17:58 
@describe: 
"""

import collections
import copy
import csv
import datetime as pydt
import numpy as np
import os
from WindPy import *

# 国内三大期货交易所共47个商品合约
list_item = ['A.DCE', 'AG.SHF', 'AL.SHF', 'AU.SHF', 'B.DCE', 'BB.DCE', 'BU.SHF', 'C.DCE',
             'CF.CZC', 'CS.DCE', 'CU.SHF', 'CY.CZC', 'FB.DCE', 'FG.CZC', 'FU.SHF', 'HC.SHF',
             'I.DCE', 'J.DCE', 'JD.DCE', 'JM.DCE', 'JR.CZC', 'L.DCE', 'LR.CZC', 'M.DCE',
             'MA.CZC', 'NI.SHF', 'OI.CZC', 'P.DCE', 'PB.SHF', 'PM.CZC', 'PP.DCE', 'RB.SHF',
             'RI.CZC', 'RM.CZC', 'RS.CZC', 'RU.SHF', 'SF.CZC', 'SM.CZC', 'SN.SHF', 'SR.CZC',
             'TA.CZC', 'V.DCE', 'WH.CZC', 'WR.SHF', 'Y.DCE', 'ZC.CZC', 'ZN.SHF']

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


# 根据展期收益率排名获取多空组合
def get_extension_list(date_now):

    dict_rate = {}

    prefix = "date="
    suffix = ";sectorid="

    if not os.path.exists("ranking.csv"):

        list_csv_title = copy.copy(list_item)
        list_csv_title.insert(0, "Contracts")

        with open("ranking.csv", "ab+") as csvfile:
            writer = csv.writer(csvfile)
            writer.writerow(list_csv_title)

    for item in list_item:
        dicts = {}
        scope = prefix + date_now + suffix + dict_item[item]
        result = w.wset("sectorconstituent", scope)

        if result.ErrorCode == 0:
            list_contract = result.Data[1]

            for contract in list_contract:
                result_volume = w.wsd(contract, "volume", date_now, date_now, "")

                if result_volume.ErrorCode == 0:
                    dicts[contract] = result_volume.Data[0][0]

            result_main = w.wsd(item, "trade_hiscode", date_now, date_now, "")

            if result_main.ErrorCode == 0 and len(result_main.Data[0]) != 0:
                main_contract = result_main.Data[0][0]
                main_contract_price = w.wsd(main_contract, "close", date_now, date_now, "")
                main_contract_delivery = w.wsd(main_contract, "lastdelivery_date", date_now, date_now, "")

                dicts.pop(main_contract)

                second_contract = sorted(dicts.items(), key=lambda item: item[1], reverse=True)[:1]
                second_contract_price = w.wsd(dict(second_contract).keys(), "close", date_now, date_now, "")
                second_contract_delivery = w.wsd(dict(second_contract).keys(), "lastdelivery_date", date_now, date_now, "")

                if main_contract_price.Data[0][0] is None or second_contract_price.Data[0][0] is None:
                    dict_rate[item] = "N/A"
                else:
                    diff_price = np.log(float(main_contract_price.Data[0][0])) - np.log(float(second_contract_price.Data[0][0]))
                    diff_date = (second_contract_delivery.Data[0][0] - main_contract_delivery.Data[0][0]).days
                    dict_rate[item] = 365 * diff_price / diff_date

    sort_list_rate = sorted(dict_rate.items(), key=lambda item:item[0])

    print "sort_list_rate=", sort_list_rate

    sort_dict_rate = collections.OrderedDict(sort_list_rate)

    print "sort_dict_rate=", sort_dict_rate

    list_rate = list(sort_dict_rate.values())
    # list_rate = sort_list_rate.values()
    list_rate.insert(0, date_now)

    print "list_rate=", list_rate

    with open("ranking.csv", "ab+") as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(list_rate)


# 获取交易日列表
def get_trade_day(begin_date, end_date):

    list_trade_day = []
    result_trade_day = w.tdays(begin_date, end_date, "")

    for day in result_trade_day.Data[0]:
        str_date = day.strftime('%Y-%m-%d')
        list_trade_day.append(str_date)

    return list_trade_day

if __name__ == "__main__":

    BEGIN_DATE = "2018-02-06"
    END_DATE = "2018-04-15"

    w.start()

    list_day = get_trade_day("2018-05-01", "2018-05-18")
    list_step_day = list_day[::5]

    # trade_day_list = w.tdays(BEGIN_DATE, END_DATE, "")
    #for day in trade_day_list.Data[0]:
    #   str_date = day.strftime('%Y-%m-%d')
    for day in list_step_day:
        get_extension_list(day)
