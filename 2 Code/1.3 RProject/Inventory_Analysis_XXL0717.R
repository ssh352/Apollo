#===========================库存策略==================================================#
#模型：同比、环比
#时间：2018-07-10
#名称：库存+库存变化率横截面季节性策略
#描述：
#基本逻辑：库存高，未来价格下降概率更大；库存低，未来价格上升概率更大。
#          1.指标一：今年库存与往年库存水平比较；总量是上升还是下降。
#          2.指标二：当前月份环比数据与往年相同月份环比数据比较；也就是库存变化速度。
#=====================================================================================#

rm(list=ls())    
library(RODBC)  
library(quantmod) 
library(tseries)
library(lubridate)
library(plotrix)
library(stringr)
library(openxlsx)
library(xlsx)

#=======================设置工作路径=========================#
setwd("F:/Apollo/C.Code/c.RProject") 

#===================加载品种原始数据=========================#
inventory_return <-as.matrix( read.csv("./data/inventory_return_XXL.csv",header = FALSE))
inventory_ratio <- as.matrix( read.csv("./data/inventory_ratio_XXL.csv",header=FALSE))
tmp<-inventory_ratio[2:nrow(inventory_ratio)]
calendar_day <- year(tmp)*10000+month(tmp)*100+day(tmp)
tmp<-inventory_return[2:nrow(inventory_return)]
trade_day <- year(tmp)*10000+month(tmp)*100+day(tmp)
secu_Code<-inventory_return[1,2:ncol(inventory_ratio)]
Matrix_Return<-as.matrix(inventory_return[2:nrow(inventory_return),2:ncol(inventory_return)])
Matrix_Return=apply(Matrix_Return,2,as.numeric)
Matrix_Ratio<-as.matrix(inventory_ratio[2:nrow(inventory_ratio),2:ncol(inventory_ratio)])
Matrix_Ratio=apply(Matrix_Ratio,2,as.numeric)

#============================================================#
coefficient_matrix <- as.matrix( read.csv("./data/系数表_XXL.csv",header=FALSE))
coefficient_matrix<-coefficient_matrix[2:6,]
coefficient_matrix<-gsub(",","",coefficient_matrix)
coefficient_matrix=apply(coefficient_matrix,2,as.numeric)
#============================================================#

#============用Array来保存32个品种的所有库存数据=============#
Inventory_Array<-array(NA,c(length(calendar_day),5,length(secu_Code)))

#============================================================#
filelist <- list.files("./data/库存原始数据")  
StartDay <- 20090101
EndDay<-20180608
for(i in 1:length(filelist))
{
  fileName<-substr(filelist[i],1,(nchar(filelist[i])-5))
  index_tmp<-which(match(as.character(secu_Code),fileName)==1)
  
  new_tmp<-read.xlsx(file=paste("./data/库存原始数据/",filelist[i],sep=''),sheetIndex =1,encoding='UTF-8') #读取xlsx文件
  tradeday_tmp<-year(new_tmp[,1])*10000+month(new_tmp[,1])*100+day(new_tmp[,1])  
  
  #考虑到有些品种是否在20090101之前有数据
  if(tradeday_tmp[1]>StartDay)
  {
    startIndex<-which(match(calendar_day,tradeday_tmp[1])==1)
    EndIndex<-which(match(tradeday_tmp,EndDay)==1)
    
    for(j in 2:ncol(new_tmp))
    {
      Inventory_Array[startIndex:length(calendar_day),(j-1),index_tmp]<-new_tmp[1:EndIndex,j]
    }
  }
  else
  {
    startIndex<-which(match(tradeday_tmp,StartDay)==1)
    EndIndex<-which(match(tradeday_tmp,EndDay)==1)
    
    for(j in 2:ncol(new_tmp))
    {
      Inventory_Array[,(j-1),index_tmp]<-new_tmp[startIndex:EndIndex,j]
    }
  }
}

#====================交易量数据========================================#
volume_filelist <- list.files("./data/volume") 
volume_Array<-array(NA,c(length(trade_day),length(secu_Code)))

