#!/usr/bin/env python  
# encoding: utf-8  

""" 
@version: v1.0 
@author: xiaxianba 
@license: Apache Licence  
@contact: scuxia@gmail.com 
@site: http://weibo.com/xiaxianba 
@software: PyCharm 
@file: frame_mocktrade.py 
@time: 2018/6/7 9:11 
@describe: 
"""

from WindPy import *
from datetime import datetime
import csv
import json
import os

STRATEGYCODE = 'A'
STRATEGYPERIOD = '1W'
DEFAULTPATH = './record/'
FIXEDAMOUNT = 10000
S_BASICUNIT = 100


# 从上证50中获取市值排名的股票
def get_marketvalue_rank(date):

    int_fixed_amount = 10000
    dict_return = {}
    dict_return['BUY'] = {}
    dict_return['SELL'] = {}
    dict_stock = {}

    prefix = "date="
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

    long_dict_stock = sorted(dict_stock.items(), key=lambda item: item[1], reverse=True)[6:10]
    short_dict_stock = sorted(dict_stock.items(), key=lambda item: item[1], reverse=True)[-10:-6]

    for long_dict_key in dict(long_dict_stock).keys():
        long_result_price = w.wsd(long_dict_key, "close", date, date, "")
        long_price = long_result_price.Data[0][0]
        dict_return['BUY'][long_dict_key] = [date, int(FIXEDAMOUNT/(S_BASICUNIT*long_price)), long_price]

    for short_dict_key in dict(short_dict_stock).keys():
        short_result_price = w.wsd(short_dict_key, "close", date, date, "")
        short_price = short_result_price.Data[0][0]
        dict_return['SELL'][short_dict_key] = [date, int(FIXEDAMOUNT/(S_BASICUNIT*short_price)), short_price]

    # json_filename = DEFAULTPATH + get_filename(date) + '.json'
    #
    # # 将开仓字典写入JSON文件
    # with open(json_filename, "w") as jsonfile:
    #     jsonfile.write(json.dumps(dict_return))

    # 从JSON文件中获取开仓字典
    # with open("open.json",'r') as jsonfile:
    #     dict_open = json.load(jsonfile)
    #     print dict_open

    return dict_return


# 将字典写入策略文件
def write_json(date, dict_json):

    json_filename = DEFAULTPATH + get_filename(date) + '.json'

    # 将开仓字典写入JSON文件
    with open(json_filename, "w") as jsonfile:
        jsonfile.write(json.dumps(dict_json))


# 获取文件名
def get_filename(str_date):

    date = datetime.strptime(str_date, '%Y-%m-%d')
    str_date = date.strftime('%Y%m%d')

    filename = STRATEGYCODE + '_' + str_date + '_' + STRATEGYPERIOD

    return filename


# 判断是否存在历史策略文件
def get_history(path):

    b_flag = False

    file_list = os.listdir(path)

    for file in file_list:
        if os.path.splitext(file)[1] == '.json' and os.path.splitext(file)[0].split('_')[0] == STRATEGYCODE:
                b_flag = True

    return b_flag


# 获取最近的策略文件
def get_recent_file(path):

    recent_date = ''
    file_name = ''
    file_list = os.listdir(path)

    for file in file_list:
        if os.path.splitext(file)[1] == '.json' and os.path.splitext(file)[0].split('_')[0] == STRATEGYCODE:
            if recent_date == '':
                recent_date = datetime.strptime(os.path.splitext(file)[0].split('_')[1], '%Y%m%d')
            else:
                compare_date = datetime.strptime(os.path.splitext(file)[0].split('_')[1], '%Y%m%d')
                if compare_date > recent_date:
                    recent_date = compare_date

    file_name = STRATEGYCODE + '_' + recent_date.strftime('%Y%m%d') + '_' + STRATEGYPERIOD + '.json'

    return file_name


# 获取策略文件的日期偏差
def get_period(str_period):

    int_period = 0

    if str_period.strip() == '':
        return int_period
    else:
        unit = str_period[-1:]
        count = str_period[:-1]

    if unit == 'D':
        int_period = int(count)
    elif unit == 'W':
        int_period = 5*int(count)
    elif unit == 'M':
        int_period = 22*int(count)

    return int_period


