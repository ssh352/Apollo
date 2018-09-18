library(RODBC)  
library(quantmod) 
library(tseries)
library(lubridate)
library(plotrix)
library(stringr)
library(openxlsx)
library(xlsx)
rm(list=ls()) 
setwd("F:/Apollo/C.Code/c.RProject") 
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
coefficient_matrix <- as.matrix( read.csv("./data/系数表_XXL.csv",header=FALSE))
coefficient_matrix<-coefficient_matrix[2:6,]
coefficient_matrix<-gsub(",","",coefficient_matrix)
coefficient_matrix=apply(coefficient_matrix,2,as.numeric)
Inventory_Array<-array(NA,c(length(calendar_day),5,length(secu_Code)))
TradeRatio<-0.2
StartDay <- 20090101
EndDay<-20180608
filelist <- list.files("./data/库存原始数据")  
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
NPeriod<-30 
Matrix_TRatio<-matrix(NA,nrow=length(calendar_day),ncol=length(secu_Code))     
Matrix_HRatio<-matrix(NA,nrow=length(calendar_day),ncol=length(secu_Code))
for(i in 1:length(filelist))
{
  for(j in NPeriod:length(calendar_day))
  {
    Inventory_Last<-Inventory_Array[(j-NPeriod+1),,i]
    ValidDataIndex<-which(is.na(Inventory_Last)!=1 )
    Inventory_Last<-sum(Inventory_Array[(j-NPeriod+1),ValidDataIndex,i])
    Inventory_Now <-sum(Inventory_Array[j,ValidDataIndex,i])
    if(Inventory_Now>1e-5)
    {
      Matrix_TRatio[j,i]<-(Inventory_Now-Inventory_Last)/Inventory_Last
    }
  }
}
KReserve<-5 
KHolding<-5  
ncolNum<-TradeRatio*length(filelist)
quantile_max <- matrix(NA,nrow = length(trade_day),ncol = 5)
long_max <- matrix(NA,nrow = length(trade_day),ncol = ncolNum)
short_max <- matrix(NA,nrow = length(trade_day),ncol = ncolNum)
Return_Tmp<-matrix(NA,nrow =length(secu_Code),ncol = 2)
Ratio_Tmp<-matrix(NA,nrow =length(secu_Code),ncol = 2)
bench_mark_re <- c()
for(i in seq(KReserve+1,(length(trade_day)-KHolding), KHolding))
{
  if(trade_day[i]>(StartDay+NPeriod))
  {
    Return_Tmp<-matrix(NA,nrow =length(secu_Code),ncol = 2)
    Ratio_Tmp<-matrix(NA,nrow =length(secu_Code),ncol = 2)
    Mom_Tmp<-matrix(NA,nrow =length(secu_Code),ncol = 2)
    Mom_Tmp[,1]<-1:length(secu_Code)
    Mom_Tmp[,2]<-colSums(Matrix_Return[i-KReserve:i,])
    index_mom<-which(is.na(Mom_Tmp[,2])!=1)  
    Ratio_index<-which(calendar_day==trade_day[i])
    Ratio_Tmp[,1]<-(1:length(secu_Code))
    Ratio_Tmp[,2]<-Matrix_TRatio[Ratio_index,]
    index_stock<-which(is.na(Ratio_Tmp[,2])!=1)
    index_init<-intersect(index_mom,index_stock)
    len_init<-length(index_init)
    if(len_init<4)
    {
      bench_mark_re[(i+1):(i+KHolding)]<-0
      next
    }
    Ratio_index<-which(calendar_day==trade_day[i])
    Ratio_sort<-matrix(NA,nrow =length(index_init),ncol = 2)
    Ratio_sort[,1]<-index_init
    Ratio_sort[,2]<-Matrix_TRatio[Ratio_index,index_init]
    Ratio_sort<-Ratio_sort[order(Ratio_sort[,2],decreasing=F),]
    Sto_long_index<-Ratio_sort[1:floor(len_init/2),1]
    Sto_short_index<-Ratio_sort[((len_init-(floor(len_init/2)))+1):len_init,1]
    Mom_long<-matrix(NA,nrow =length(Sto_long_index),ncol = 2)
    Mom_short<-matrix(NA,nrow =length(Sto_short_index),ncol = 2)
    Mom_long[,1]<-Sto_long_index
    Mom_long[,2]<-Mom_Tmp[Sto_long_index,2]
    Mom_short[,1]<-Sto_short_index
    Mom_short[,2]<-Mom_Tmp[Sto_short_index,2]
    Mom_long<-Mom_long[order(Mom_long[,2],decreasing=F),]
    len_long<-length(Mom_long[,1])
    index_long<-Mom_long[1:floor(len_long/2),1]
    Mom_short<-Mom_short[order(Mom_short[,2],decreasing=T),]
    len_short<-length(Mom_short[,1])
    index_short<-Mom_short[1:floor(len_short/2),1]
    bench_re_Max_Long <- as.matrix(Matrix_Return[(i+1):(i+KHolding),index_long])
    bench_re_Max_Long[which(is.na(bench_re_Max_Long)==1,arr.ind = T)] <- 0
    bench_re_Max_Short <- as.matrix(Matrix_Return[(i+1):(i+KHolding),index_short])
    bench_re_Max_Short[which(is.na(bench_re_Max_Short)==1,arr.ind = T)] <- 0
    bench_mark_re[(i+1):(i+KHolding)] <- rowSums(bench_re_Max_Long)/ncol(bench_re_Max_Long)-rowSums(bench_re_Max_Short)/ncol(bench_re_Max_Short)
  }
}
bench_mark_re[which(is.na(bench_mark_re)==1,arr.ind = T)] <- 0
startIndex<-287
plot(cumsum(bench_mark_re[startIndex:length(bench_mark_re)]),type="l",ylim = c(-1.0,1.0))
axis(1,at=1:length(trade_day[startIndex:length(trade_day)]),labels=trade_day[startIndex:length(trade_day)])



