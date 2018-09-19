#!/usr/bin/env python  
# encoding: utf-8  

""" 
@version: v1.0 
@author: xiaxianba 
@license: Apache Licence  
@contact: scuxia@gmail.com 
@site: http://weibo.com/xiaxianba 
@software: PyCharm 
@file: strategy_inventory.py 
@time: 2018/6/14 10:01 
@describe: 
"""

from WindPy import *
import csv
import pandas as pd

FIXEDAMOUNT = 10000

dict_inventory = {'CU': ['S0049493', 'S0029752', 'S0114144'], 'AL': ['S0049494', 'S0029756'], 'PB': ['S0167708', 'S0029764'],
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
# str_startdate = "2015-09-01"
str_enddate = "2018-06-08"
str_path = "./data/"


# 获取交易日列表
def get_trade_day(begin_date, end_date):

    list_trade_day = []
    result_trade_day = w.tdays(begin_date, end_date, "")

    for day in result_trade_day.Data[0]:
        str_date = day.strftime('%Y-%m-%d')
        list_trade_day.append(str_date)

    return list_trade_day


# 获取同比、环比数据
def get_ratio(date):

    same_ratio = 0.0
    ring_ratio = 0.0
    dict_same_ratio = {}
    dict_ring_ratio = {}
    dict_rate = {}

    date_today = date
    date_month_ago = date_today - pd.tseries.offsets.DateOffset(months=1)
    date_year_ago = date_today - pd.tseries.offsets.DateOffset(months=12)

    for variety in dict_inventory.keys():
        pd_variety = pd.read_excel("./data/" +variety+ ".xlsx")

        if date_today in pd_variety.index and date_month_ago in pd_variety.index and \
                date_year_ago in pd_variety.index and pd_variety.loc[date_today].sum() != 0 \
                and pd_variety.loc[date_year_ago].sum() != 0 and pd_variety.loc[date_month_ago].sum() != 0\
                and not pd_variety.loc[date_today].isnull().any():
            same_ratio = float(pd_variety.loc[date_today].sum())/pd_variety.loc[date_year_ago].sum()
            ring_ratio = float(pd_variety.loc[date_today].sum())/pd_variety.loc[date_month_ago].sum()

            # print variety, pd_variety.loc[date_today], pd_variety.loc[date_today].isnull().any()
            # print date, variety, pd_variety.loc[date_today].sum(), pd_variety.loc[date_month_ago].sum(), \
            #    pd_variety.loc[date_year_ago].sum(), same_ratio, ring_ratio

            dict_same_ratio[variety] = same_ratio
            dict_ring_ratio[variety] = ring_ratio

            # 策略2：环比与同比之差
            dict_rate[variety] = float(ring_ratio-same_ratio)

            # 策略3：横截面季节性策略，同比/环比
            # dict_rate[variety] = float(ring_ratio/same_ratio)

    # return dict_same_ratio, dict_ring_ratio
    return dict_rate


# 获取排序之后的字典
def get_sort_dict(dict_rate):

    list_long = sorted(dict_rate.items(), key=lambda item: item[1], reverse=True)[:len(dict_rate) // 5]
    list_short = sorted(dict_rate.items(), key=lambda item: item[1])[:len(dict_rate) // 5]

    dict_long = dict(list_long)
    dict_short = dict(list_short)

    return dict_long, dict_short


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


# 获取多空交易列表
def get_trade_list():
    pass

if __name__ == "__main__":

    dict_same_ratio = {}
    dict_ring_ratio = {}

    total_profit = 0.0
    list_profit = []
    dict_hold = {}
    dict_hold['BUY'] = {}
    dict_hold['SELL'] = {}

    w.start()

    list_day = get_trade_day(str_startdate, str_enddate)
    list_step_day = list_day[::5]

    for day in list_step_day:

        profit = 0.0
        dict_open = {}
        dict_open['BUY'] = {}
        dict_open['SELL'] = {}

        date_day = datetime.strptime(day, "%Y-%m-%d")
        dict_same_ratio = get_ratio(date_day)

        dict_long, dict_short = get_sort_dict(dict_same_ratio)

        for l_key in dict_long.keys():
            l_result_main = w.wsd(main_contract(l_key), "trade_hiscode", day, day, "")
            l_main_contract = l_result_main.Data[0][0]
            l_main_contract_price = w.wsd(l_main_contract, "close", day, day, "")
            print "l_main_contract_price:", l_key, l_main_contract, l_main_contract_price
            dict_open['BUY'][l_main_contract] = [day, FIXEDAMOUNT/float(l_main_contract_price.Data[0][0]), float(l_main_contract_price.Data[0][0])]

        for s_key in dict_short.keys():
            s_result_main = w.wsd(main_contract(s_key), "trade_hiscode", day, day, "")
            s_main_contract = s_result_main.Data[0][0]
            s_main_contract_price = w.wsd(s_main_contract, "close", day, day, "")
            print "s_main_contract_price:", s_key, s_main_contract, s_main_contract_price
            dict_open['SELL'][s_main_contract] = [day, FIXEDAMOUNT/float(s_main_contract_price.Data[0][0]), float(s_main_contract_price.Data[0][0])]

        if not dict_hold:
            dict_hold = dict_open
        else:
            for b_key in dict_hold['BUY'].keys():
                if b_key not in dict_open['BUY'].keys():
                    b_price_open = dict_hold['BUY'][b_key][2]
                    b_price_close_result = w.wsd(b_key, "close", day, day, "")
                    b_price_close = b_price_close_result.Data[0][0]

                    if b_price_close is None:
                        b_price_close = b_price_open

                    profit += (b_price_close - b_price_open)*dict_hold['BUY'][b_key][1]
                    dict_hold['BUY'].pop(b_key)

            for s_key in dict_hold['SELL'].keys():
                if s_key not in dict_open['SELL'].keys():
                    s_price_open = dict_hold['SELL'][s_key][2]
                    s_price_close_result = w.wsd(s_key, "close", day, day, "")
                    s_price_close = s_price_close_result.Data[0][0]

                    if s_price_close is None:
                        s_price_close = s_price_open

                    profit += (s_price_open - s_price_close) * dict_hold['SELL'][s_key][1]
                    dict_hold['SELL'].pop(s_key)

            total_profit += profit

            for b_key in dict_open['BUY'].keys():
                if b_key not in dict_hold['BUY'].keys():
                    long_result_price = w.wsd(b_key, "close", day, day, "")
                    long_price = long_result_price.Data[0][0]
                    dict_hold['BUY'][b_key] = [day, FIXEDAMOUNT / float(long_price), long_price]

            for s_key in dict_open['SELL'].keys():
                if s_key not in dict_hold['SELL'].keys():
                    short_result_price = w.wsd(s_key, "close", day, day, "")
                    short_price = short_result_price.Data[0][0]
                    dict_hold['SELL'][s_key] = [day, FIXEDAMOUNT / float(short_price), short_price]

        # print dict_hold
        # print total_profit

        list_main = []
        list_main.append(total_profit)
        list_main.insert(0, day)

        with open("./data/profit.csv", "ab+") as csvfile:
            writer = csv.writer(csvfile)
            writer.writerow(list_main)

