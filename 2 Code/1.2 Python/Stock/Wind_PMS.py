#!/usr/bin/env python  
# encoding: utf-8  

""" 
@version: v1.0 
@author: xiaxianba 
@license: Apache Licence  
@contact: scuxia@gmail.com 
@site: http://weibo.com/xiaxianba 
@software: PyCharm 
@file: trade_mock_wind.py 
@time: 2018/5/28 14:30 
@describe: 
"""

from WindPy import *


# 从上证50中获取市值排名的股票
def get_marketvalue_rank(date):

    dict_return = {}
    dict_stock = {}

    prefix = "date="
    # suffix = ";sectorid=a001010100000000"  # 全部A股
    suffix = ";sectorid=a00103010b000000"  # 上证50指数成份
    scope = prefix + date + suffix

    result = w.wset("sectorconstituent", scope)
    if result.ErrorCode != 0:
        return
    else:
        list_stocks = result.Data[1]

    for stock in list_stocks:
        result_mv = w.wsd(stock, "mkt_cap_ard", date, date, "unit=1")
        if result_mv.ErrorCode != 0:
            continue
        else:
            dict_stock[stock] = result_mv.Data[0][0]

    sort_dict_stock = sorted(dict_stock.items(), key=lambda item: item[1], reverse=True)[30:35]

    for dict_key in dict(sort_dict_stock).keys():
        dict_return[dict_key] = date

    print dict_return
    return dict_return


# 函数入口
if __name__ == "__main__":

    w.start()
    result = w.wupf("StockA", "20180528", "000001.SZ", "10000", "10.57","Direction=Long;Method=BuySell;CreditTrading=No;type=flow")
    print result