for(i in 1:length(volume_filelist))
{
  fileName<-substr(volume_filelist[i],1,(nchar(volume_filelist[i])-4))
  index_tmp<-which(match(as.character(secu_Code),fileName)==1)
  
  new_tmp<-read.csv(file=paste("./data/volume/",volume_filelist[i],sep=''),header=FALSE) #读取csv库存文件

  volume_Array[,index_tmp]<-new_tmp[,2]
  
  # for(j in 2:ncol(new_tmp))
  # {
  #   volume_Array[,(j-1),index_tmp]<-new_tmp[,j]
  # }
}


#=====================找到每列的持仓数据的不为零的索引并存储=================================#
Matrix_Index<-array(NA,c(1,5,length(filelist)))
for(i in 1:length(filelist))
{
  for(j in 1:5)
  {
    tmp<-which(is.na(Inventory_Array[,j,i])!=1)
    if(length(tmp)>0)
    {
      Matrix_Index[1,j,i]<-tmp[1]
    }
  }
}
#============================================================================================#

#==================================完成库存数据单位的统一====================================#
for(i in 1:length(filelist))
{
  for(j in 1:5)
  {
    indexTmp<-Matrix_Index[1,j,i]
    if(!is.na(indexTmp))
    {
      Inventory_Array[indexTmp:length(calendar_day),j,i]<-Inventory_Array[indexTmp:length(calendar_day),j,i]*coefficient_matrix[j,i]   
    }
  }
}

#==========================2018.07.18库存同比数据============================================#
TPeriod<-365
Matrix_TRatio<-matrix(NA,nrow=length(calendar_day),ncol=length(secu_Code))  #保存同比数据矩阵
for(i in 1:length(filelist))
{
  #=============思路：获取对比数据的个数，只有在个数相同的情况下，作比较===================#
  for(j in TPeriod:length(calendar_day))
  {
    Inventory_Last<-Inventory_Array[(j-TPeriod+1),,i]
    ValidDataIndex<-which(is.na(Inventory_Last)!=1 )
    Inventory_Last<-sum(Inventory_Array[(j-TPeriod+1),ValidDataIndex,i])
    # 问题：会不会存在NOW的某列数据没有的情况？
    Inventory_Now <-sum(Inventory_Array[j,ValidDataIndex,i])
    if(Inventory_Now>1e-5)
    {
      # Matrix_HRatio[j,i]<-(Inventory_Now-Inventory_Last)/Inventory_Now
      Matrix_TRatio[j,i]<-(Inventory_Now-Inventory_Last)/Inventory_Last
    }
  }
}

#==========================2018.07.18库存环比数据============================================#
HPeriod<-30
TPeriod<-365
Matrix_CHRatio<-matrix(NA,nrow=length(calendar_day),ncol=length(secu_Code))  #保存同比数据矩阵
Matrix_LHRatio<-matrix(NA,nrow=length(calendar_day),ncol=length(secu_Code)) #保存环比的同比数据矩阵
Matrix_DiffRatio<-matrix(NA,nrow=length(calendar_day),ncol=length(secu_Code)) #保存环比同比增量矩阵

for(i in 1:length(filelist))
{
  #=============思路：获取对比数据的个数，只有在个数相同的情况下，作比较===================#
  for(j in HPeriod:length(calendar_day))
  {
    CL_Inventory<-Inventory_Array[(j-HPeriod+1),,i]
    ValidDataIndex<-which(is.na(CL_Inventory)!=1 )
    CL_Inventory<-sum(Inventory_Array[(j-HPeriod+1),ValidDataIndex,i])
    # 问题：会不会存在NOW的某列数据没有的情况？
    CC_Inventory <-sum(Inventory_Array[j,ValidDataIndex,i])
    if(Inventory_Now>1e-5)
    {
      Matrix_CHRatio[j,i]<-(CC_Inventory-CL_Inventory)/CL_Inventory
    }
  }
}

