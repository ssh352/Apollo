#!/usr/bin/env python  
# encoding: utf-8  

""" 
@version: v1.0 
@author: xiaxianba 
@license: Apache Licence  
@contact: scuxia@gmail.com 
@site: http://weibo.com/xiaxianba 
@software: PyCharm 
@file: SimTrade.py 
@time: 2017/02/06 13:00 
@describe: 展期数据
"""

import csv
import datetime as pydt
import numpy as np
import os
from WindPy import *


# 国内三大期货交易所共46个商品合约
list_item = ['CU.SHF', 'AL.SHF', 'ZN.SHF', 'PB.SHF', 'AU.SHF', 'AG.SHF', 'NI.SHF', 'SN.SHF',
             'RB.SHF', 'WR.SHF', 'HC.SHF', 'BU.SHF', 'RU.SHF', 'M.DCE', 'Y.DCE',
             'A.DCE', 'B.DCE', 'P.DCE', 'C.DCE', 'J.DCE', 'V.DCE', 'I.DCE',
             'BB.DCE', 'FB.DCE', 'L.DCE',  'PP.DCE',  'JM.DCE', 'CS.DCE', 'CY.CZC',
             'SR.CZC', 'CF.CZC', 'ZC.CZC', 'FG.CZC', 'TA.CZC', 'MA.CZC', 'WH.CZC', 'PM.CZC',
             'RI.CZC', 'LR.CZC', 'JR.CZC', 'RS.CZC', 'OI.CZC', 'RM.CZC', 'SF.CZC', 'SM.CZC', ]

# 商品合约对应的Wind板块id
dict_item = {'CU.SHF':'a599010202000000', 'AL.SHF':'a599010203000000', 'ZN.SHF':'a599010204000000',
             'PB.SHF':'1000002892000000', 'AU.SHF':'a599010205000000', 'AG.SHF':'1000006502000000',
             'NI.SHF':'1000011457000000', 'SN.SHF':'1000011458000000', 'RB.SHF':'a599010206000000',
             'WR.SHF':'a599010207000000', 'HC.SHF':'1000011455000000',
             'BU.SHF':'1000011013000000', 'RU.SHF':'a599010208000000', 'M.DCE':'a599010304000000',
             'Y.DCE':'a599010306000000', 'A.DCE':'a599010302000000', 'B.DCE':'a599010303000000',
             'P.DCE':'a599010307000000', 'C.DCE':'a599010305000000', 'J.DCE':'1000002976000000',
             'V.DCE':'a599010309000000', 'I.DCE':'1000011439000000', 'BB.DCE':'1000011466000000',
             'FB.DCE':'1000011465000000', 'L.DCE':'a599010308000000',  'PP.DCE':'1000011468000000',
             'JM.DCE':'1000009338000000', 'CS.DCE':'1000011469000000',
             'CY.CZC':'1000011479000000', 'SR.CZC':'a599010405000000', 'CF.CZC':'a599010404000000',
             'ZC.CZC':'1000011012000000', 'FG.CZC':'1000008549000000', 'TA.CZC':'a599010407000000',
             'MA.CZC':'1000005981000000', 'WH.CZC':'a599010403000000', 'PM.CZC':'1000006567000000',
             'RI.CZC':'a599010406000000', 'LR.CZC':'1000011476000000', 'JR.CZC':'1000011474000000',
             'RS.CZC':'1000008621000000', 'OI.CZC':'a599010408000000', 'RM.CZC':'1000008622000000',
             'SF.CZC':'1000011478000000', 'SM.CZC':'1000011477000000'}

def get_zhanqi(ext_list, datestr):

    dict_rate = {}

    prefix = "date="
    suffix = ";sectorid="

    file = os.getcwd() + "\\" + datestr + ".csv"
    with open(file, "wb") as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(["secuid", "exchangeid", "updatetime", "actionday", "tradingday", "value"])

        for item in ext_list:
            a = item.split('.')
            dicts = {}
            scope = prefix + datestr + suffix + dict_item[item]
            result = w.wset("sectorconstituent", scope)

            if result.ErrorCode == 0:
                list_contract = result.Data[1]

                for contract in list_contract:
                    result_volume = w.wsd(contract, "volume", datestr, datestr, "")

                    if result_volume.ErrorCode == 0:
                        dicts[contract] = result_volume.Data[0][0]

                result_main = w.wsd(item, "trade_hiscode", datestr, datestr, "")

                if result_main.ErrorCode == 0 and len(result_main.Data[0]) != 0:
                    main_contract = result_main.Data[0][0]
                    main_contract_price = w.wsi(main_contract, "close", datestr + " 11:00:00",
                                                datestr + " 11:30:00", "BarSize=30")

                    main_contract_delivery = w.wsd(main_contract, "lastdelivery_date", datestr, datestr, "")

                    dicts.pop(main_contract)

                    second_contract = sorted(dicts.items(), key=lambda item: item[1], reverse=True)[:1]

                    second_contract_price = w.wsi(dict(second_contract).keys(), "close", datestr + " 11:00:00",
                                                  datestr + " 11:30:00", "BarSize=30")
                    second_contract_delivery = w.wsd(dict(second_contract).keys(), "lastdelivery_date", datestr, datestr, "")

                    if isinstance(main_contract_price.Data[0][0],float) and isinstance(second_contract_price.Data[0][0],float):

                        diff_price = np.log(float(main_contract_price.Data[0][0])) - np.log(float(second_contract_price.Data[0][0]))
                        diff_date = (second_contract_delivery.Data[0][0] - main_contract_delivery.Data[0][0]).days
                        dict_rate[item] = 365 * diff_price / diff_date

                        writer.writerow([a[0], a[1], "000000", datestr, datestr, dict_rate[item]])


def gene_file(date):
    file = os.getcwd() + "\\" + date + ".csv"
    with open(file,"wb") as csvfile:
     writer=csv.writer(csvfile)
     writer.writerow(["secuid","exchangeid","updatetime","actionday","tradingday","value"])

if __name__ == "__main__":
    date_now = pydt.date.today() - pydt.timedelta(days=1)
    # datestr_tmp = date_now.strftime("%Y%m%d")
    datestr_tmp='20180823'
    if len(sys.argv) > 1:
      print sys.argv[1]
      datestr = sys.argv[1]
    w.start()
    date_date = pydt.datetime.strptime(datestr_tmp, "%Y%m%d")
    if date.isoweekday(date_date) < 6:
      get_zhanqi(list_item, datestr_tmp)
    else:
      gene_file(datestr_tmp)
    w.close()

