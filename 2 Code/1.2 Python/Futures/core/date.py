#!/usr/bin/env python  
# encoding: utf-8  

""" 
@version: v1.0 
@author: xiaxianba 
@license: Apache Licence  
@contact: scuxia@gmail.com 
@site: http://weibo.com/xiaxianba 
@software: PyCharm 
@file: date.py 
@time: 2018/9/19 17:28 
@describe: 
"""

from WindPy import *


# 获取交易日列表
def get_tradeday(begin_date, end_date):

    list_tradeday = []
    result_tradeday = w.tdays(begin_date, end_date, "")

    for day in result_tradeday.Data[0]:
        str_date = day.strftime('%Y-%m-%d')
        list_tradeday.append(str_date)

    return list_tradeday