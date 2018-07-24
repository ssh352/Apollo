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

import datetime
import smtplib
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from email.header import Header

HOLDPERIOD = 5
SORTPERIOD = 20
BASEVOLUME = 100000.0

# 国内三大期货交易所共47个商品合约
list_item = ['CU.SHF', 'AL.SHF', 'ZN.SHF', 'PB.SHF', 'AU.SHF', 'AG.SHF', 'NI.SHF', 'SN.SHF',
             'RB.SHF', 'WR.SHF', 'HC.SHF', 'FU.SHF', 'BU.SHF', 'RU.SHF', 'M.DCE', 'Y.DCE',
             'A.DCE', 'B.DCE', 'P.DCE', 'C.DCE', 'J.DCE', 'V.DCE', 'I.DCE',
             'BB.DCE', 'FB.DCE', 'L.DCE', 'PP.DCE', 'JM.DCE', 'CS.DCE', 'CY.CZC', # 'JD.DCE',
             'SR.CZC', 'CF.CZC', 'ZC.CZC', 'FG.CZC', 'TA.CZC', 'MA.CZC', 'WH.CZC', 'PM.CZC',
             'RI.CZC', 'LR.CZC', 'JR.CZC', 'RS.CZC', 'OI.CZC', 'RM.CZC', 'SF.CZC', 'SM.CZC', ]

# 商品合约对应的Wind板块id
dict_item = {'CU.SHF': 'a599010202000000', 'AL.SHF': 'a599010203000000', 'ZN.SHF': 'a599010204000000',
             'PB.SHF': '1000002892000000', 'AU.SHF': 'a599010205000000', 'AG.SHF': '1000006502000000',
             'NI.SHF': '1000011457000000', 'SN.SHF': '1000011458000000', 'RB.SHF': 'a599010206000000',
             'WR.SHF': 'a599010207000000', 'HC.SHF': '1000011455000000', 'FU.SHF': 'a599010209000000',
             'BU.SHF': '1000011013000000', 'RU.SHF': 'a599010208000000', 'M.DCE': 'a599010304000000',
             'Y.DCE': 'a599010306000000', 'A.DCE': 'a599010302000000', 'B.DCE': 'a599010303000000',
             'P.DCE': 'a599010307000000', 'C.DCE': 'a599010305000000', 'J.DCE': '1000002976000000',
             'V.DCE': 'a599010309000000', 'I.DCE': '1000011439000000', 'BB.DCE': '1000011466000000',
             'FB.DCE': '1000011465000000', 'L.DCE': 'a599010308000000', 'PP.DCE': '1000011468000000',
             'JM.DCE': '1000009338000000', 'JD.DCE': '1000011464000000', 'CS.DCE': '1000011469000000',
             'CY.CZC': '1000011479000000', 'SR.CZC': 'a599010405000000', 'CF.CZC': 'a599010404000000',
             'ZC.CZC': '1000011012000000', 'FG.CZC': '1000008549000000', 'TA.CZC': 'a599010407000000',
             'MA.CZC': '1000005981000000', 'WH.CZC': 'a599010403000000', 'PM.CZC': '1000006567000000',
             'RI.CZC': 'a599010406000000', 'LR.CZC': '1000011476000000', 'JR.CZC': '1000011474000000',
             'RS.CZC': '1000008621000000', 'OI.CZC': 'a599010408000000', 'RM.CZC': '1000008622000000',
             'SF.CZC': '1000011478000000', 'SM.CZC': '1000011477000000'}


# 获取符合十万手成交量的商品合约
def get_valid_list():
    list_valid_item = []
    date_now = pydt.date.today() - pydt.timedelta(days=1)

    for item in list_item:
        result = w.wsd(item, "volume", "ED-9TD", date_now, "")

        if result.ErrorCode == 0:
            ma = float(sum(result.Data[0]) / len(result.Data[0]))

            if ma > BASEVOLUME:
                list_valid_item.append(item)

    return list_valid_item


