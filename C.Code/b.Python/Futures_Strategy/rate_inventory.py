#!/usr/bin/env python  
# encoding: utf-8  

""" 
@version: v1.0 
@author: xiaxianba 
@license: Apache Licence  
@contact: scuxia@gmail.com 
@site: http://weibo.com/xiaxianba 
@software: PyCharm 
@file: rate_inventory.py 
@time: 2018/6/27 10:53 
@describe: 
"""

from WindPy import *

import csv
import pandas as pd

# dict_inventory = {'CU': ['S0049493', 'S0029752' 'S0114144'], 'AL': ['S0049494', 'S0029756'], 'PB': ['S0167708', 'S0029764'],
#                   'ZN': ['S0049498', 'S0029760'], 'NI': ['S0213018', 'S0029772'], 'SN': ['S0213019', 'S0029768'],
#                   'AU': ['S0049505', 'S0114145'], 'AG': ['S0174908', 'S0114146'], 'RU': ['S0049495'],
#                   'RB': ['S0181750'], 'HC': ['S0110143'], 'BU': ['S0204821'], 'A': ['S5041136', 'S0117164'],
#                   'M': ['S5041144'], 'Y': ['S5063139', 'S5028184'], 'P': ['S5006381', 'S5029336'],
#                   'C': ['S0049491', 'S5063274', 'S5063275'], 'CS': ['S0243229'], 'L': ['S0049492', 'S5446196'],
#                   'PP': ['S0205112', 'S5471283'], 'PVC': ['S0180500', 'S5471265'], 'JM': ['S5125686', 'S5133418'],
#                   'J': ['S5120058', 'S5116629', 'S5116630', 'S5136709', 'S5133852'], 'I': ['S0110152'],
#                   'CF': ['S0049502', 'S0049517'], 'SR': ['S0049502', 'S0049517'], 'OI': ['S5041913', 'S0164192'],
#                   'RM': ['S0182089'], 'ZC': ['S5116621', 'S5118163', 'S5103725', 'S5103728'],
#                   'MA': ['S5436526', 'S5436527'], 'PTA': ['S0049499'], 'FG': ['S5914176']}

dict_inventory = {'A': ['S5041136', 'S0117164']}

dict_multiplier = {'CU': [1.0, 1.0, 0.9072], 'AL': [1.0, 1.0], 'PB': [1.0, 1.0],
                  'ZN': [1.0, 1.0], 'NI': [1.0, 1.0], 'SN': [1.0, 1.0],
                  'AU': [1000.0, 31.1035], 'AG': [1000.0, 31.1035], 'RU': [1.0],
                  'RB': [1.0], 'HC': [1.0], 'BU': [1.0], 'A': [10000.0, 1.0],
                  'M': [1.0], 'Y': [1.0, 1.0], 'P': [1.0, 1.0],
                  'C': [10.0, 10000.0, 10000.0], 'CS': [1.0], 'L': [5.0, 1.0],
                  'PP': [5.0, 1.0], 'PVC': [5.0, 1.0], 'JM': [1.0, 1.0],
                  'J': [1.0, 1.0, 1.0, 1.0, 1.0], 'I': [1.0],
                  'CF': [1.0, 1.0], 'SR': [1.0, 1.0], 'OI': [10.0, 1.0],
                  'RM': [1.0], 'ZC': [10000.0, 10000.0, 10000.0, 1.0],
                  'MA': [1.0, 1.0], 'PTA': [1.0], 'FG': [1.0]}

str_startdate = "2009-01-01"
str_enddate = "2018-06-08"
str_path = "./data/"


# 获取日历日列表
def get_calendar_day(begin_date, end_date):

    list_calendar_day = []
    result_calendar_day = w.tdays(begin_date, end_date, "Days=Alldays")

    for day in result_calendar_day.Data[0]:
        str_date = day.strftime('%Y-%m-%d')
        list_calendar_day.append(str_date)

    return list_calendar_day


# 获取同比、环比数据
def get_ratio(variety, date):

    list_main =[]

    date_today = date
    current_value = 0.0
    previous_value = 0.0
    # date_month_ago = date_today - pd.tseries.offsets.DateOffset(months=1)
    date_year_ago = date_today - pd.tseries.offsets.DateOffset(months=12)

    pd_variety = pd.read_excel("./data/" + variety + ".xlsx")

    if date_today in pd_variety.index and date_year_ago in pd_variety.index:
        for col in range(len(pd_variety.columns)):
            if not pd.isnull(pd_variety.ix[date_today, pd_variety.columns[col]]) and \
                    not pd.isnull(pd_variety.ix[date_year_ago, pd_variety.columns[col]]):
                current_value += float(pd_variety.ix[date_today, pd_variety.columns[col]])*dict_multiplier[variety][col]
                previous_value += float(pd_variety.ix[date_year_ago, pd_variety.columns[col]])*dict_multiplier[variety][col]

        if current_value != 0 and previous_value != 0:
            same_ratio = float(current_value / previous_value) - 1.0
        else:
            same_ratio = ''
    else:
        same_ratio = ''

    # print pd_variety.columns[0]
    # print variety+"row: ", len(pd_variety)
    # print variety+"columns: ", len(pd_variety.columns)
    # print variety, pd_variety.iloc[:, [0]]

    # if date_today in pd_variety.index and date_year_ago in pd_variety.index and\
    #                 pd_variety.loc[date_today].sum() != 0 and pd_variety.loc[date_year_ago].sum() != 0 and\
    #         not pd_variety.loc[date_today].isnull().any():
    #     same_ratio = float(pd_variety.loc[date_today].sum()) / pd_variety.loc[date_year_ago].sum() - 1
    # else:
    #     same_ratio = ''

    list_main.append(same_ratio)
    list_main.insert(0, date)
    with open("./data/" + variety + ".csv", "ab+") as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(list_main)

if __name__ == "__main__":

    w.start()
    list_day = get_calendar_day(str_startdate, str_enddate)

    for variety in dict_inventory.keys():
        for day in list_day:
            date_day = datetime.strptime(day, "%Y-%m-%d")
            get_ratio(variety, date_day)
