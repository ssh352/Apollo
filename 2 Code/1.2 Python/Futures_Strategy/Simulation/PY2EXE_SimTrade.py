#!/usr/bin/env python  
# encoding: utf-8  

""" 
@version: v1.0 
@author: xiaxianba 
@license: Apache Licence  
@contact: scuxia@gmail.com 
@site: http://weibo.com/xiaxianba 
@software: PyCharm 
@file: PY2EXE_SimTrade.py 
@time: 2018/7/16 13:38 
@describe: 
"""

from distutils.core import setup
import py2exe

options = {"SimTrade_exe.py":{"compressed":1,
                              "optimize":2,
                              "bundle_files":1}}

setup(console=['SimTrade_exe.py'],
      options=options)