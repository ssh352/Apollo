#===========================库存策略==========================#
#模型：同比、环比
#时间：2018-07-10
#
#
#
#=============================================================#

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
setwd("F:/Inventory_Factor") 

#===================加载品种原始数据=========================#
inventory_return <-as.matrix( read.csv("库存同比策略(1)/inventory_return.csv",header = FALSE))
inventory_ratio <- as.matrix( read.csv("库存同比策略(1)/inventory_ratio.csv",header=FALSE))
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
coefficient_matrix <- as.matrix( read.csv("库存同比策略(1)/系数表.csv",header=FALSE))
coefficient_matrix<-coefficient_matrix[2:6,]
coefficient_matrix<-gsub(",","",coefficient_matrix)
coefficient_matrix=apply(coefficient_matrix,2,as.numeric)
#============================================================#

#============用Array来保存32个品种的所有库存数据=============#
Inventory_Array<-array(NA,c(length(calendar_day),5,length(secu_Code)))

#============================================================#
filelist <- list.files("库存原始数据")  
StartDay <- 20090101
EndDay<-20180608
for(i in 1:length(filelist))
{
  fileName<-substr(filelist[i],1,(nchar(filelist[i])-5))
  index_tmp<-which(match(as.character(secu_Code),fileName)==1)
  
  new_tmp<-read.xlsx(file=paste("./库存原始数据/",filelist[i],sep=''),sheetIndex =1,encoding='UTF-8') #读取xlsx文件
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
#============================================================================================#

#====================================构建矩阵存储环比、同比数据==============================#
#环比计算规则：从startday开始，计算N周期的环比数据；
NPeriod<-365
Matrix_TRatio<-matrix(NA,nrow=length(calendar_day),ncol=length(secu_Code))     #同比矩阵
Matrix_HRatio<-matrix(NA,nrow=length(calendar_day),ncol=length(secu_Code))     #环比矩阵
for(i in 1:length(filelist))
{
  #=============思路：获取对比数据的个数，只有在个数相同的情况下，作比较===================#
  for(j in NPeriod:length(calendar_day))
  {
    Inventory_Last<-Inventory_Array[(j-NPeriod+1),,i]
    ValidDataIndex<-which(is.na(Inventory_Last)!=1 )
    Inventory_Last<-sum(Inventory_Array[(j-NPeriod+1),ValidDataIndex,i])
    Inventory_Now <-sum(Inventory_Array[j,ValidDataIndex,i])
    if(Inventory_Now>1e-5)
    {
      Matrix_HRatio[j,i]<-(Inventory_Now-Inventory_Last)/Inventory_Now
    }
  }
}
#============================================================================================#
KHolding<-15       #换仓周期
TradeRatio<-0.2   #持仓比例
ncolNum<-TradeRatio*length(filelist)
quantile_max <- matrix(NA,nrow = length(trade_day),ncol = 5)
long_max <- matrix(NA,nrow = length(trade_day),ncol = ncolNum)
short_max <- matrix(NA,nrow = length(trade_day),ncol = ncolNum)
Return_Tmp<-matrix(NA,nrow =length(secu_Code),ncol = 2)
Ratio_Tmp<-matrix(NA,nrow =length(secu_Code),ncol = 2)

bench_mark_re <- c()

for(i in seq(1,(length(trade_day)-KHolding), KHolding))
{
  if(trade_day[i]>(StartDay+NPeriod))
  {
    Return_Tmp<-matrix(NA,nrow =length(secu_Code),ncol = 2)
    Ratio_Tmp<-matrix(NA,nrow =length(secu_Code),ncol = 2)
    #============判断当前的Return、和环比数据中的有效值，排序==========#
    Return_Tmp[,1]<-1:length(secu_Code)
    Return_Tmp[,2]<-Matrix_Return[i,]
    index_tmp1<-which(is.na(Return_Tmp[,2])!=1)  
    
    #==================================================================#
    Ratio_index<-which(calendar_day==trade_day[i])
    Ratio_Tmp[,1]<-(1:length(secu_Code))
    Ratio_Tmp[,2]<-Matrix_HRatio[Ratio_index,]
    index_tmp2<-which(is.na(Ratio_Tmp[,2])!=1)
    #==================================================================#
    
    index_tmp<-intersect(index_tmp1,index_tmp2)
    Ratio_Tmp<-Ratio_Tmp[index_tmp,]
    Ratio_Tmp <-Ratio_Tmp[order(Ratio_Tmp[,2],decreasing=T),]
    #==================================================================#
    
   
    #=================构建组合，KHolding换仓，分层测试=================#
    TradeNum<-floor(nrow(Ratio_Tmp)*TradeRatio)
    len=nrow(Ratio_Tmp)
    
    if(TradeNum>0)
    {
      bench_Short_Index<-Ratio_Tmp[1:floor(len/5),1]
      bench_Long_Index<-Ratio_Tmp[(len-floor(len/5)+1):len,1]
      for(kk in 1:5)
      {
        max_index<-Ratio_Tmp[((kk-1)*floor(len/5)+1):(kk*floor(len/5)),1]
        temp_re_max <- Matrix_Return[(i+1):(i+KHolding),max_index]
        temp_re_max[which(is.na(temp_re_max)==1,arr.ind = T)] <- 0
        ret <- rowSums(temp_re_max)/length(max_index)
        quantile_max[(i+1):(i+KHolding),kk] <- ret
      }
      bench_re_Max_Long <- Matrix_Return[(i+1):(i+KHolding),bench_Long_Index]
      bench_re_Max_Long[which(is.na(bench_re_Max_Long)==1,arr.ind = T)] <- 0
      
      bench_re_Max_Short <- Matrix_Return[(i+1):(i+KHolding),bench_Short_Index]
      bench_re_Max_Short[which(is.na(bench_re_Max_Short)==1,arr.ind = T)] <- 0
      
      bench_mark_re[(i+1):(i+KHolding)] <- rowSums(bench_re_Max_Long)/length(bench_Long_Index)-rowSums(bench_re_Max_Short)/length(bench_Short_Index)
    }
  }
}

#================================作图============================================#
startIndex<-300
plot(cumsum(bench_mark_re[startIndex:length(bench_mark_re)]),type="l")
axis(1,at=1:length(trade_day[startIndex:length(trade_day)]),labels=trade_day[startIndex:length(trade_day)])

plot(cumsum(quantile_max[startIndex:nrow(quantile_max),1]),type="l",ylim = c(-0.3,1.5),xaxt="n")
lines(cumsum(quantile_max[startIndex:nrow(quantile_max),2]),type="l",col="red")
lines(cumsum(quantile_max[startIndex:nrow(quantile_max),3]),type="l",col="blue")
lines(cumsum(quantile_max[startIndex:nrow(quantile_max),4]),type="l",col="green")
lines(cumsum(quantile_max[startIndex:nrow(quantile_max),5]),type="l",col="purple")
axis(1,at=1:length(trade_day[startIndex:length(trade_day)]),labels=trade_day[startIndex:length(trade_day)])
#================================================================================#


