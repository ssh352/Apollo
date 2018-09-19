#!/usr/bin/env python  
# encoding: utf-8  

""" 
@version: v1.0 
@author: xiaxianba 
@license: Apache Licence  
@contact: scuxia@gmail.com 
@site: http://weibo.com/xiaxianba 
@software: PyCharm 
@file: backtest.py 
@time: 2018/9/19 17:17 
@describe: 
"""

import csv
import datetime as pydt
import numpy as np
import os
from WindPy import *

import smtplib
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from email.header import Header

from 

BEGIN_DATE = "2017-04-06"  # 模拟开始日期
END_DATE = "2018-05-18"    # 模拟截止日期

HOLD_PERIOD = 5            # 换仓期
SORT_PERIOD = 20           # 排序期
BASE_VOLUME = 100000.0     # 交易量过滤条件
FIXED_VALUE = 300000.0     # 等货值交易

# 国内三大期货交易所共46个商品合约
list_item = ['CU.SHF', 'AL.SHF', 'ZN.SHF', 'PB.SHF', 'AU.SHF', 'AG.SHF', 'NI.SHF', 'SN.SHF',
             'RB.SHF', 'WR.SHF', 'HC.SHF', 'BU.SHF', 'RU.SHF', 'M.DCE', 'Y.DCE',
             'A.DCE', 'B.DCE', 'P.DCE', 'C.DCE', 'J.DCE', 'V.DCE', 'I.DCE',
             'BB.DCE', 'FB.DCE', 'L.DCE',  'PP.DCE',  'JM.DCE', 'CS.DCE', 'CY.CZC',
             'SR.CZC', 'CF.CZC', 'ZC.CZC', 'FG.CZC', 'TA.CZC', 'MA.CZC', 'WH.CZC', 'PM.CZC',
             'RI.CZC', 'LR.CZC', 'JR.CZC', 'RS.CZC', 'OI.CZC', 'RM.CZC', 'SF.CZC', 'SM.CZC', ]

# 商品合约对应的Wind板块id
dict_item = {'CU.SHF':'a599010202000000', 'AL.SHF':'a599010203000000', 'ZN.SHF':'a599010204000000',
             'PB.SHF':'1000002892000000', 'AU.SHF':'a599010205000000', 'AG.SHF':'1000006502000000',
             'NI.SHF':'1000011457000000', 'SN.SHF':'1000011458000000', 'RB.SHF':'a599010206000000',
             'WR.SHF':'a599010207000000', 'HC.SHF':'1000011455000000',
             'BU.SHF':'1000011013000000', 'RU.SHF':'a599010208000000', 'M.DCE':'a599010304000000',
             'Y.DCE':'a599010306000000', 'A.DCE':'a599010302000000', 'B.DCE':'a599010303000000',
             'P.DCE':'a599010307000000', 'C.DCE':'a599010305000000', 'J.DCE':'1000002976000000',
             'V.DCE':'a599010309000000', 'I.DCE':'1000011439000000', 'BB.DCE':'1000011466000000',
             'FB.DCE':'1000011465000000', 'L.DCE':'a599010308000000',  'PP.DCE':'1000011468000000',
             'JM.DCE':'1000009338000000', 'CS.DCE':'1000011469000000',
             'CY.CZC':'1000011479000000', 'SR.CZC':'a599010405000000', 'CF.CZC':'a599010404000000',
             'ZC.CZC':'1000011012000000', 'FG.CZC':'1000008549000000', 'TA.CZC':'a599010407000000',
             'MA.CZC':'1000005981000000', 'WH.CZC':'a599010403000000', 'PM.CZC':'1000006567000000',
             'RI.CZC':'a599010406000000', 'LR.CZC':'1000011476000000', 'JR.CZC':'1000011474000000',
             'RS.CZC':'1000008621000000', 'OI.CZC':'a599010408000000', 'RM.CZC':'1000008622000000',
             'SF.CZC':'1000011478000000', 'SM.CZC':'1000011477000000'}

