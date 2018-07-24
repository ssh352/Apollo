#!/usr/bin/env python
# encoding: utf-8

""" 
@version: v1.0 
@author: xiaxianba 
@license: Apache Licence  
@contact: scuxia@gmail.com 
@site: http://weibo.com/xiaxianba 
@software: PyCharm 
@file: TestData.py 
@time: 2018/5/22 11:17 
@describe: 
"""
from WindPy import *
import time

DICT_CODES = {"000001.SZ": [[], []], "000002.SZ": [[], []]}


def DemoWSQCallback(result):
    amount = 0.0
    last_price = 0.0
    last_last_vol = 0.0
    last_vol = 0.0

    if result.ErrorCode != 0:
        print 'error code:'+str(result.ErrorCode)
        return

    for key in range(len(result.Fields)):
        if result.Fields[key] == 'RT_AMT':  # 当前交易日的总成交额
            amount = result.Data[key][0]
        elif result.Fields[key] == 'RT_LAST':  # 当前的价格
            last_price = result.Data[key][0]
        elif result.Fields[key] == 'RT_LAST_VOL':  # 当前一笔的成交量
            last_last_vol = result.Data[key][0]
        elif result.Fields[key] == 'RT_VOL':  # 当前交易日的总成交量
            last_vol = result.Data[key][0]
        else:
            pass

    for code in result.Codes:
        DICT_CODES[code][0].append(last_last_vol)
        DICT_CODES[code][1].append(last_price)
        print code, DICT_CODES[code][0][-4:]
        print code, DICT_CODES[code][1][-4:]


    # print 'amount=', amount
    # print 'last_price=', last_price
    # print 'last_last_vol=', last_last_vol
    # print 'last_vol=', last_vol

if __name__ == "__main__":

    w.start()
    w.wsq(DICT_CODES.keys(), "rt_amt,rt_time,rt_last,rt_last_vol,rt_vol", func=DemoWSQCallback)

    while True:
        time.sleep(0.01)