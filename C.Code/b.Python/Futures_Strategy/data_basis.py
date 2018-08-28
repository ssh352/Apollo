#!/usr/bin/env python  
# encoding: utf-8  

""" 
@version: v1.0 
@author: xiaxianba 
@license: Apache Licence  
@contact: scuxia@gmail.com 
@site: http://weibo.com/xiaxianba 
@software: PyCharm 
@file: data_basis.py 
@time: 2018/8/27 15:24 
@describe: 
    1.方法一：从Wind上获取现货和主力合约的价格，按照公式计算两者之差；
    2.方法二：从Wind上获取近月合约（用近月合约替代现货）和远月合约的价格，计算两者之差。
"""

from WindPy import *
import numpy as np
import pandas as pd
import math

dict_commodity = {'CU.SHF':'W00030SPT.NM', 'AL.SHF':'W00031SPT.NM', 'ZN.SHF':'W00032SPT.NM',
             'PB.SHF':'W00033SPT.NM',  'AG.SHF':'W00034SPT.NM','NI.SHF':'W00042SPT.NM',
             'SN.SHF':'W00041SPT.NM', 'RB.SHF':'W00035SPT.NM', 'SM.CZC':'W00027SPT.NM',
             'WR.SHF':'W00036SPT.NM', 'HC.SHF':'W00037SPT.NM', 'FU.SHF':'W00038SPT.NM',
             'BU.SHF':'W00039SPT.NM', 'RU.SHF':'W00040SPT.NM', 'M.DCE':'W00003SPT.NM',
             'Y.DCE':'W00004SPT.NM', 'A.DCE':'W00002SPT.NM', 'B.DCE':'W00002SPT.NM',
             'P.DCE':'W00005SPT.NM', 'C.DCE':'W00001SPT.NM', 'J.DCE':'W00012SPT.NM',
             'V.DCE':'W00010SPT.NM', 'I.DCE':'W00014SPT.NM', 'BB.DCE':'W00007SPT.NM',
             'FB.DCE':'W00008SPT.NM','SF.CZC':'W00028SPT.NM', 'RM.CZC':'W00024SPT.NM',
             'JM.DCE':'W00013SPT.NM', 'JD.DCE':'W00006SPT.NM','CS.DCE':'W00043SPT.NM',
             'SR.CZC':'W00017SPT.NM', 'CF.CZC':'W00016SPT.NM','OI.CZC':'W00019SPT.NM',
             'ZC.CZC':'W00025SPT.NM', 'FG.CZC':'W00022SPT.NM', 'TA.CZC':'W00018SPT.NM',
             'MA.CZC':'W00021SPT.NM', 'WH.CZC':'W00015SPT.NM', 'PM.CZC':'W00015SPT.NM',
             'RI.CZC':'W00029SPT.NM', 'LR.CZC':'W00021SPT.NM', 'JR.CZC':'W00026SPT.NM',
             'RS.CZC':'W00024SPT.NM',
             # 'AU.SHF':'a599010205000000','L.DCE':'a599010308000000','PP.DCE':'1000011468000000',
             # 'CY.CZC':'1000011479000000',
             }

if __name__ == "__main__":

    datestr = '2018-08-27'
    w.start()

    for key in dict_commodity.keys():
        result1 = w.wsd(key, "close", datestr, datestr, "",)
        price1 = result1.Data[0][0]

        result2 = w.wsd(dict_commodity[key], "close", datestr, datestr, "",)
        price2 = result2.Data[0][0]

        if isinstance(result1.Data[0][0], float) and isinstance(result2.Data[0][0], float):
            price3 = price2 - price1
        else:
            price3 = None

        print price1,price2,price3

