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
@describe: 通过Wind接口获取品种的复权主力合约价格和展期收益率
"""

import csv
import multiprocessing
import numpy as np
import re

from WindPy import *

END_DATE = "2017-10-15"


def get_futures_data(param, abbr, begin_date):

    w.start()

    data_file = "output/" + abbr + ".csv"
    factor = 1.0
    pre_main_contract = ""
    pre_main_price = 0.0

    trade_day_list = w.tdays(begin_date, END_DATE, "")

    for day in trade_day_list.Data[0]:
        str_date = day.strftime('%Y-%m-%d')
        dicts = {}

        prefix = "date="
        suffix = ";sectorid="
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

        if pre_main_contract != dict(main_contract).keys()[0] and pre_main_contract != "":
            factor = factor * pre_main_price/(main_contract_price.Data[0][0])

        pre_main_contract = dict(main_contract).keys()[0]
        pre_main_price = main_contract_price.Data[0][0]

        for i in range(1, 12):
            second_contract = sorted(dicts.items(), key=lambda item: item[1], reverse=True)[i:i+1]
            if not second_contract:
                break

            second_month = re.findall(r"\d+\.?\d*", dict(second_contract).keys()[0])
            int_second_month = float(second_month[0].encode("utf-8"))

            if int_main_month < 1000.0:
                int_main_month += 1000.0

            if int_second_month < 1000.0:
                int_second_month += 1000.0

            if int_main_month < int_second_month:
                second_contract_price = w.wsd(dict(second_contract).keys(), "close", str_date, str_date, "")
                second_contract_delivery = w.wsd(dict(second_contract).keys(), "lastdelivery_date", str_date, str_date, "")
                break

        if second_contract_price.Data[0][0] is None:
            rollover = "NaN"
        else:
            rollover = 365 * ((np.log(main_contract_price.Data[0][0]) - np.log(second_contract_price.Data[0][0])) /
                        (second_contract_delivery.Data[0][0] - main_contract_delivery.Data[0][0]).days)

        with open(data_file, "ab+") as csvfile:
            writer = csv.writer(csvfile)
            writer.writerow([str_date, dict(main_contract).keys()[0], dict(main_contract).values()[0],
                             main_contract_price.Data[0][0],
                             main_contract_delivery.Data[0][0],
                             dict(second_contract).keys()[0], dict(second_contract).values()[0],
                             second_contract_price.Data[0][0],
                             second_contract_delivery.Data[0][0],
                             factor * main_contract_price.Data[0][0],
                             rollover
                             ])

if __name__ == "__main__":

    p_sr = multiprocessing.Process(target=get_futures_data, args=("a599010405000000", "SR", "2010-01-01"))
    p_cf = multiprocessing.Process(target=get_futures_data, args=("a599010404000000", "CF", "2017-08-30"))
    p_zc = multiprocessing.Process(target=get_futures_data, args=("1000011012000000", "ZC", "2014-08-15"))
    p_fg = multiprocessing.Process(target=get_futures_data, args=("1000008549000000", "FG", "2012-12-03"))
    p_ta = multiprocessing.Process(target=get_futures_data, args=("a599010407000000", "TA", "2010-01-01"))
    p_ma = multiprocessing.Process(target=get_futures_data, args=("1000005981000000", "MA", "2011-10-28"))
    p_wh = multiprocessing.Process(target=get_futures_data, args=("a599010403000000", "WH", "2016-07-20"))
    p_oi = multiprocessing.Process(target=get_futures_data, args=("a599010408000000", "OI", "2016-11-22"))
    p_rm = multiprocessing.Process(target=get_futures_data, args=("1000008622000000", "RM", "2017-04-20"))

    p_sr.start()
    p_cf.start()
    p_zc.start()
    p_fg.start()
    p_ta.start()
    p_ma.start()
    p_wh.start()
    p_oi.start()
    p_rm.start()

