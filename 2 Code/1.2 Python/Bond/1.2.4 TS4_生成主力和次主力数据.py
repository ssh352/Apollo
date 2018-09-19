# -*- coding:utf-8 -*-

# 名称：获取TS4（主力和次主力：按持仓量）期限结构，获取展期收益率
# 时间：2017-09-18
# 作者：xiaxb

import re
import csv
import math
import datetime

from dateutil.relativedelta import relativedelta
from WindPy import *

# 根据交易代码和时间，获取当天交易的所有合约
# code：期货品种主力合约代码，格式为RB.SHF
# date：日期，格式为2000-12-31
#
# 返回值：列表；所有合约列表


def get_contracts(str_date):

    prefix = "date="

    suffix = ";sectorid=a599010206000000"

    scope = prefix + str_date + suffix

    result = w.wset("sectorconstituent", scope)

    if result.ErrorCode < 0:
        return "NaN"

    return result.Data[1]

# 获取当天主力合约和次主力合约信息

def get_interest(str_date, list_contract):

    dicts = {}

    for contract in list_contract:
        result = w.wsd(contract, "oi", str_date, str_date, "")
        if result.ErrorCode < 0:
            return "NaN"
        dicts[contract] = result.Data[0][0]

        print str_date
        print contract, result.Data[0][0]

    main_contract = sorted(dicts.items(), key=lambda item: item[1], reverse=True)[:1]

    main_contract_price = w.wsd(dict(main_contract).keys(), "close", str_date, str_date, "")

    main_contract_delivery = w.wsd(dict(main_contract).keys(), "lastdelivery_date", str_date, str_date, "")

    main_month = re.findall(r"\d+\.?\d*", dict(main_contract).keys()[0])
    int_main_month = float(main_month[0].encode("utf-8"))

    for i in range(1, 12):
        second_contract = sorted(dicts.items(), key=lambda item: item[1], reverse=True)[i:i+1]

        second_month = re.findall(r"\d+\.?\d*", dict(second_contract).keys()[0])
        int_second_month = float(second_month[0].encode("utf-8"))

        if int_main_month < int_second_month:
            second_contract_price = w.wsd(dict(second_contract).keys(), "close", str_date, str_date, "")
            second_contract_delivery = w.wsd(dict(second_contract).keys(), "lastdelivery_date", str_date, str_date, "")
            break

    with open("rollover_TS4.csv", "ab+") as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow([str_date, dict(main_contract).keys()[0], dict(main_contract).values()[0],
                         main_contract_price.Data[0][0],
                         main_contract_delivery.Data[0][0],
                         dict(second_contract).keys()[0], dict(second_contract).values()[0],
                         second_contract_price.Data[0][0],
                         second_contract_delivery.Data[0][0]
                         ])

if __name__ == "__main__":

    begin_date = "2010-10-13"
    end_date = "2010-10-14"

    w.start()

    trade_day_list = w.tdays(begin_date, end_date, "")

    for day in trade_day_list.Data[0]:

        str_day = day.strftime('%Y-%m-%d')
        contract_list = get_contracts(str_day)
        get_interest(str_day, contract_list)
