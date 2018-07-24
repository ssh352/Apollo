#!/usr/bin/env python  
# encoding: utf-8  

""" 
@version: v1.0 
@author: xiaxianba 
@license: Apache Licence  
@contact: scuxia@gmail.com 
@site: http://weibo.com/xiaxianba 
@software: PyCharm 
@file: Draw_MA.py 
@time: 2018/4/2 11:05 
@describe: 
"""

import matplotlib.pyplot as plt
import pandas as pd
from WindPy import *

'''
Desc: 获取收盘价列表
Params: 
    -- target: XXXXXX.SZ,XXXXXX.SH
    -- start, end yyyy-mm-dd
'''
def get_close_list(target, start, end):

    list_price = []
    result = w.wsd(target, "close", start, end, "")

    if result.ErrorCode == 0:
        list_date = result.Times
        list_price = result.Data[0]

    return list_date, list_price

if __name__ == "__main__":

    w.start()

    list_t, list_p = get_close_list("000001.SZ", "2012-12-01", "2013-12-31")

    series_price = pd.Series(list_p, index=list_t)

    series_price.plot()
    plt.gcf().autofmt_xdate()    # 自动旋转日期标记
    plt.show()



