# -*- coding:utf-8 -*-

# 名称：获取主力合约价格
# 时间：2017-09-18
# 作者：xiaxb

import csv
import math
import datetime

from dateutil.relativedelta import relativedelta
from WindPy import *

################################################
# 获取指定日期，指定品种的收盘价
# code：期货品种主力合约代码，字符串；格式为RB.SHF
# date：日期，字符串；格式为2000-12-31
# 返回值：正常-指定品种收盘价格；异常-NaN
###############################################


def get_close_price(code, date):

    results = w.wsd(code, "close", date, date, "")

    if results.ErrorCode < 0:
        return "NaN"
    else:
        return results.Data[0][0]

########################################
# 根据主力合约，获取具体的月合约代码
# code：期货品种主力合约代码，格式为RB.SHF
# date：日期，格式为2000-12-31
# 返回值：正常-指定月合约代码；异常-NaN
########################################


def get_history_code(code, date):
    results = w.wsd(code, "trade_hiscode", date, date, "")

    if results.ErrorCode < 0:
        return "NaN"
    else:
        return results.Data[0][0]

#######################################
# 获取复权因子
# code：期货品种主力合约代码，格式为RB.SHF
# start_time：开始日期，格式为2000-12-31
# end_time：开始日期，格式为2000-12-31
# 返回值：复权因子
#######################################


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

#######################################
# 根据交易品种和交易日期，获取近月合约价格
# code：期货品种合约代码，格式为RB
# date：日期，字符串；格式为2000-12-31
# 返回值：返回近月合约价格
#######################################


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

#####################################
# 获取下月合约价格
# code：期货品种合约代码，格式为RB.SHF
# date：日期，格式为2000-12-31
# 返回值：返回下月合约价格
#####################################


def get_next_contract_price(code, date):

    '''
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
    '''

    # 上期所16个品种 .SHF
    SHF_list = ['RB', 'HC']

    # 大商所16个品种 .DCE
    DCE_list = ['I', 'J', 'JM']

    # 郑商所19个品种 .CZC
    CZC_list = ['ZC']

    date_time = datetime.strptime(date, "%Y-%m-%d")
    serial_date = (date_time - relativedelta(months=-1)).strftime('%y%m')

    if code in SHF_list:
        recent_code = code + serial_date + '.SHF'
    elif code in DCE_list:
        recent_code = code + serial_date + '.DCE'
    else:
        recent_code = code + serial_date + '.CZC'

    recent_price = w.wsd(recent_code, "close", date, date, "")

    if recent_price.ErrorCode < 0:
        return "NaN"
    else:
        return recent_price.Data[0][0]

######################################
# 获取下月合约交割日期
# code：期货品种合约代码，格式为RB.SHF
# date：日期，格式为2000-12-31
# 返回值：返回交割日期 datetime
######################################


def get_next_delivery_date(code, date):

    '''
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
    '''

    # 上期所16个品种 .SHF
    SHF_list = ['RB', 'HC']

    # 大商所16个品种 .DCE
    DCE_list = ['I', 'J', 'JM']

    # 郑商所19个品种 .CZC
    CZC_list = ['ZC']

    date_time = datetime.strptime(date, "%Y-%m-%d")
    serial_date = (date_time - relativedelta(months=-1)).strftime('%y%m')

    if code in SHF_list:
        recent_code = code + serial_date + '.SHF'
    elif code in DCE_list:
        recent_code = code + serial_date + '.DCE'
    else:
        recent_code = code + serial_date + '.CZC'

    results = w.wsd(recent_code, "lastdelivery_date", date, date, "")

    if results.ErrorCode < 0 or results.Data[0][0] is None:
        return "NaN"

    if results.ErrorCode == 0:
        return results.Data[0][0]

######################################
# 获取主力合约交割日期
# code：期货品种合约代码，格式为RB.SHF
# date：日期，格式为2000-12-31、
# 返回值：主力合约交割日期，datetime
######################################


