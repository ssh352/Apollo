#!/usr/bin/env python  
# encoding: utf-8  

""" 
@version: v1.0 
@author: xiaxianba
@license: Apache Licence  
@contact: 778757080@qq.com 
@site: http://xianba.github.com/ 
@software: PyCharm 
@file: rollover.py.py 
@time: 2017/10/15 09:23 
"""

import csv
import multiprocessing
import re

from WindPy import *

BEGIN_DATE = "2010-01-01"
END_DATE = "2017-10-12"


def get_futures_data(param, abbr):

    w.start()

    data_file = abbr + "_TS4_2010.csv"

    trade_day_list = w.tdays(BEGIN_DATE, END_DATE, "")

    for day in trade_day_list.Data[0]:
        str_date = day.strftime('%Y-%m-%d')
        dicts = {}

        prefix = "date="
        suffix = ";sectorid="
        # suffix = ";sectorid=a599010206000000"     # RB.SHF
        # suffix = ";sectorid=a599010202000000"     # CU.SHF
        # suffix = ";sectorid=a599010405000000"     # SR.CZC
        # suffix = ";sectorid=a599010404000000"     # CF.CZC

        scope = prefix + str_date + suffix + param

        result = w.wset("sectorconstituent", scope)

        if result.ErrorCode < 0 or not result.Data:
            return "NaN"

        list_contract = result.Data[1]

        for contract in list_contract:
            result = w.wsd(contract, "oi", str_date, str_date, "")
            if result.ErrorCode < 0:
                return "NaN"

            dicts[contract] = result.Data[0][0]

        main_contract = sorted(dicts.items(), key=lambda item: item[1], reverse=True)[:1]

        main_contract_price = w.wsd(dict(main_contract).keys(), "close", str_date, str_date, "")

        main_contract_delivery = w.wsd(dict(main_contract).keys(), "lastdelivery_date", str_date, str_date, "")

        main_month = re.findall(r"\d+\.?\d*", dict(main_contract).keys()[0])
        int_main_month = float(main_month[0].encode("utf-8"))

        for i in range(1, 12):
            second_contract = sorted(dicts.items(), key=lambda item: item[1], reverse=True)[i:i+1]

            if not second_contract:
                return

            second_month = re.findall(r"\d+\.?\d*", dict(second_contract).keys()[0])
            int_second_month = float(second_month[0].encode("utf-8"))

            if int_main_month < int_second_month:
                second_contract_price = w.wsd(dict(second_contract).keys(), "close", str_date, str_date, "")
                second_contract_delivery = w.wsd(dict(second_contract).keys(), "lastdelivery_date", str_date, str_date, "")
                break

        with open(data_file, "ab+") as csvfile:
            writer = csv.writer(csvfile)
            writer.writerow([str_date, dict(main_contract).keys()[0], dict(main_contract).values()[0],
                             main_contract_price.Data[0][0],
                             main_contract_delivery.Data[0][0],
                             dict(second_contract).keys()[0], dict(second_contract).values()[0],
                             second_contract_price.Data[0][0],
                             second_contract_delivery.Data[0][0]
                             ])

if __name__ == "__main__":

    get_futures_data("a599010401000000", "ALL")
    #  p_rb = multiprocessing.Process(target=get_futures_data, args=("a599010206000000", "RB"))
    # p_cu = multiprocessing.Process(target=get_futures_data, args=("a599010202000000", "CU"))
    # p_sr = multiprocessing.Process(target=get_futures_data, args=("a599010405000000", "SR"))
    # p_cf = multiprocessing.Process(target=get_futures_data, args=("a599010404000000", "CF"))

    # p_rb.start()
    # p_cu.start()
    # p_sr.start()
    # p_cf.start()


