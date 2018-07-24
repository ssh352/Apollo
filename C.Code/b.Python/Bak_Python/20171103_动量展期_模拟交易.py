#!/usr/bin/env python  
# encoding: utf-8  

""" 
@version: v1.0 
@author: xiaxianba 
@license: Apache Licence  
@contact: scuxia@gmail.com 
@site: http://weibo.com/xiaxianba 
@software: PyCharm 
@file: SimTrade.py 
@time: 2017/11/2 14:09 
@describe: 动量-展期策略模拟交易
"""

import csv
import datetime as pydt
import numpy as np
import os
from WindPy import *

HOLDPERIOD = 5
SORTPERIOD = 20
BASEVOLUME = 100000.0

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


# 获取符合十万手成交量的商品合约
def get_valid_list():

    list_valid_item = []
    date_now = pydt.date.today() - pydt.timedelta(days=1)

    for item in list_item:
        result = w.wsd(item, "volume", "ED-9TD", date_now, "")

        if result.ErrorCode == 0:
            ma = float(sum(result.Data[0])/len(result.Data[0]))

            if ma > BASEVOLUME:
                list_valid_item.append(item)

    return list_valid_item


# 根据动量排名获取多空组合
def get_momentum_list(mom_list):

    dict_item = {}

    date_now = pydt.date.today() - pydt.timedelta(days=1)
    period = "ED-" + str(SORTPERIOD-1) + "TD"

    for mom in mom_list:
        result = w.wsd(mom, "close", period, date_now, "")

        if result.ErrorCode == 0:
            dict_item[mom] = float((result.Data[0][-1]-result.Data[0][0])/result.Data[0][0])

    dict_sort = sorted(dict_item.items(), key=lambda item: item[1])

    short_momlist = dict_sort[:len(dict_sort)//2]
    long_momlist = dict_sort[len(dict_sort)//2:]

    return long_momlist, short_momlist


# 根据展期收益率排名获取多空组合
def get_extension_list(ext_list, flag):

    dict_rate = {}

    # date_now = (pydt.date.today() - pydt.timedelta(days=1)).strftime('%Y-%m-%d')
    date_base = pydt.date.today().strftime('%Y-%m-%d')
    result = w.tdaysoffset(-1, date_base, "")
    date_now = result.Data[0][0].strftime('%Y-%m-%d')

    prefix = "date="
    suffix = ";sectorid="

    for item in ext_list:
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

                diff_price = np.log(float(main_contract_price.Data[0][0])) - np.log(float(second_contract_price.Data[0][0]))
                diff_date = (second_contract_delivery.Data[0][0] - main_contract_delivery.Data[0][0]).days
                dict_rate[item] = 365 * diff_price / diff_date

    if flag == 1:
        long_extlist = sorted(dict_rate.items(), key=lambda item: item[1], reverse=True)[:len(dict_rate)//2]
        return long_extlist

    if flag == 0:
        short_extlist = sorted(dict_rate.items(), key=lambda item: item[1])[:len(dict_rate) // 2]
        return short_extlist


# 输出交易品种记录
def get_trade_item(trade_list, flag):

    list_main = []
    # date_now = (pydt.date.today() - pydt.timedelta(days=1)).strftime('%Y-%m-%d')
    date_base = pydt.date.today().strftime('%Y-%m-%d')
    result = w.tdaysoffset(-1, date_base, "")
    date_now = result.Data[0][0].strftime('%Y-%m-%d')

    for trade in trade_list:
        result_main = w.wsd(trade, "trade_hiscode", date_now, date_now, "")
        list_main.append(result_main.Data[0][0])

    list_main.insert(0, date_now)

    if flag == 1:
        if os.path.exists("long.csv"):
            with open("long.csv", "r+") as csvfile:
                lines = csv.reader(csvfile)
                for line in lines:
                    date_pre = line[0]

            date_now_t = datetime.strptime(date_now, "%Y-%m-%d")
            date_pre_t = datetime.strptime(date_pre, "%Y/%m/%d")

            if (date_now_t - date_pre_t).days >= 5:
                with open("long.csv", "ab+") as csvfile:
                    writer = csv.writer(csvfile)
                    writer.writerow(list_main)

        else:
            with open("long.csv", "ab+") as csvfile:
                writer = csv.writer(csvfile)
                writer.writerow(list_main)

    if flag == 0:
        if os.path.exists("short.csv"):
            with open("short.csv", "r+") as csvfile:
                lines = csv.reader(csvfile)
                for line in lines:
                    date_pre = line[0]

            date_now_t = datetime.strptime(date_now, "%Y-%m-%d")
            date_pre_t = datetime.strptime(date_pre, "%Y-%m-%d")

            if (date_now_t - date_pre_t).days >= 5:
                with open("short.csv", "ab+") as csvfile:
                    writer = csv.writer(csvfile)
                    writer.writerow(list_main)
        else:
            with open("short.csv", "ab+") as csvfile:
                writer = csv.writer(csvfile)
                writer.writerow(list_main)

if __name__ == "__main__":

    w.start()

    list_valid = get_valid_list()

    long_list, short_list = get_momentum_list(list_valid)

    long_key = dict(long_list).keys()
    short_key = dict(short_list).keys()

    long_item = get_extension_list(long_key, 1)
    short_item = get_extension_list(short_key, 0)

    get_trade_item(dict(long_item).keys(), 1)
    get_trade_item(dict(short_item).keys(), 0)


