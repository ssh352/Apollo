#!/usr/bin/env python  
# encoding: utf-8  

""" 
@version: v1.0 
@author: xiaxianba 
@license: Apache Licence  
@contact: scuxia@gmail.com 
@site: http://weibo.com/xiaxianba 
@software: PyCharm 
@file: NewStock.py 
@time: 2018/5/23 14:12 
@describe: 
"""

from WindPy import *

if __name__ == "__main__":

    prefix = "date="
    date = "2018-5-22"
    suffix = ";sectorid="
    value = "0201320000000000"
    # value = "1000032098000000"
    # value = "a001010l00000000"
    # value = "1000025141000000"
    # value = "1000027970000000"

    w.start()
    scope = prefix + date + suffix + value
    result = w.wset("sectorconstituent", scope)

    if result.ErrorCode == 0:
        print result.Data[1], len(result.Data[1])
    else:
        print result.ErrorCode

