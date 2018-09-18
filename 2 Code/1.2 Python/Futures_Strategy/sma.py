#!/usr/bin/env python  
# encoding: utf-8  

""" 
@version: v1.0 
@author: xiaxianba 
@license: Apache Licence  
@contact: scuxia@gmail.com 
@site: http://weibo.com/xiaxianba 
@software: PyCharm 
@file: sma.py 
@time: 2017/10/26 14:57 
"""
import time
import talib
import numpy as np
import pandas as pd

close_price = np.random.random(20)

print "close_price:", close_price

start = time.clock()
talib_price = talib.SMA(close_price, timeperiod = 5)
end = time.clock()

print talib_price
print end-start

df = pd.DataFrame(close_price, columns=['close_price'])

df.head()

start = time.clock()
df_price = df.rolling(5).mean()
end = time.clock()

print df_price
print end-start




