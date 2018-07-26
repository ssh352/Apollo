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

DICT_CODES = {"000001.SZ": [[], []], "000002.SZ": [[], []], "600000.SH": [[], []], "600016.SH": [[], []],
              "600028.SH": [[], []], "600029.SH": [[], []], "600030.SH": [[], []], "600036.SH": [[], []],
              "600050.SH": [[], []], "600104.SH": [[], []], "600111.SH": [[], []], "600276.SH": [[], []],
              "600340.SH": [[], []], "600519.SH": [[], []], "600547.SH": [[], []], "600585.SH": [[], []],
              "300490.SZ": [[], []], "603520.SH": [[], []], "300357.SZ": [[], []], "600756.SH": [[], []],
              "600306.SH": [[], []], "600137.SH": [[], []], "300022.SZ": [[], []], "603022.SH": [[], []],
              "000488.SZ": [[], []], "601108.SH": [[], []], "603605.SH": [[], []], "603711.SH": [[], []],
              "300666.SZ": [[], []], "002241.SZ": [[], []], "600460.SH": [[], []], "600516.SH": [[], []],}


def DemoWSQCallback(result):

    last_price = 0.0
    last_last_vol = 0.0

    if result.ErrorCode != 0:
        print 'error code:'+str(result.ErrorCode)
        return

    for code in range(len(result.Codes)):
        try:
            for key in range(len(result.Fields)):
                if result.Fields[key] == 'RT_LAST':  # 当前的价格
                    last_price = result.Data[key][code]
                    DICT_CODES[result.Codes[code]][0].append(last_price)
                    # print "##########", last_price
                elif result.Fields[key] == 'RT_LAST_VOL':  # 当前一笔的成交量
                    last_last_vol = result.Data[key][code]
                    DICT_CODES[result.Codes[code]][1].append(last_last_vol)
                    # print "******", last_last_vol
                else:
                    continue
            # print DICT_CODES
            # print result.Codes[code], DICT_CODES[result.Codes[code]][0][-1], DICT_CODES[result.Codes[code]][1][-1]
            if len(DICT_CODES[result.Codes[code]][0])>10 and len(DICT_CODES[result.Codes[code]][1])>60 and \
                            DICT_CODES[result.Codes[code]][0][-1] > sum(DICT_CODES[result.Codes[code]][0][-11:-1]) and \
                            DICT_CODES[result.Codes[code]][1][-1] >= max(DICT_CODES[result.Codes[code]][1][-61:-1]):
                print ">>>>", result.Codes[code], DICT_CODES[result.Codes[code]][0][-1], DICT_CODES[result.Codes[code]][1][-1]

        # except Exception as ex:
            # print "EXPECT", ex
        except Exception as Error:
            pass

    # for key in range(len(result.Fields)):
    #     if result.Fields[key] == 'RT_AMT':  # 当前交易日的总成交额
    #         amount = result.Data[key][0]
    #     elif result.Fields[key] == 'RT_LAST':  # 当前的价格
    #         last_price = result.Data[key][0]
    #         # list_price.append(last_price)
    #     elif result.Fields[key] == 'RT_LAST_VOL':  # 当前一笔的成交量
    #         last_last_vol = result.Data[key][0]
    #         # list_vol.append(last_last_vol)
    #     elif result.Fields[key] == 'RT_VOL':  # 当前交易日的总成交量
    #         last_vol = result.Data[key][0]
    #     else:
    #         pass
    #
    #     print result
    #     for code in result.Codes:
    #         DICT_CODES[code][0].append(result.Data['RT_LAST_VOL'][0])
    #         DICT_CODES[code][1].append(last_price)
    #
    #         if DICT_CODES[code][0][-1] > sum(DICT_CODES[code][0][-11:-1]) and \
    #                      DICT_CODES[code][1][-1] >= max(DICT_CODES[code][1][-61:-1]):
    #             print "##", code, DICT_CODES[code][0][-1], DICT_CODES[code][1][-1]

    # print result
    # print 'amount=', amount
    # print 'last_price=', last_price
    # print 'last_last_vol=', last_last_vol
    # print 'last_vol=', last_vol

if __name__ == "__main__":

    w.start()
    w.wsq(DICT_CODES.keys(), "rt_amt,rt_time,rt_last,rt_last_vol,rt_vol", func=DemoWSQCallback)

    while True:
        time.sleep(0.1)