# 获取策略的总盈利
def get_total_profit():

    total_profit = 0.0

    csv_filename = STRATEGYCODE + '_' + 'profit.csv'

    if os.path.exists(DEFAULTPATH + csv_filename):
        with open(DEFAULTPATH + csv_filename, 'rb') as csvfile:
            profit_lines = csvfile.readlines()

        total_profit = profit_lines[-1].split(',')[2]

    return total_profit

if __name__ == "__main__":

    dict_init = {}

    w.start()

    date_today = datetime.today()
    str_today = date_today.strftime('%Y-%m-%d')

    dict_init = get_marketvalue_rank(str_today)

    if not get_history(DEFAULTPATH):
        write_json(str_today, dict_init)
    else:
        recent_file = get_recent_file(DEFAULTPATH)
        str_recent = os.path.splitext(recent_file)[0].split('_')[1]
        str_period = os.path.splitext(recent_file)[0].split('_')[2]

        date_recent = datetime.strptime(str_recent, '%Y%m%d')
        int_period = get_period(str_period)

        while(w.tdaysoffset(int_period, date_recent, "").Data[0][0] < date_today):

            profit = 0.0
            total_profit = 0.0
            list_profit = []
            dict_hold = {}
            dict_open = {}

            recent_file = get_recent_file(DEFAULTPATH)
            with open(DEFAULTPATH + recent_file, 'r') as jsonfile:
                dict_hold = json.load(jsonfile)

            date_adjust = w.tdaysoffset(int_period, date_recent, "").Data[0][0]
            str_date_adjust = date_adjust.strftime('%Y-%m-%d')
            dict_open = get_marketvalue_rank(str_date_adjust)

            for b_key in dict_hold['BUY'].keys():
                if b_key not in dict_open['BUY'].keys():
                    b_price_open = dict_hold['BUY'][b_key][2]
                    b_price_close_result = w.wsd(b_key, "close", date_adjust, date_adjust, "")
                    b_price_close = b_price_close_result.Data[0][0]

                    profit += (b_price_close - b_price_open)*dict_hold['BUY'][b_key][1] * S_BASICUNIT
                    dict_hold['BUY'].pop(b_key)
                    print "BUY:", str_date_adjust, b_key

            for s_key in dict_hold['SELL'].keys():
                if s_key not in dict_open['SELL'].keys():
                    s_price_open = dict_hold['SELL'][s_key][2]
                    s_price_close_result = w.wsd(s_key, "close", date_adjust, date_adjust, "")
                    s_price_close = s_price_close_result.Data[0][0]

                    profit += (s_price_open - s_price_close) * dict_hold['SELL'][s_key][1] * S_BASICUNIT
                    dict_hold['SELL'].pop(s_key)
                    print "SELL:", str_date_adjust, s_key

            total_profit = float(get_total_profit()) + profit

            list_profit.append(date_adjust)
            list_profit.append(profit)
            list_profit.append(total_profit)

            with open(DEFAULTPATH + STRATEGYCODE + '_' + 'profit.csv', "ab+") as csvfile:
                writer = csv.writer(csvfile)
                writer.writerow(list_profit)

            for b_key in dict_open['BUY'].keys():
                if b_key not in dict_hold['BUY'].keys():
                    long_result_price = w.wsd(b_key, "close", str_date_adjust, str_date_adjust, "")
                    long_price = long_result_price.Data[0][0]
                    dict_hold['BUY'][b_key] = [str_date_adjust, int(FIXEDAMOUNT / (S_BASICUNIT * long_price)), long_price]

            for s_key in dict_open['SELL'].keys():
                if s_key not in dict_hold['SELL'].keys():
                    short_result_price = w.wsd(s_key, "close", str_date_adjust, str_date_adjust, "")
                    short_price = short_result_price.Data[0][0]
                    dict_hold['SELL'][s_key] = [str_date_adjust, int(FIXEDAMOUNT / (S_BASICUNIT * short_price)), short_price]

            write_json(str_date_adjust, dict_hold)
            date_recent = date_adjust