def get_main_delivery_date(code, date):
    results = w.wsd(code, "trade_hiscode", date, date, "")

    if results.ErrorCode < 0:
        return "NaN"

    delivery_date = w.wsd(results.Data[0][0], "lastdelivery_date", date, date, "")

    if delivery_date.ErrorCode < 0 or delivery_date.Data[0][0] is None:
        return "NaN"
    else:
        return delivery_date.Data[0][0]

# 计算展期收益率
# commodity：交易的品种，格式RB
# code：期货主力合约代码，格式为RB.SHF
# date：日期，格式为2000-12-31


def get_rollover_yield(commodity, code, date):
    if not isinstance(date, str):
        date = date.strftime("%Y-%m-%d")
    print date
    next_price = get_next_contract_price(commodity, date)
    main_price = get_close_price(code, date)

    if next_price is None or main_price is None:
        return "NaN"

    print "code,next_price,main_price = ", code, next_price, main_price

    if get_next_delivery_date(commodity, date) == "NaN" or get_main_delivery_date(code, date) == "NaN":
        return "NaN"
    print get_main_delivery_date(code, date)
    next_delta = (get_next_delivery_date(commodity, date) - datetime.strptime(date, "%Y-%m-%d")).days
    main_delta = (get_main_delivery_date(code, date) - datetime.strptime(date, "%Y-%m-%d")).days

    if(main_delta-next_delta) == 0:
        rollover_yield = 365 * (math.log(next_price) - math.log(main_price))
    else:
        rollover_yield = 365*(math.log(next_price) - math.log(main_price))/(main_delta-next_delta)

    return rollover_yield

# 获取当天涨跌幅前N的期货品种
# date：日期，格式为2000-12-31


def get_sort_commodity(date):

    '''
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
    '''

    # 上期所16个品种 .SHF
    SHF_list = ['RB', 'HC']

    # 大商所16个品种 .DCE
    DCE_list = ['I', 'J', 'JM']

    # 郑商所19个品种 .CZC
    CZC_list = ['ZC']

    dicts = {}
    rollover = 0.0
    main_contract = 'NaN'

    pool_long = []
    pool_short = []

    for key in SHF_list+DCE_list+CZC_list:
        if key in SHF_list:
            main_contract = key+'.SHF'
        elif key in DCE_list:
            main_contract = key+'.DCE'
        else:
            main_contract = key+'.CZC'

        rollover = get_rollover_yield(key, main_contract, date)
        print "main_contract,rollover = ",main_contract,rollover

        if rollover != "NaN":
            dicts[key] = rollover
            print "dicts[key] ,rollover = ", key, rollover

    pool_short += sorted(dicts.items(), key=lambda item: item[1])[:1]
    pool_long += sorted(dicts.items(), key=lambda item: item[1])[-1:]
    pool_list_long = dict(pool_long).keys()
    pool_list_short = dict(pool_short).keys()

    return pool_list_long, pool_list_short

# 获取N个交易日后的日期
# period: 日期偏移量，正数表示往后推移，负数表示往前推移
# date：日期，格式为2000-12-31


def get_offset_day(date, period):
    offset_day = w.tdaysoffset(period, date, "")

    return offset_day

