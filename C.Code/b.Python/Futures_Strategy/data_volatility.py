#!/usr/bin/env python  
# encoding: utf-8  

""" 
@version: v1.0 
@author: xiaxianba 
@license: Apache Licence  
@contact: scuxia@gmail.com 
@site: http://weibo.com/xiaxianba 
@software: PyCharm 
@file: data_volatility.py 
@time: 2018/8/27 13:38 
@describe: 
"""

from WindPy import *
import numpy as np
import pandas as pd
import math

if __name__ == "__main__":
    w.start()

    tp_close = w.wsd("CU.SHF", "close", "2017-08-25", "2018-08-26", "", usedf=True)
    df_close = tp_close[1]
    df_change = df_close.pct_change()

    f_std = df_change.std()

    f_volatility = f_std*math.sqrt(252)