# 函数入口
if __name__ == "__main__":

    total_profit = 0.0
    dict_open = {}
    dict_close = {}
    dict_hold = {}

    w.start()

    list_day = get_trade_day(BEGIN_DATE, END_DATE)
    list_step_day = list_day[::20]



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

    short_momlist = sorted(dict_item.items(), key=lambda item: item[1])[:len(dict_item) // 2]
    long_momlist = sorted(dict_item.items(), key=lambda item: item[1])[len(dict_item) // 2:]

    return long_momlist, short_momlist


# 根据展期收益率排名获取多空组合
def get_extension_list(ext_list, flag):

    dict_rate = {}

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
                main_contract_price = w.wsi(main_contract, "close", date_now + " 11:00:00",
                               date_now + " 11:30:00", "BarSize=30")
                main_contract_delivery = w.wsd(main_contract, "lastdelivery_date", date_now, date_now, "")

                dicts.pop(main_contract)

                second_contract = sorted(dicts.items(), key=lambda item: item[1], reverse=True)[:1]
                second_contract_price = w.wsi(dict(second_contract).keys(), "close", date_now + " 11:00:00",
                                            date_now + " 11:30:00", "BarSize=30")
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


# 获取交易列表
def get_trade_list(list_long,list_short):

    list_main = []
    dict_tradelist = {}
    dict_tradelist['BUY'] = {}
    dict_tradelist['SELL'] = {}

    date_base = pydt.date.today().strftime('%Y-%m-%d')
    result = w.tdaysoffset(-1, date_base, "")
    date_now = result.Data[0][0].strftime('%Y-%m-%d')

    for long in list_long:
        result_main = w.wsd(long, "trade_hiscode", date_now, date_now, "")
        result_price = w.wsq(long, "rt_last")

        if result_main.ErrorCode == 0 and result_price.ErrorCode == 0:
            result_multiplier = w.wsd(result_main.Data[0][0], "contractmultiplier", date_now, date_now, "")
            int_position = np.floor(FIXEDVALUE / (result_price.Data[0][0] * result_multiplier.Data[0][0]))

            dict_tradelist['BUY'][result_main.Data[0][0]] = int_position
        else:
            continue

    for short in list_short:
        result_main = w.wsd(short, "trade_hiscode", date_now, date_now, "")
        result_price = w.wsq(short, "rt_last")

        if result_main.ErrorCode == 0 and result_price.ErrorCode == 0:
            result_multiplier = w.wsd(result_main.Data[0][0], "contractmultiplier", date_now, date_now, "")
            int_position = np.floor(FIXEDVALUE / (result_price.Data[0][0] * result_multiplier.Data[0][0]))

            dict_tradelist['SELL'][result_main.Data[0][0]] = int_position
        else:
            continue

    print "当期交易列表：", dict_tradelist
    return dict_tradelist


# 获取持仓列表
def get_hold_list():

    dict_hold = {}
    dict_hold['BUY'] = {}
    dict_hold['SELL'] = {}

    result_login = w.tlogon('0000', 0, 'W100435100502', 'hx123456', 'SHF')
    # result_login = w.tlogon('29990401', "", '105545', 'hexin123', 'SHF')
    if result_login.ErrorCode == 0:
        result_position = w.tquery('Position', 'LoginID=1')
        len_contracts = len(result_position.Data[0])

        if result_position.ErrorCode == 0 and len(result_position.Fields) > 3:
            for contracts in range(len_contracts):
                if result_position.Data[4][contracts] == 'Buy':
                    dict_hold['BUY'][result_position.Data[0][contracts].upper()] = result_position.Data[6][contracts]
                elif result_position.Data[4][contracts] == 'Short':
                    dict_hold['SELL'][result_position.Data[0][contracts].upper()] = result_position.Data[6][contracts]

    print "持仓列表：", dict_hold
    return dict_hold


# 平仓
def exe_close(dict_trade, dict_hold):

    dict_close = {}
    dict_close['BUY'] = {}
    dict_close['SELL'] = {}

    for key in dict_trade.keys():
        for contracts in dict_hold[key].keys():
            if contracts not in dict_trade[key].keys():
                if key == 'BUY':
                    result_price = w.wsq(contracts, "rt_last")
                    w.torder(contracts, 'Sell', result_price.Data[0][0], dict_hold[key][contracts],
                             'OrderType=LMT;LogonID=1')
                elif key == 'SELL':
                    result_price = w.wsq(contracts, "rt_last")
                    w.torder(contracts, 'Cover', result_price.Data[0][0], dict_hold[key][contracts],
                             'OrderType=LMT;LogonID=1')

                dict_close[key][contracts] = dict_hold[key][contracts]

    print "平仓列表：", dict_close


# 开仓
def exe_open(dict_trade, dict_hold):

    dict_open = {}
    dict_open['BUY'] = {}
    dict_open['SELL'] = {}

    for key in dict_trade.keys():
        for contracts in dict_trade[key].keys():
            if contracts not in dict_hold[key].keys():
                if key == 'BUY':
                    result_price = w.wsq(contracts, "rt_last")
                    w.torder(contracts, 'Buy', result_price.Data[0][0], dict_trade[key][contracts],
                             'OrderType=LMT;LogonID=1')
                elif key == 'SELL':
                    result_price = w.wsq(contracts, "rt_last")
                    w.torder(contracts, 'Short', result_price.Data[0][0], dict_trade[key][contracts],
                             'OrderType=LMT;LogonID=1')

                dict_open[key][contracts] = dict_trade[key][contracts]

    print "开仓列表：", dict_open


# 输出交易品种记录
def get_trade_item(trade_list, flag):

    list_main = []
    date_base = pydt.date.today().strftime('%Y-%m-%d')
    result = w.tdaysoffset(-1, date_base, "")
    date_now = result.Data[0][0].strftime('%Y-%m-%d')

    for trade in trade_list:
        result_main = w.wsd(trade, "trade_hiscode", date_now, date_now, "")
        if result_main.ErrorCode == 0:
            list_main.append(result_main.Data[0][0])
        else:
            continue

    for i in range(len(list_main)):
        if list_main[i].endswith('.SHF') or list_main[i].endswith('.DCE'):
            list_main[i] = list_main[i].lower()

    list_main.insert(0, date_now)

    if flag == 1:
        if os.path.exists("long.csv"):
            with open("long.csv", "r+") as csvfile:
                lines = csv.reader(csvfile)
                for line in lines:
                    date_pre = line[0]

            date_now_t = datetime.strptime(date_now, "%Y-%m-%d")
            date_pre_t = datetime.strptime(date_pre, "%Y-%m-%d")

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


# 输出具体数值到txt文件
def write_ext_txt(long_item, short_item):
    list_main_long = []
    list_main_short = []
    date_base = pydt.date.today().strftime('%Y-%m-%d')
    result = w.tdaysoffset(-1, date_base, "")
    date_now = result.Data[0][0].strftime('%Y-%m-%d')

    list_main_long.insert(0, date_base)
    list_main_short.insert(0, date_base)

    if os.path.exists("long_detail.txt"):
        os.remove("long_detail.txt")

    list_main_long.append(long_item)
    with open("long_detail.txt", "ab+") as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(list_main_long)

    if os.path.exists("short_detail.txt"):
        os.remove("short_detail.txt")

    list_main_short.append(short_item)

    with open("short_detail.txt", "ab+") as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(list_main_short)


# 发送邮件
def send_csv_mail():

    mail_host = "smtp.163.com"
    mail_user = "scuxia"
    mail_pass = "mail163"

    sender = 'scuxia@163.com'
    receivers = ['173795499@qq.com']
    # receivers = ['173795499@qq.com', '3263548205@qq.com', '215112048@qq.com']

    message = MIMEMultipart()
    message['From'] = sender
    message['To'] = Header("动量-展期策略", 'utf-8')
    subject = '动量-展期策略交易品种'
    message['Subject'] = Header(subject, 'utf-8')

    now = datetime.now()
    str_date = now.strftime('%Y-%m-%d')
    message.attach(MIMEText(str_date+' 动量-展期交易品种如下:', 'plain', 'utf-8'))

    att1 = MIMEText(open('long.csv', 'rb').read(), 'base64', 'utf-8')
    att1["Content-Type"] = 'application/octet-stream'
    att1["Content-Disposition"] = 'attachment; filename="long.csv"'
    message.attach(att1)

    att2 = MIMEText(open('short.csv', 'rb').read(), 'base64', 'utf-8')
    att2["Content-Type"] = 'application/octet-stream'
    att2["Content-Disposition"] = 'attachment; filename="short.csv"'
    message.attach(att2)

    att3 = MIMEText(open('long_detail.txt', 'rb').read(), 'base64', 'utf-8')
    att3["Content-Type"] = 'application/octet-stream'
    att3["Content-Disposition"] = 'attachment; filename="long_detail.txt"'
    message.attach(att3)

    att4 = MIMEText(open('short_detail.txt', 'rb').read(), 'base64', 'utf-8')
    att4["Content-Type"] = 'application/octet-stream'
    att4["Content-Disposition"] = 'attachment; filename="short_detail.txt"'
    message.attach(att4)

    try:
        smtpObj = smtplib.SMTP()
        smtpObj.connect(mail_host, 25)  # 25 为 SMTP 端口号
        smtpObj.login(mail_user, mail_pass)
        smtpObj.sendmail(sender, receivers, message.as_string())
        print "邮件发送成功"
    except smtplib.SMTPException, e:
        print str(e)
        print "Error: 无法发送邮件"

if __name__ == "__main__":

    w.start()

    list_valid = get_valid_list()
    long_list, short_list = get_momentum_list(list_valid)
    long_key = dict(long_list).keys()
    short_key = dict(short_list).keys()
    long_item = get_extension_list(long_key, 1)
    short_item = get_extension_list(short_key, 0)

    long_futures = dict(long_item).keys()
    short_futures = dict(short_item).keys()

    dict_trade = get_trade_list(long_futures, short_futures)
    dict_hold = get_hold_list()

    exe_close(dict_trade, dict_hold)
    exe_open(dict_trade, dict_hold)

    write_ext_txt(long_item, short_item)
    get_trade_item(dict(long_item).keys(), 1)
    get_trade_item(dict(short_item).keys(), 0)

    send_csv_mail()

