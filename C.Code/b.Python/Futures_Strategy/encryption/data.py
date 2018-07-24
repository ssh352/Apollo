#!/usr/bin/env python  
# encoding: utf-8  

""" 
@version: v1.0 
@author: xiaxianba 
@license: Apache Licence  
@contact: scuxia@gmail.com 
@site: http://weibo.com/xiaxianba 
@software: PyCharm 
@file: data_inventory.py 
@time: 2018/6/8 14:39 
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

str_startdate = "2008-01-01"
str_enddate = "2018-06-08"
str_path = "./data/"


def get_data():

    w.start()

    result_days = w.tdays(str_startdate, str_enddate, "Days=Alldays")
    list_alldays = result_days.Times

    for key in dict_inventory.keys():

        result = w.edb(dict_inventory[key], str_startdate, str_enddate, "Fill=Previous")
        if result.ErrorCode == 0:
            df = pd.DataFrame(result.Data, columns=result.Times, index=result.Codes).T

            for day in list_alldays:
                if day not in df.index:
                    for i in range(1, 10):
                        result_off = w.tdaysoffset(-i, day, "Days=Alldays")
                        if result_off.Times[0] in df.index and result_off.Times[0] > df.index[0]:
                            df.ix[day] = df.ix[result_off.Times[0]]
                            break

            df.sort_index(inplace=True)
            df.to_excel(str_path + key + '.xlsx', index=True)

