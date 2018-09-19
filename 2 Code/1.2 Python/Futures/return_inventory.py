#!/usr/bin/env python  
# encoding: utf-8  

""" 
@version: v1.0 
@author: xiaxianba 
@license: Apache Licence  
@contact: scuxia@gmail.com 
@site: http://weibo.com/xiaxianba 
@software: PyCharm 
@file: return_inventory.py 
@time: 2018/6/27 16:20 
@describe: 
"""

from WindPy import *

import csv
import pandas as pd

dict_inventory = {'CU': ['S0049493', 'S0114144'], 'AL': ['S0049494', 'S0029756'], 'PB': ['S0167708', 'S0029764'],
                  'ZN': ['S0049498', 'S0029760'], 'NI': ['S0213018', 'S0029772'], 'SN': ['S0213019', 'S0029768'],
                  'AU': ['S0049505', 'S0114145'], 'AG': ['S0174908', 'S0114146'], 'RU': ['S0049495'],
                  'RB': ['S0181750'], 'HC': ['S0110143'], 'BU': ['S0204821'], 'A': ['S5041136', 'S0117164'],
                  'M': ['S5041144'], 'Y': ['S5063139', 'S5028184'], 'P': ['S5006381', 'S5029336'],
                  'C': ['S0049491', 'S5063274', 'S5063275'], 'CS': ['S0243229'], 'L': ['S0049492', 'S5446196'],
                  'PP': ['S0205222', 'S5471283'], 'PVC': ['S0180500', 'S5471265'], 'JM': ['S5125686', 'S5133418'],
                  'J': ['S5120058', 'S5116629', 'S5116630', 'S5136709', 'S5133852'], 'I': ['S0110152'],
                  'CF': ['S0049502', 'S0049517'], 'SR': ['S0049502', 'S0049517'], 'OI': ['S5041913', 'S0164192'],
                  'RM': ['S0182089'], 'ZC': ['S5116621', 'S5118163', 'S5103725', 'S5103728'],
                  'MA': ['S5436526', 'S5436527'], 'PTA': ['S0049499'], 'FG': ['S5914176']}

shf_suffix = ['CU', 'AL', 'ZN', 'PB', 'AU', 'AG', 'NI', 'SN', 'RB', 'WR', 'HC', 'FU', 'BU', 'RU']

dce_suffix = ['M', 'Y', 'A', 'B', 'P', 'C', 'J', 'V', 'I', 'BB', 'FB', 'L', 'PP', 'JM', 'JD','CS']

czc_suffix = ['CY', 'SR', 'CF', 'ZC', 'FG', 'TA', 'MA', 'WH', 'PM', 'RI', 'LR', 'JR', 'RS',
              'OI', 'RM', 'SF', 'SM']

str_startdate = "2009-01-01"
str_enddate = "2018-06-08"


# 获取交易日列表
def get_trade_day(begin_date, end_date):

    list_trade_day = []
    result_trade_day = w.tdays(begin_date, end_date, "")

    for day in result_trade_day.Data[0]:
        str_date = day.strftime('%Y-%m-%d')
        list_trade_day.append(str_date)

    return list_trade_day


# 转换成交易所交易交易合约代码
def main_contract(str_contract):
    if str_contract in shf_suffix:
        str_contract += '.SHF'
    elif str_contract in dce_suffix:
        str_contract += '.DCE'
    elif str_contract in czc_suffix:
        str_contract += '.CZC'
    elif str_contract == 'PTA':
        str_contract = 'TA.CZC'
    elif str_contract == 'PVC':
        str_contract = 'V.DCE'

    return str_contract


# 转换成商品指数易交易合约代码
def index_contract(str_contract):
    if str_contract in shf_suffix:
        str_contract += 'FI.WI'
    elif str_contract in dce_suffix:
        str_contract += 'FI.WI'
    elif str_contract in czc_suffix:
        str_contract += 'FI.WI'
    elif str_contract == 'PTA':
        str_contract = 'TAFI.WI'
    elif str_contract == 'PVC':
        str_contract = 'VFI.WI'

    return str_contract


# 获取日内收益率
def get_return(variety, date):

    int_volume = 0
    list_main = []

    date_last_result = w.tdaysoffset(-1, date, "")
    date_last = date_last_result.Data[0][0]

    # price_today_result = w.wsd(index_contract(variety), "close", date, date, "")
    # price_last_result = w.wsd(index_contract(variety), "close", date_last, date_last, "")
    volume_today_result = w.wsd(index_contract(variety), "volume", date, date, "")
    int_volume = volume_today_result.Data[0][0]

    # if price_today_result.ErrorCode == 0 and price_last_result.ErrorCode == 0 and \
    #     price_today_result.Data[0][0] and price_last_result.Data[0][0]:
    #     # print variety, date, price_today_result.Data[0][0], price_last_result.Data[0][0]
    #     return_ratio = float(price_today_result.Data[0][0]) / float(price_last_result.Data[0][0]) -1
    # else:
    #     return_ratio = ''
    # list_main.append(return_ratio)
    # list_main.insert(0, date)

    list_main.append(int_volume)
    list_main.insert(0, date)

    with open("./volume/" + variety + ".csv", "ab+") as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(list_main)

if __name__ == "__main__":

    w.start()
    list_day = get_trade_day(str_startdate, str_enddate)

    for variety in dict_inventory.keys():
        for day in list_day:
            date_day = datetime.strptime(day, "%Y-%m-%d")
            get_return(variety, date_day)
