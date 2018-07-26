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

DICT_CODES = {  # "": [[], []], "": [[], []], "": [[], []], "": [[], []], "": [[], []],
    "600186.SH": [[], []], "600225.SH": [[], []], "002021.SZ": [[], []], "002676.SZ": [[], []], "002357.SZ": [[], []],
    "000679.SZ": [[], []], "000518.SZ": [[], []], "600187.SH": [[], []], "600821.SH": [[], []], "002469.SZ": [[], []],
    "000955.SZ": [[], []], "000514.SZ": [[], []], "600106.SH": [[], []], "600512.SH": [[], []], "600112.SH": [[], []],
    "300318.SZ": [[], []], "002211.SZ": [[], []], "000548.SZ": [[], []], "002638.SZ": [[], []], "300116.SZ": [[], []],
    "002058.SZ": [[], []], "000812.SZ": [[], []], "300163.SZ": [[], []], "002703.SZ": [[], []], "300428.SZ": [[], []],
    "002899.SZ": [[], []], "002721.SZ": [[], []], "603105.SH": [[], []], "300412.SZ": [[], []], "002528.SZ": [[], []],
    "002141.SZ": [[], []], "300587.SZ": [[], []], "000962.SZ": [[], []], "000017.SZ": [[], []], "601869.SH": [[], []],
    "002628.SZ": [[], []], "600130.SH": [[], []], "000760.SZ": [[], []], "002932.SZ": [[], []], "300018.SZ": [[], []],
    "300561.SZ": [[], []], "300225.SZ": [[], []], "300216.SZ": [[], []], "603713.SH": [[], []], "300436.SZ": [[], []],
    "600303.SH": [[], []], "300492.SZ": [[], []], "002213.SZ": [[], []], "300358.SZ": [[], []], "002590.SZ": [[], []],
    "002684.SZ": [[], []], "002575.SZ": [[], []], "600712.SH": [[], []], "300167.SZ": [[], []], "000722.SZ": [[], []],
    "600255.SH": [[], []], "600726.SH": [[], []], "002447.SZ": [[], []], "000971.SZ": [[], []], "002198.SZ": [[], []],
    "300129.SZ": [[], []], "002143.SZ": [[], []], "000721.SZ": [[], []], "000506.SZ": [[], []], "000509.SZ": [[], []],
    "300032.SZ": [[], []], "601789.SH": [[], []], "002379.SZ": [[], []], "600139.SH": [[], []], "600212.SH": [[], []],
    "601700.SH": [[], []], "600084.SH": [[], []], "300090.SZ": [[], []], "002471.SZ": [[], []], "000897.SZ": [[], []],
    "300028.SZ": [[], []], "600777.SH": [[], []], "002554.SZ": [[], []], "300345.SZ": [[], []], "600131.SH": [[], []],}


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




