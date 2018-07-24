# -*- coding: utf-8 -*-
"""
Created on Wed Oct 18 10:28:27 2017

@author: Administrator
"""

import os 
import sys
import csv
import datetime
import math
import copy



def getFileName(path):
    ''' 获取指定目录下的所有指定后缀的文件名 '''
    result=[]

    f_list = os.listdir(path)
    # print f_list
    for i in f_list:
        # os.path.splitext():分离文件名与扩展名
        if os.path.splitext(i)[1] == '.csv':
            result.append(i)
            
            
    return result


# csv文件中的数据格式：日期,交易标的,交易量,收盘价,复权因子,复权价格,展期收益率中间变量,展期收益率
def Data_collation(file_list):
    '''将数据整理成方便回测的格式'''
    all_data={}
    for name in file_list:
        if len(name)==14:
            variety=name[-6:-4]
        else:
            variety=name[-5]
        timelist_data={}
        with open(name, "rb") as csvfile:
            reader = csv.reader(csvfile)
            for line in reader:
                # timelist_data['2014/4/24'] =  交易标的,交易量,收盘价,复权因子,复权价格,展期收益率中间变量,展期收益率
                timelist_data[str2date(line[0])]=line[1:]
        # all_data['HC']= {'date':'info','date1':'info1'...}
        all_data[variety]=timelist_data

    return all_data
                
# 转换日期
def str2date(str_):
    if str_.find('-') != -1:
        a=str_.split('-')
        b=datetime.date(int(a[0]),int(a[1]),int(a[2]))
        return datetime.datetime.strptime(str(b),'%Y-%m-%d') 
    else:
        a=str_.split('/')
        b=datetime.date(int(a[0]),int(a[1]),int(a[2]))
        return datetime.datetime.strptime(str(b),'%Y-%m-%d')
    
    
def get_timelist(begin_date,end_date):
    '''获取交易日期'''
    from WindPy import *
    w.start()
    return w.tdays(begin_date, end_date, "").Data[0]

def get_long_short_group(date,data):
    '''计算多空组合'''
    
    result={}
    high_low={}
    high_low['H']=[]
    high_low['L']=[]
    for variety in data:
        if date in data[variety].keys():
            if data[variety][date][6] != 'N' and data[variety][date][5] != 'N' and data[variety][date][6] != '#NUM!' and data[variety][date][5] != '#NUM!':

                result[variety]=float(data[variety][date][6])
                
    rank_result=sorted(result.iteritems(),key=lambda d:d[1],reverse = True)
    import pdb;
    pdb.set_trace()
    half=int(len(rank_result)/float(2))
    for i in range(len(rank_result)):
        if i < half:
            high_low['H'].append(rank_result[i][0])
        else:
            high_low['L'].append(rank_result[i][0])
            
    for key in high_low:
        if key=='H':
            high_long={}
            for name in high_low[key]:
                high_long[name]=float(data[name][date][5])
        else:
            low_short={}
            for name in high_low[key]:
                low_short[name]=float(data[name][date][5])
                
    rank_high_long=sorted(high_long.iteritems(),key=lambda d:d[1],reverse = True)
    rank_low_short=sorted(low_short.iteritems(),key=lambda d:d[1],reverse = False)
    half_high=int(len(rank_high_long)/float(2))
    half_low=int(len(rank_low_short)/float(2))
    
    long_short={}
    long_short['L']=[]
    long_short['S']=[]
    for i in range(len(rank_high_long)):
        if i < half_high:
            long_short['L'].append(rank_high_long[i][0])
            
    for i in range(len(rank_low_short)):
        if i < half_low:
            long_short['S'].append(rank_low_short[i][0])


    return long_short

