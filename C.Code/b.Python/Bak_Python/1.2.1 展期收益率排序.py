# -*- coding:utf-8 -*-

# 名称：获取主力合约价格
# 时间：2017-09-18
# 作者：xiaxb

import csv
import math
import datetime

from dateutil.relativedelta import relativedelta
from WindPy import *

# 获取指定日期，指定品种的收盘价
# code：期货品种主力合约代码，格式为RB.SHF
# date：日期，格式为2000-12-31


def get_close_price(code, date):

    results = w.wsd(code, "close", date, date, "PriceAdj=B")

    if results.ErrorCode < 0:
        return "NaN"
    else:
        return results.Data[0][0]

# 获取指定日期，月合约代码
# code：期货品种主力合约代码，格式为RB.SHF
# date：日期，格式为2000-12-31


def get_history_code(code, date):
    results = w.wsd(code, "trade_hiscode", date, date, "PriceAdj=B")

    if results.ErrorCode < 0:
        return "NaN"
    else:
        return results.Data[0][0]

# 获取复权因子
# code：期货品种主力合约代码，格式为RB.SHF
# start_time：开始日期，格式为2000-12-31
# end_time：开始日期，格式为2000-12-31


def get_recovery_factor(code, start_time, end_time):

    factor = 1
    prev_price = 0
    prev_code = 0

    results = w.wsd(code, "close,trade_hiscode", start_time, end_time, "PriceAdj=B")

    for tmp_time in results.Times:
        current_price = get_close_price(code, tmp_time)
        current_code = get_history_code(code, tmp_time)

        if prev_price != 0 and current_code != prev_code:
            factor = (prev_price / current_price) * factor
            print factor

        prev_price = current_price
        prev_code = current_code

    return factor

# 获取近月合约价格，可能是当月或者下个月
# code：期货品种合约代码，格式为RB.SHF
# date：日期，格式为2000-12-31


def get_recent_contract_price(code, date):

    date_time = datetime.strptime(date, "%Y-%m-%d")
    curr_serial_date = date_time.strftime('%y%m')
    next_serial_date = (date_time - relativedelta(months=-1)).strftime('%y%m')

    recent_code = code + curr_serial_date + ".SHF"

    delivery_date = w.wsd(recent_code, "lastdelivery_date", date, date, "")

    if delivery_date.ErrorCode < 0:
        return "NaN"

    if date_time < delivery_date.Data[0][0]:
        recent_code = code + curr_serial_date + ".SHF"
        recent_price = w.wsd(recent_code, "close", date, date, "")

        if recent_price.Data[0][0] is None:
            return "NaN"
        else:
            return recent_price.Data[0][0]
    else:
        recent_code = code + next_serial_date + ".SHF"
        recent_price = w.wsd(recent_code, "close", date, date, "")

        if recent_price.Data[0][0] is None:
            return "NaN"
        else:
            return recent_price.Data[0][0]

# 获取下月合约价格
# code：期货品种合约代码，格式为RB.SHF
# date：日期，格式为2000-12-31


def get_next_contract_price(code, date):

    date_time = datetime.strptime(date, "%Y-%m-%d")
    serial_date = (date_time - relativedelta(months=-1)).strftime('%y%m')

    recent_code = code + serial_date + ".SHF"
    recent_price = w.wsd(recent_code, "close", date, date, "")

    if recent_price.ErrorCode < 0:
        return "NaN"
    else:
        return recent_price.Data[0][0]

# 获取下月合约交割日期
# code：期货品种合约代码，格式为RB.SHF
# date：日期，格式为2000-12-31


def get_next_delivery_date(code, date):
    date_time = datetime.strptime(date, "%Y-%m-%d")
    serial_date = (date_time - relativedelta(months=-1)).strftime('%y%m')

    recent_code = code + serial_date + ".SHF"
    results = w.wsd(recent_code, "lastdelivery_date", date, date, "")

    if results.ErrorCode < 0:
        return "NaN"
    else:
        return results.Data[0][0]

# 获取主力合约交割日期
# code：期货品种合约代码，格式为RB.SHF
# date：日期，格式为2000-12-31


def get_main_delivery_date(code, date):
    results = w.wsd(code, "trade_hiscode", date, date, "")

    if results.ErrorCode < 0:
        return "NaN"

    delivery_date = w.wsd(results.Data[0][0], "lastdelivery_date", date, date, "")

    if results.ErrorCode < 0:
        return "NaN"
    else:
        return delivery_date.Data[0][0]

# 计算展期收益率
# commodity：交易的品种，格式RB
# code：期货主力合约代码，格式为RB.SHF
# date：日期，格式为2000-12-31