for(i in 1:length(filelist))
{
  #=============思路：获取对比数据的个数，只有在个数相同的情况下，作比较===================#
  for(j in (HPeriod+TPeriod):length(calendar_day))
  {
    LL_Inventory<-Inventory_Array[(j-HPeriod-TPeriod+1),,i]
    ValidDataIndex<-which(is.na(LL_Inventory)!=1 )
    LL_Inventory<-sum(Inventory_Array[(j-HPeriod-TPeriod+1),ValidDataIndex,i])
    # 问题：会不会存在NOW的某列数据没有的情况？
    LC_Inventory <-sum(Inventory_Array[j-TPeriod,ValidDataIndex,i])
    if(Inventory_Now>1e-5)
    {
      Matrix_LHRatio[j,i]<-(LC_Inventory-LL_Inventory)/LL_Inventory
    }
  }
}

#库存环比的同比数据差值
Matrix_DiffRatio<-Matrix_CHRatio-Matrix_LHRatio
#==========================================================================================#


# #环比计算规则：从startday开始，计算N周期的环比数据；
# HPeriod<-30                  #环比周期
# TPeriod<-365                 #同比周期   
# Matrix_TRatio<-matrix(NA,nrow=length(calendar_day),ncol=length(secu_Code))     #同比矩阵
# Matrix_HRatio<-matrix(NA,nrow=length(calendar_day),ncol=length(secu_Code))     #环比矩阵
# for(i in 1:length(filelist))
# {
#   #=============思路：获取对比数据的个数，只有在个数相同的情况下，作比较===================#
#   for(j in NPeriod:length(calendar_day))
#   {
#     Inventory_Last<-Inventory_Array[(j-NPeriod+1),,i]
#     ValidDataIndex<-which(is.na(Inventory_Last)!=1 )
#     Inventory_Last<-sum(Inventory_Array[(j-NPeriod+1),ValidDataIndex,i])
#     Inventory_Now <-sum(Inventory_Array[j,ValidDataIndex,i])
#     if(Inventory_Now>1e-5)
#     {
#       Matrix_HRatio[j,i]<-(Inventory_Now-Inventory_Last)/Inventory_Now
#     }
#   }
# }
# 
# #===============计算环比的同比======================#
# for(i in 1:length(filelist))
# {
#   #=============================================#
#   for(j in (NPeriod1+NPeriod):length(calendar_day))
#   {
#     # 问题：这里多加了一天
#     Inventory_Last<-Matrix_HRatio[(j-NPeriod1+1),i]
#     Inventory_Now <-Matrix_HRatio[j,i]
#     
#     # Matrix_TRatio[j,i]<-(Inventory_Now-Inventory_Last)/abs(Inventory_Last)
#     Matrix_TRatio[j,i]<-(Inventory_Now-Inventory_Last)
#   }
#   #=============================================#
# }




#============================================================================================#
KHolding<-10      #换仓周期

quantile_max <- matrix(NA,nrow = length(trade_day),ncol = 5)
Return_Tmp<-matrix(NA,nrow =length(secu_Code),ncol = 2)
Ratio_Tmp<-matrix(NA,nrow =length(secu_Code),ncol = 2)
Diff_Tmp<-matrix(NA,nrow =length(secu_Code),ncol = 2)
Vol_Tmp<-matrix(NA,nrow =length(secu_Code),ncol = 2)
bench_mark_re <- c()

