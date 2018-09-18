#!/usr/bin/env python  
# encoding: utf-8  

""" 
@version: v1.0 
@author: xiaxianba 
@license: Apache Licence  
@contact: scuxia@gmail.com 
@site: http://weibo.com/xiaxianba 
@software: PyCharm 
@file: Detail_list.py 
@time: 2017/10/24 10:25 
"""

import csv
import os
import datetime

DIRECTORY = "F:/Python/FICC_Contract/history/"
RECORD = "trade.csv"

for i in os.listdir(DIRECTORY):
    if os.path.isfile(os.path.join(DIRECTORY, i)):
        filename = os.path.join(DIRECTORY, i)

        with open(filename, "rb") as csvfile:
            reader = csv.reader(csvfile)
            for row in reader:

                with open(RECORD, 'ab+') as recfile:
                    writer = csv.writer(recfile)
                    writer.writerow(row)

                # if len(row) > 4:
                #     recordfile = open(RECORD, 'ab+')
                #
                #     if len(recordfile.read()) == 0:
                #         with open(RECORD, 'wb') as recfile:
                #             writer = csv.writer(recfile)
                #             writer.writerow(row)
                #     else:
                #         with open(RECORD, 'rb') as recfile1:
                #             rec_reader = csv.reader(recfile1)
                #
                #             for rec_row in rec_reader:
                #                 if datetime.datetime.strptime(row[3], '%Y-%m-%d %H:%M:%S') > datetime.datetime.strptime(rec_row[3], '%Y-%m-%d %H:%M:%S'):
                #                     with open(RECORD, 'ab+') as recfile2:
                #                         writer2 = csv.writer(recfile2)
                #                         writer2.writerow(row)
                #                         break
                # else:
                #     # print row[0]
