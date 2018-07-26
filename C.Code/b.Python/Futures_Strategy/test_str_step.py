#!/usr/bin/env python  
# encoding: utf-8  

""" 
@version: v1.0 
@author: xiaxianba 
@license: Apache Licence  
@contact: scuxia@gmail.com 
@site: http://weibo.com/xiaxianba 
@software: PyCharm 
@file: test_str_step.py 
@time: 2018/1/12 10:17 
@describe: 
"""

from WindPy import *


# 获取交易日列表
def get_trade_day(begin_date, end_date):

    list_trade_day = []
    result_trade_day = w.tdays(begin_date, end_date, "")

    for day in result_trade_day.Data[0]:
        str_date = day.strftime('%Y-%m-%d')
        list_trade_day.append(str_date)

    return list_trade_day

if __name__ == "__main__":

    w.start()
    result = w.wset("sectorconstituent", "date=2018-07-26;sectorid=a001010100000000")
    print result.Data[1]

    # date_start_t = datetime.strptime("2018-02-08", "%Y-%m-%d")
    # date_now_t = datetime.today()
    #
    # if (date_now_t - date_start_t).days > 365:
    #     print "LOSSES CAUSED BY USING WIND QUANT API FOR Python."
    #     sys.exit()

    # w.start()
    #
    # list_days = get_trade_day("2017-01-01", "2017-12-31")
    #
    # list_temp = list_days[::5]
    # print list_temp