for(i in seq(1,(length(trade_day)-KHolding), KHolding))
{
  if(trade_day[i]>=calendar_day[HPeriod+TPeriod])
  {
    Return_Tmp<-matrix(NA,nrow =length(secu_Code),ncol = 2)
    Ratio_Tmp<-matrix(NA,nrow =length(secu_Code),ncol = 2)
    
    #==========================过滤交易量==============================#
    Vol_Tmp[,1]<-1:length(secu_Code)
    Vol_Tmp[,2]<-volume_Array[i,]
    index_tmp0<-which(Vol_Tmp[,2]>10000)  
    
    #============判断当前的Return、和环比数据中的有效值，排序==========#
    Return_Tmp[,1]<-1:length(secu_Code)
    Return_Tmp[,2]<-Matrix_Return[i,]
    index_tmp1<-which(is.na(Return_Tmp[,2])!=1)  
    
    #==================================================================#
    Ratio_index<-which(calendar_day==trade_day[i])
    Ratio_Tmp[,1]<-(1:length(secu_Code))
    Ratio_Tmp[,2]<-Matrix_TRatio[Ratio_index,]
    index_tmp2<-which(is.na(Ratio_Tmp[,2])!=1)
    #==================================================================#
    
    #==================================================================#
    Diff_index<-which(calendar_day==trade_day[i])
    Diff_Tmp[,1]<-(1:length(secu_Code))
    Diff_Tmp[,2]<-Matrix_DiffRatio[Diff_index,]
    index_tmp3<-which(is.na(Diff_Tmp[,2])!=1)
    #==================================================================#
    
    mid_tmp<-intersect(intersect(index_tmp1,index_tmp2),index_tmp3)
    index_tmp<-intersect(index_tmp0,mid_tmp)
    
    # 问题：这里index_tmp返回的是具体列数，经过下面一句语句处理，会重新排序，返回的并非index_tmp
    # short_index<-intersect(which(Ratio_Tmp[index_tmp,2]>0),which(Diff_Tmp[index_tmp,2]>0))
    # long_index<-intersect(which(Ratio_Tmp[index_tmp,2]<0),which(Diff_Tmp[index_tmp,2]<0))
    short_index<-intersect(which(Ratio_Tmp[,2]>0),which(Diff_Tmp[,2]>0))
    long_index<-intersect(which(Ratio_Tmp[,2]<0),which(Diff_Tmp[,2]<0))
    #==================================================================#
    
    real_short_index<-intersect(index_tmp, short_index)
    real_long_index<-intersect(index_tmp, long_index)

    bench_re_Max_Long <- as.matrix(Matrix_Return[(i+1):(i+KHolding),real_long_index])
    bench_re_Max_Long[which(is.na(bench_re_Max_Long)==1,arr.ind = T)] <- 0
      
    bench_re_Max_Short <- as.matrix(Matrix_Return[(i+1):(i+KHolding),real_short_index])
    bench_re_Max_Short[which(is.na(bench_re_Max_Short)==1,arr.ind = T)] <- 0
    
    bench_mark_re[(i+1):(i+KHolding)] <- rowSums(bench_re_Max_Long)/length(real_long_index)-rowSums(bench_re_Max_Short)/length(real_short_index)
  }
}
#==================================计算夏普比率==========================================#
# bench_mean<-mean(bench_mark_re,trim=0,na.rm=TRUE)
# bench_sd<-sd(bench_mark_re,na.rm=TRUE)
# 
# bench_sharp<-bench_mean/bench_sd*sqrt(252)
# 
# print(bench_sharp)

#========================================作图============================================#
bench_mark_re[which(is.na(bench_mark_re)==1,arr.ind = T)] <- 0

startIndex<-487
plot(cumsum(bench_mark_re[startIndex:length(bench_mark_re)]),type="l",ylim = c(-0.3,1.5))
axis(1,at=1:length(trade_day[startIndex:length(trade_day)]),labels=trade_day[startIndex:length(trade_day)])

print(tail(cumsum(bench_mark_re[487:730]),-1))


plot(cumsum(quantile_max[startIndex:nrow(quantile_max),1]),type="l",ylim = c(-0.3,1.5),xaxt="n")
lines(cumsum(quantile_max[startIndex:nrow(quantile_max),2]),type="l",col="red")
lines(cumsum(quantile_max[startIndex:nrow(quantile_max),3]),type="l",col="blue")
lines(cumsum(quantile_max[startIndex:nrow(quantile_max),4]),type="l",col="green")
lines(cumsum(quantile_max[startIndex:nrow(quantile_max),5]),type="l",col="purple")
axis(1,at=1:length(trade_day[startIndex:length(trade_day)]),labels=trade_day[startIndex:length(trade_day)])
#================================================================================#