def get_rollover_yield(commodity, code, date):

    next_price = get_next_contract_price(commodity, date)
    main_price = get_close_price(code, date)

    if type(get_next_delivery_date(commodity, date)) != datetime or \
                    type(get_main_delivery_date(code, date)) != datetime:
        return "NaN"

    next_delta = (get_next_delivery_date(commodity, date) - datetime.strptime(date, "%Y-%m-%d")).days
    main_delta = (get_main_delivery_date(code, date) - datetime.strptime(date, "%Y-%m-%d")).days

    if main_delta == next_delta:
        rollover_yield = 365*(math.log(next_price) - math.log(main_price))
    else:
        rollover_yield = 365*(math.log(next_price) - math.log(main_price))/(main_delta-next_delta)

    return rollover_yield

if __name__ == "__main__":

    # 上期所16个品种 .SHF
    SHF_list = ['CU', 'AL', 'ZN', 'PB', 'AU', 'AG', 'NI', 'SN',
                'RB', 'WR', 'HC', 'FU', 'BU', 'SC', 'RU', 'IM']

    # 大商所16个品种 .DCE
    DCE_list = ['M', 'Y', 'A', 'B', 'P', 'C', 'CS', 'JD', 'BB',
                'FB', 'L', 'V', 'PP', 'J', 'JM', 'I']

    # 郑商所19个品种 .CZC
    CZC_list = ['SR', 'CF', 'ZC', 'FG', 'TA', 'MA', 'WH', 'PM',
                'RI', 'LR', 'JR', 'RS', 'OI', 'RM', 'SF', 'SM',
                'CY']

    dicts = {}
    rollover = 0.0
    main_contract = 'NaN'
    date = "2017-09-12"

    pool_long = []
    pool_short = []

    w.start()

    for key in SHF_list+DCE_list+CZC_list:
        if key in SHF_list:
            main_contract = key+'.SHF'
        elif key in DCE_list:
            main_contract = key+'.DCE'
        else:
            main_contract = key+'.CZC'

        rollover = get_rollover_yield(key, main_contract, date)
        if rollover != "NaN":
            dicts[key] = rollover

    pool_short += sorted(dicts.items(), key=lambda item:item[1])[:4]
    pool_long += sorted(dicts.items(), key=lambda item:item[1])[-4:]
    pool_list_long = dict(pool_long).keys()
    pool_list_short = dict(pool_short).keys()

    print pool_list_long
    print "=============="
    print pool_list_short

'''
    w.start()
    my_commodity = "RB"
    my_code = "RB.SHF"
    begin_date = date(2010, 1, 1)
    end_date = date(2010, 12, 31)

    for i in range((end_date - begin_date).days+1):
        day = begin_date + timedelta(days=i)
        print day
        get_rollover_yield(my_commodity, my_code, str(day))

    # values = get_rollover_yield(my_commodity, my_code, my_date)

    for key in dict_commodity:
        dict_commodity[key][1] = get_rollover_yield(key, dict_commodity[key][0], my_date)
        print key
        print dict_commodity[key][1]

    print dict_commodity


    dict_commodity = {"CU": "CU.SHF", "AL": "AL.SHF", "ZN": "ZN.SHF", "PB": "PB.SHF", "NI": "NI.SHF",
                      "": "", "": "", "": "", "": "",
                      "": "", "": "", "": "", "": "",
                      "": "", "": "", "": "", "": "",
                      "": "", "": "", "": "", "": "",
                      "": "", "": "", "": "", "": "",
                      "": "", "": "", "": "", "": "",
                      "": "", "": "", "": "", "": "",
                      "": "", "": "", "": "", "": "",
                      "": "", "": "", "": "", "": "", "": "",}
    my_code = "RB"
    my_date = "2010-03-12"

    w.start()
    values = get_next_delivery_date(my_code, my_date)

    print values

    w.start()

    ratio = get_rollover_yield("RB.SHF", "2009-03-27", "2017-09-18")
    print ratio


    factor = 1
    main_code = "RB.SHF"
    start_time = "2009-03-27"
    end_time = "2015-09-18"

    w.start()
    print "abc"

    results = w.wsd("RB.SHF", "close,trade_hiscode", start_time, end_time, "PriceAdj=B")

    prev_price = 0
    prev_code = 0
    for tmp_time in results.Times:
        current_price = get_close_price(main_code, tmp_time)
        current_code = get_history_code(main_code, tmp_time)
        print factor
        print prev_price,prev_code

        if prev_price !=0 and current_code != prev_code:
            factor = (prev_price / current_price)*factor
            print factor

        prev_price = current_price
        prev_code = current_code
'''


