#!/usr/bin/env python  
# encoding: utf-8  

""" 
@version: v1.0 
@author: xiaxianba 
@license: Apache Licence  
@contact: scuxia@gmail.com 
@site: http://weibo.com/xiaxianba 
@software: PyCharm 
@file: test_two.py 
@time: 2018/8/23 10:46 
@describe: 
"""

from WindPy import *
import re

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


def getViceMain(contract, date):

    dict_volume = {}

    prefix = "date="
    suffix = ";sectorid="

    result_main = w.wsd(contract, "trade_hiscode", date, date, "")

    print result_main
    main_contract = result_main.Data[0][0]

    main_contract_date = re.findall(r"\d+",main_contract)

    scope = prefix + date + suffix + dict_item[contract]
    result = w.wset("sectorconstituent", scope)

    list_contracts = result.Data[1]

    for contract in list_contracts:
        result_volume = w.wsd(contract, "volume", date, date, "")

        if result_volume.ErrorCode == 0:
            dict_volume[contract] = result_volume.Data[0][0]

    print main_contract
    print dict_volume
    dict_volume.pop(main_contract)
    second_contract = sorted(dict_volume.items(), key=lambda item: item[1], reverse=True)

    for sequ in range(len(second_contract)):

        vice_contract = dict(second_contract[sequ:sequ+1]).keys()
        vice_contract_date = re.findall(r"\d+",vice_contract[0])

        len_main_contract_date = len(main_contract_date[0])
        len_vice_contract_date = len(vice_contract_date[0])

        if len_main_contract_date == len_vice_contract_date and int(vice_contract_date[0])>int(main_contract_date[0]):
            print "hello"
            return(vice_contract)

        int_main_date = int(main_contract_date[0])
        int_vice_date = int(vice_contract_date[0])

        if len_main_contract_date != len_vice_contract_date:
            max_len = max(len_main_contract_date,len_vice_contract_date)
            if len_main_contract_date<max_len:
                print type(int_main_date),type(max_len)
                int_main_date += 10**(max_len-1)
            elif len_vice_contract_date<max_len:
                int_vice_date += 10**(max_len-1)

            if int_vice_date>int_main_date:
                print "get here"
                return (vice_contract)

if __name__ == "__main__":

    w.start()
    result = getViceMain('TA.CZC','2016-01-09')
    print result