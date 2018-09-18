#!/usr/bin/env python  
# encoding: utf-8  

""" 
@version: v1.0 
@author: xiaxianba 
@license: Apache Licence  
@contact: scuxia@gmail.com 
@site: http://weibo.com/xiaxianba 
@software: PyCharm 
@file: count_AShares.py 
@time: 2018/5/31 17:42 
@describe: 
"""

import csv

if __name__ == "__main__":

    count = 0

    csv_reader = csv.reader(open("data/msciA.csv"))

    for row in csv_reader:
        if row[3].split()[-1] == 'A':
            print row[3]
            count += 1

    print count
