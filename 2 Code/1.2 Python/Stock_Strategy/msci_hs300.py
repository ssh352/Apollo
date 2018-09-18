#!/usr/bin/env python  
# encoding: utf-8  

""" 
@version: v1.0 
@author: xiaxianba 
@license: Apache Licence  
@contact: scuxia@gmail.com 
@site: http://weibo.com/xiaxianba 
@software: PyCharm 
@file: msci_hs300.py 
@time: 2018/6/8 16:16 
@describe: 
"""

from WindPy import *

if __name__ == "__main__":

    list_same = []
    list_hs300_other = []
    list_msci_other = []

    list_hs300 = []
    list_msci = []


    w.start()

    reuslt_hs300 = w.wset("sectorconstituent","date=2018-06-08;sectorid=a001030201000000")

    list_hs300 = reuslt_hs300.Data[1]

    result_msci = w.wset("sectorconstituent","date=2018-06-08;sectorid=1000011313000000")

    list_msci = result_msci.Data[1]

    list_same = list(set(list_hs300).intersection(set(list_msci)))

    list_hs300_other = list(set(list_hs300).difference(set(list_msci)))

    list_msci_other = list(set(list_msci).difference(set(list_hs300)))

    print len(list_same), list_same
    print len(list_hs300_other), list_hs300_other
    print len(list_msci_other), list_msci_other

