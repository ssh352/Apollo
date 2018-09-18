#!/usr/bin/env python  
# encoding: utf-8  

""" 
@version: v1.0 
@author: xiaxianba 
@license: Apache Licence  
@contact: scuxia@gmail.com 
@site: http://weibo.com/xiaxianba 
@software: PyCharm 
@file: uuid_gen.py 
@time: 2018/8/28 15:12 
@describe: 
"""

import random
import uuid


def gen_uuid():
    s = ""

    for num in range(32):
        n = random.randint(1,2)

        if n == 1:
            num_char = random.randint(0, 9)
            s += str(num_char)
        else:
            m = random.randint(97, 122)
            char_char = chr(m)
            s += char_char

    print s


if __name__ == "__main__":

    # s_uuid = str(uuid.uuid4())
    # print s_uuid
    #
    # l_uuid = s_uuid.split('-')
    # print l_uuid
    # s_uuid = ''.join(l_uuid)
    # print s_uuid

    for i in range(10):
        gen_uuid()