if __name__ == "__main__":

    """
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
    """

    # 上期所16个品种 .SHF
    SHF_list = ['RB', 'HC']

    # 大商所16个品种 .DCE
    DCE_list = ['I', 'J', 'JM']

    # 郑商所19个品种 .CZC
    CZC_list = ['ZC']

    start_date = "2015-05-30"
    profit = 0.0
    total_profit = 0.0
    cycle = 20
    long_list = []
    short_list = []
    long_dict = {}
    short_dict = {}

    w.start()

    tmp_date = start_date
    for i in range(cycle):
        close_price = 0.0

        long_list, short_list = get_sort_commodity(tmp_date)
        print long_list, short_list

        # 多单的处理逻辑
        if not long_dict:
            for key in long_list:
                if key in SHF_list:
                    main_contract = key + '.SHF'
                elif key in DCE_list:
                    main_contract = key + '.DCE'
                else:
                    main_contract = key + '.CZC'
                long_dict[key] = get_close_price(main_contract, tmp_date)

                with open("backtest.csv", "ab+") as csvfile:
                    writer = csv.writer(csvfile)
                    writer.writerow([tmp_date, main_contract, "open_long", long_dict[key], profit, total_profit])

        else:
            close_long_list = list(set(long_dict.keys()).difference(set(long_list)))
            for tmp in close_long_list:
                if tmp in SHF_list:
                    main_contract = tmp + '.SHF'
                elif tmp in DCE_list:
                    main_contract = tmp + '.DCE'
                else:
                    main_contract = tmp + '.CZC'
                close_price = get_close_price(main_contract, tmp_date)

                # profit = close_price - long_dict[tmp]
                profit = (close_price - long_dict[tmp]) / long_dict[tmp]
                total_profit += profit
                long_dict.pop(tmp)

                with open("backtest.csv", "ab+") as csvfile:
                    writer = csv.writer(csvfile)
                    writer.writerow([tmp_date, main_contract, "close_long", close_price, profit, total_profit])

            open_long_list = list(set(long_list).difference(set(long_dict.keys())))
            for tmp2 in open_long_list:
                if tmp2 in SHF_list:
                    main_contract = tmp2 + '.SHF'
                elif tmp2 in DCE_list:
                    main_contract = tmp2 + '.DCE'
                else:
                    main_contract = tmp2 + '.CZC'
                long_dict[tmp2] = get_close_price(main_contract, tmp_date)

                with open("backtest.csv", "ab+") as csvfile:
                    writer = csv.writer(csvfile)
                    writer.writerow([tmp_date, main_contract, "open_long", long_dict[tmp2], profit, total_profit])

        # 空单的处理逻辑
        if not short_dict:
            for key in short_list:
                if key in SHF_list:
                    main_contract = key + '.SHF'
                elif key in DCE_list:
                    main_contract = key + '.DCE'
                else:
                    main_contract = key + '.CZC'
                short_dict[key] = get_close_price(main_contract, tmp_date)

                with open("backtest.csv", "ab+") as csvfile:
                    writer = csv.writer(csvfile)
                    writer.writerow([tmp_date, main_contract, "open_short", short_dict[key], profit, total_profit])
        else:
            close_short_list = list(set(short_dict.keys()).difference(set(short_list)))
            for s_tmp in close_short_list:
                if s_tmp in SHF_list:
                    main_contract = s_tmp + '.SHF'
                elif s_tmp in DCE_list:
                    main_contract = s_tmp + '.DCE'
                else:
                    main_contract = s_tmp + '.CZC'
                close_price = get_close_price(main_contract, tmp_date)

                # profit = short_dict[s_tmp] - close_price
                profit = (short_dict[s_tmp] - close_price) / short_dict[s_tmp]
                total_profit += profit
                short_dict.pop(s_tmp)

                with open("backtest.csv", "ab+") as csvfile:
                    writer = csv.writer(csvfile)
                    writer.writerow([tmp_date, main_contract, "close_short", close_price, profit, total_profit])

            open_short_list = list(set(short_list).difference(set(short_dict.keys())))
            for tmp2 in open_short_list:
                if tmp2 in SHF_list:
                    main_contract = tmp2 + '.SHF'
                elif tmp2 in DCE_list:
                    main_contract = tmp2 + '.DCE'
                else:
                    main_contract = tmp2 + '.CZC'
                short_dict[tmp2] = get_close_price(main_contract, tmp_date)

                with open("backtest.csv", "ab+") as csvfile:
                    writer = csv.writer(csvfile)
                    writer.writerow([tmp_date, main_contract, "open_short", short_dict[tmp2], profit, total_profit])

        results = get_offset_day(tmp_date, cycle)
        tmp_date = results.Data[0][0]

        print total_profit


