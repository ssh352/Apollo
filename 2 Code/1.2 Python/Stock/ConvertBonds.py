#!/usr/bin/env python  
# encoding: utf-8  

""" 
@version: v1.0 
@author: xiaxianba 
@license: Apache Licence  
@contact: scuxia@gmail.com 
@site: http://weibo.com/xiaxianba 
@software: PyCharm 
@file: ConvertBonds.py 
@time: 2018/5/21 9:19 
@describe: 
        1.收集每天可转债的交易列表；
        2.计算可转债的均价和方差，从这两个维度观察可转债的变化。
"""

import csv
import numpy
from WindPy import *

dict_convertbonds = {'WIND':'a101020600000000', 'CIRC':'1000019571000000', 'SFC':'1000021892000000'}


# 获取所有可转债列表
def get_convert_bonds(date):

    length_list = 0
    list_bonds = []
    list_subbonds = []

    prefix = "date="
    suffix = ";sectorid="

    for value in dict_convertbonds.values():
        scope = prefix + date + suffix + value
        result_s = w.wset("sectorconstituent", scope)
        list_bonds += result_s.Data[1]
        list_bonds = list(set(list_bonds))

    for bond in list_bonds:
        result_c = w.wsd(bond, "close", date, date, "")

        if result_c.Data[0][0] is not None:
            list_subbonds.append(bond)

    length_list = len(list_subbonds)
    return list_subbonds, length_list


# 获取所有可转债的价格
def get_bonds_price(date):
    total_bonds = 0
    mean_bonds = 0.0
    median_bonds = 0.0
    std_bonds = 0.0
    list_bonds = []
    list_bondsprice = []
    list_csv = []

    list_bonds, total_bonds = get_convert_bonds(date)

    for key in list_bonds:
        result = w.wsd(key, "close", date, date, "")
        list_bondsprice += result.Data[0]

    if total_bonds == len(list_bondsprice):
        narray = numpy.array(list_bondsprice)
        mean_bonds = narray.mean()
        std_bonds = narray.std()
        median_bonds = numpy.median(narray)


    list_csv.append(date)
    list_csv.append(mean_bonds)
    list_csv.append(median_bonds)
    list_csv.append(std_bonds)
    write_to_file(list_csv)

    return mean_bonds, std_bonds


# 将数据写入csv文件
def write_to_file(list):

    with open("convertbonds.csv", "ab+") as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(list)

if __name__ == "__main__":

    BEGIN_DATE = "2018-05-18"
    END_DATE = "2018-5-23"

    w.start()
    list_trade_day = w.tdays(BEGIN_DATE, END_DATE, "")

    for day in list_trade_day.Data[0]:
        str_date = day.strftime('%Y-%m-%d')
        get_bonds_price(str_date)

