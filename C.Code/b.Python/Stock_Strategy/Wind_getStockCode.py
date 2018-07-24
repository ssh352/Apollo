#!/usr/bin/env python  
# encoding: utf-8  

""" 
@version: v1.0 
@author: xiaxianba 
@license: Apache Licence  
@contact: scuxia@gmail.com 
@site: http://weibo.com/xiaxianba 
@software: PyCharm 
@file: Wind_getStockCode.py 
@time: 2018/6/11 10:52 
@describe: 
"""

from WindPy import *
import json
w.start();

#获取上交所A股代码#
AllShAStock = w.wset("SectorConstituent","date=20151122;sectorId=a001010200000000;field=wind_code");
if AllShAStock.ErrorCode != 0:
        print("Get Data failed! exit!")
        exit()
#获取深圳交所A股代码#
AllSzAStock = w.wset("SectorConstituent","date=20151122;sectorId=a001010300000000;field=wind_code");
if AllSzAStock.ErrorCode != 0:
        print("Get Data failed! exit!")
        exit()
#获取所有A股代码#
AllAStock =w.wset("SectorConstituent","date=20151122;sectorId=a001010100000000;field=wind_code");
if AllAStock.ErrorCode != 0:
    print("Get Data failed! exit!")
    exit()

#保存股票代码到文件中#
with open('AllSzAStock.json',mode='w') as f:json.dump(AllSzAStock.Data[0],f);
with open('AllShAStock.json',mode='w') as f1:json.dump(AllShAStock.Data[0],f1);
with open('AllAStock.json',mode='w') as f2:json.dump(AllAStock.Data[0],f2);

#使用时使用with open('entry.json', 'r') as f:Data = json.load(f)#