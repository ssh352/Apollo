#!/usr/bin/env python  
# encoding: utf-8  

""" 
@version: v1.0 
@author: xiaxianba 
@license: Apache Licence  
@contact: scuxia@gmail.com 
@site: http://weibo.com/xiaxianba 
@software: PyCharm 
@file: simpledemo.py 
@time: 2017/11/6 14:41 
@describe: 
"""

from sklearn import tree

feature = [[178, 1], [155, 0], [177, 0], [165, 0], [169, 1], [160, 0]]
label = ['male', 'female', 'male', 'female', 'male', 'female']

clf = tree.DecisionTreeClassifier()
clf = clf.fit(feature, label)

print(clf.predict([[158, 0], [172, 0]]))
