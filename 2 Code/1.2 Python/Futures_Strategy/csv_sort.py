#!/usr/bin/env python  
# encoding: utf-8  

""" 
@version: v1.0 
@author: xiaxianba 
@license: Apache Licence  
@contact: scuxia@gmail.com 
@site: http://http://weibo.com/xiaxianba 
@software: PyCharm 
@file: csv_sort.py.py 
@time: 2017/10/23 18:46 
@describe: 根据动量-期限结构找出最终交易的品种
           1.首先，把所有品种的涨跌幅排序等分成高收益组和低收一组
           2.然后，在高收益组中展期收益率排在前50%的做多，在低收益组中展期收益率排在后50%的做空
"""

"""
data.csv
date,ag,ag_,al,al_,au,au_,ax,ax_,bx,bx_
2017/10/16,-0.1234,1.1,0.1256,1.2,0.2235,0.9,-0.2458,1.2,0.1584,0.6
2017/10/17,0.2258,1.2,0.1256,1.3,0.2357,1,-0.2684,1.5,0.2548,0.9
2017/10/18,-0.3216,1.3,-0.1258,1.4,0.1458,1.1,-0.2248,1.8,-0.1856,1.2
2017/10/19,-0.2684,1.4,0.2569,1.5,-0.1234,1.2,0.1586,2.1,0.3526,1.5
2017/10/20,-0.2485,1.5,-0.1234,1.6,0.0086,1.3,0.5748,2.4,0.2458,1.8
2017/10/21,0.0025,1.6,-0.3254,1.7,0.0258,1.4,-0.2684,2.7,-0.2268,2.1
2017/10/22,-0.1258,1.7,0.0268,1.8,0.5786,1.5,0.1587,3,-0.1258,2.4
"""
import csv

with open("input/data.csv", "rb") as csvfile:
    reader = csv.DictReader(csvfile)
    for row in reader:
        dict_mom = {}
        dict_ext = {}
        dict_ext_sort = {}

        # 1.首先根据条件把所有的列按排序的因子分成几个字典
        for key in dict(row).keys():
            if key.endswith('_'):
                dict_ext[key] = dict(row)[key]
            else:
                dict_mom[key] = dict(row)[key]

        # print "dict_ext ", dict_ext
        # print "======"
        # print "dict_mom ", dict_mom

        # 2.根据第N个条件排序
        date = dict_mom.pop("date")
        mom_list = sorted(dict_mom.items(), key=lambda item: float(item[1]), reverse=True)[:len(dict_mom)//2]
        print date, dict(mom_list).keys()

        # 3.在上面条件基础上，再过滤新的条件
        for ext_key in dict(mom_list).keys():
            ext_key_suffix = ext_key + '_'
            if ext_key_suffix in dict(dict_ext).keys():
                dict_ext_sort[ext_key_suffix] = dict(dict_ext)[ext_key_suffix]

        print "dict_ext_sort ", dict_ext_sort

        # 4.根据第N+1个条件排序
        ext_list = sorted(dict_ext_sort.items(), key=lambda item: float(item[1]), reverse=True)[:len(dict_ext_sort) // 2]
        print date, dict(ext_list).keys()

        # date = row.pop("date")
        # long_list = sorted(row.items(), key=lambda item: float(item[1]), reverse=True)[:len(row)//2]
        # print date, dict(long_list).keys()