# 根据展期收益率排名获取多空组合
def get_extension_list(ext_list):
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
                second_contract_delivery = w.wsd(dict(second_contract).keys(), "lastdelivery_date", date_now, date_now,
                                                 "")

                diff_price = np.log(float(main_contract_price.Data[0][0])) - np.log(
                    float(second_contract_price.Data[0][0]))
                diff_date = (second_contract_delivery.Data[0][0] - main_contract_delivery.Data[0][0]).days
                dict_rate[item] = 365 * diff_price / diff_date

    long_extlist = sorted(dict_rate.items(), key=lambda item: item[1], reverse=True)[:len(dict_rate) // 5]
    short_extlist = sorted(dict_rate.items(), key=lambda item: item[1])[:len(dict_rate) // 5]

    return long_extlist, short_extlist


# 输出交易品种记录
def get_trade_item(long_list, short_list):

    list_long_main = []
    list_short_main = []
    # date_now = (pydt.date.today() - pydt.timedelta(days=1)).strftime('%Y-%m-%d')
    date_base = pydt.date.today().strftime('%Y-%m-%d')
    result = w.tdaysoffset(-1, date_base, "")
    date_now = result.Data[0][0].strftime('%Y-%m-%d')

    for long_trade in long_list:
        result_long_main = w.wsd(long_trade, "trade_hiscode", date_now, date_now, "")
        list_long_main.append(result_long_main.Data[0][0])

    for short_trade in short_list:
        result_short_main = w.wsd(short_trade, "trade_hiscode", date_now, date_now, "")
        list_short_main.append(result_short_main.Data[0][0])

    for i in range(len(list_long_main)):
        if list_long_main[i].endswith('.SHF') or list_long_main[i].endswith('.DCE'):
            list_long_main[i] = list_long_main[i].lower()

    for i in range(len(list_short_main)):
        if list_short_main[i].endswith('.SHF') or list_short_main[i].endswith('.DCE'):
            list_short_main[i] = list_short_main[i].lower()

    # list_long_main.insert(0, date_now)
    # list_short_main.insert(0, date_now)

    if os.path.exists("long_w.csv") and os.path.exists("short_w.csv"):
        with open("long_w.csv", "r+") as csvfile:
            lines = csv.reader(csvfile)
            for long_line in lines:
                date_long_pre = long_line[0]

        with open("short_w.csv", "r+") as csvfile:
            lines = csv.reader(csvfile)
            for short_line in lines:
                date_long_pre = short_line[0]

        long_line.remove(long_line[0])
        short_line.remove(short_line[0])

        print("long_line=", long_line)
        print("short_line=", short_line)
        print("list_long_main=", list_long_main)
        print("list_short_main=", list_short_main)

        long_list_change = list(set(long_line).difference(set(list_long_main)))
        short_list_change = list(set(short_line).difference(set(list_short_main)))

        print("long_list_change=", long_list_change)
        print("short_list_change=", short_list_change)

        if (float((len(long_list_change)+len(short_list_change))) / float((len(long_line) + len(short_line) - 2))) >= 0.3:
            with open("long_w.csv", "ab+") as csvfile:
                writer = csv.writer(csvfile)
                list_long_main.insert(0, date_base)
                writer.writerow(list_long_main)

            with open("short_w.csv", "ab+") as csvfile:
                writer = csv.writer(csvfile)
                list_short_main.insert(0, date_base)
                writer.writerow(list_short_main)
    else:
        with open("long_w.csv", "ab+") as csvfile:
            writer = csv.writer(csvfile)
            list_long_main.insert(0, date_base)
            writer.writerow(list_long_main)

        with open("short_w.csv", "ab+") as csvfile:
            writer = csv.writer(csvfile)
            list_short_main.insert(0, date_base)
            writer.writerow(list_short_main)


# 发送邮件
def send_csv_mail():

    mail_host = "smtp.163.com"
    mail_user = "scuxia"
    mail_pass = "mail163"

    sender = 'scuxia@163.com'
    # receivers = ['173795499@qq.com']
    receivers = ['173795499@qq.com', '3263548205@qq.com', '215112048@qq.com']  # 接收邮件，可设置为你的QQ邮箱或者其他邮箱

    message = MIMEMultipart()
    message['From'] = sender
    message['To'] = Header("每日换仓（账户-1D展期）展期策略", 'utf-8')
    subject = '每日换仓（账户-1D展期）展期策略交易品种'
    message['Subject'] = Header(subject, 'utf-8')

    now = datetime.datetime.now()
    str_date = now.strftime('%Y-%m-%d')
    message.attach(MIMEText(str_date+' 每日换仓（账户-1D展期）展期交易品种如下:', 'plain', 'utf-8'))

    att1 = MIMEText(open('long_w.csv', 'rb').read(), 'base64', 'utf-8')
    att1["Content-Type"] = 'application/octet-stream'
    att1["Content-Disposition"] = 'attachment; filename="long_w.csv"'
    message.attach(att1)

    att2 = MIMEText(open('short_w.csv', 'rb').read(), 'base64', 'utf-8')
    att2["Content-Type"] = 'application/octet-stream'
    att2["Content-Disposition"] = 'attachment; filename="short_w.csv"'
    message.attach(att2)

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

    print "list_valid:", list_valid

    long_item, short_item = get_extension_list(list_valid)

    print "long_item:", long_item
    print "short_item:", short_item

    get_trade_item(dict(long_item).keys(), dict(short_item).keys())

    send_csv_mail()
