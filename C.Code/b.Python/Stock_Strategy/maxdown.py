#!/usr/bin/env python  
# encoding: utf-8  

""" 
@version: v1.0 
@author: Xia Xianba
@license: Apache Licence  
@contact: scuxia@gmail.com
@site: https://weibo.com/xiaxianba/ 
@software: PyCharm 
@file: maxdown.py 
@time: 2018/6/29 18:58
"""


# 计算最大回撤率，首先要清楚的是最大回撤率的定义，以下是百度上的解释。
# 最大回撤率：在选定周期内任一历史时点往后推，产品净值走到最低点时的收益率回撤幅度的最大值。测试数据由网友提供。
def get_maxdown(list_net):

    maxdown = 0.0
    if type(list_net) != list:
        return 0

    for index in range(len(list_net)):
        for sub_index in range(index):
            max_net = max(list_net[:index])

            if float(max_net - list_net[index])/float(max_net) > maxdown:
                maxdown = float(max_net - list_net[index])/float(max_net)

    return maxdown

if __name__ == "__main__":

    list_test = [100, 200, 50, 300, 150, 100, 200]

    max_down = get_maxdown(list_test)
    print max_down
    # 根据测试数据，最终的结果为0.75，即3/4.