def get_long_short_price(data,long_short,date):
    long_short_price={}
    for key in long_short:
        if key == 'L':
            a={}      
            for variety in long_short[key]:
                b=[date,data[variety][date][0],float(data[variety][date][4])]
                a[variety]=[]
                a[variety].append(b)
            
            long_short_price[key]=a
        if key == 'S':
            a1={}      
            for variety in long_short[key]:
                b1=[date,data[variety][date][0],float(data[variety][date][4])]
                a1[variety]=[]
                a1[variety].append(b1)
            
            long_short_price[key]=a1
            
    return long_short_price
        
            
def update_record(record,long_short_price,date,data):
    if record=={}:
        record=copy.deepcopy(long_short_price)
    else:
        for key in record:
            for key1 in record[key]:
                if (key1 not in long_short_price[key].keys()) and (len(record[key][key1]) % 2 == 1):
                    if date not in data[key1].keys():   ###判断平仓时有无数据，没有将前面记录删除
                        record[key][key1].pop()
                            
                    else:
                        aa=[date,data[key1][date][0],float(data[key1][date][4])]
                        record[key][key1].append(aa)
                    
                elif (key1 in long_short_price[key].keys()) and (len(record[key][key1]) % 2 == 0):
                    aa=[date,data[key1][date][0],float(data[key1][date][4])]
                    record[key][key1].append(aa)
        for key in long_short_price:
            for key1 in long_short_price[key]:
                if key1 not in record[key].keys():
                    bb=[date,data[key1][date][0],float(data[key1][date][4])]
                    record[key][key1]=[]
                    record[key][key1].append(bb)
            
                    
    return record
                    


def s_trade(trade_day_list,data,H):
    j=0    #用来控制日期的选择
    record={}
    while True:
        print trade_day_list[j]
        long_short=get_long_short_group(trade_day_list[j],data)
        long_short_price=get_long_short_price(data,long_short,trade_day_list[j])
        
        record=update_record(record,long_short_price,trade_day_list[j],data)
        
        j=j+H
        if j > len(trade_day_list):
            break
        
    record_list={}                
    for key in record:
        cc={}
        for key1 in record[key]: 
            cc[key1]=[]
            for i in range(len(record[key][key1])):
                if i % 2 == 0 and i+1<len(record[key][key1]):
                    if key == 'L':
                        profit=(record[key][key1][i+1][2]-record[key][key1][i][2])/record[key][key1][i][2]
                    else:
                        #print key,key1,i,record[key][key1][i][1]
                        profit=(record[key][key1][i][2]-record[key][key1][i+1][2])/record[key][key1][i][2]
                    dd=[record[key][key1][i],record[key][key1][i+1],profit]
                    cc[key1].append(dd)
                elif i % 2 == 0 and i == (len(record[key][key1])-1):
                    dd=[record[key][key1][i]]
                    cc[key1].append(dd)
        
        
        record_list[key]=cc
        
    return record_list

def profit(record):
    profit_S_L={}
    profit_sum={}
    for key in record:
        aa=[]
        for key1 in record[key]:
            for item in record[key][key1]:
                if len(item)==3:
                    aa.append(item[2])
        
        profit_S_L[key]=aa
        profit_sum[key]=sum(aa)
    return profit_sum
        
   
def f_write(record):
    for key in record:
        for key1 in record[key]:
            with open(key+'_'+key1+".csv", "ab+") as csvfile:
                 writer = csv.writer(csvfile)
            
                 for item in record[key][key1]:
                     if len(item)==3:
                         
                         writer.writerow([item[0][0],item[0][1],item[0][2],item[1][0],item[1][1],item[1][2],item[2]])
                     else:
                         writer.writerow([item[0][0],item[0][1],item[0][2]])

if __name__ == "__main__":
    # import pdb;pdb.set_trace()
    file_list=getFileName('F:/Python/FICC_Contract/huice/')
    data=Data_collation(file_list)  
    trade_day_list=get_timelist('2010-1-04','2017-9-29')
    
    trade_record=s_trade(trade_day_list,data,5)
    profit_sum=profit(trade_record)
    f_write(trade_record)
    
    















