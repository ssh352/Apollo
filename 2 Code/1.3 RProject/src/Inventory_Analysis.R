# RODBC是R与sql的接口
library(RODBC)  
# 调用quantmod
library(quantmod) 
library(tseries)
library(lubridate)
library(plotrix)
library(stringr)
library(openxlsx)

# 清空历史变量
rm(list=ls())     

# # 获取文件行数
# get_file_row <- function(filename){
#   date_temp <- read.csv(filename)
#   total_row <- nrow(date_temp)
#   return(total_row)
# }
# x <- dim(inventory_ratio)
# y <- nrow(inventory_ratio)
# count_rows <- get_file_row("F:/RProject/data/inventory_return.csv")
# list_trade_day <- inventory_return$DATE
# print(typeof(list_trade_day))
# print(length(list_trade_day))

inventory_return <- read.csv("F:/RProject/data/inventory_return.csv")
inventory_ratio <- read.csv("F:/RProject/data/inventory_ratio.csv")

calendar_day <- year(inventory_ratio$DATE)*10000+month(inventory_ratio$DATE)*100+day(inventory_ratio$DATE)
trade_day <- year(inventory_return$DATE)*10000+month(inventory_return$DATE)*100+day(inventory_return$DATE)

cycle <- 20
ret <- c()
quantile_max <- matrix(NA,nrow = length(trade_day),ncol = 5)

long_max <- matrix(NA,nrow = length(trade_day),ncol = 8)
short_max <- matrix(NA,nrow = length(trade_day),ncol = 8)

# 修改持仓周期在这里修改
for(i in seq(1,length(trade_day), cycle))
{
  # 1.筛选return不是NA的数值进行比较
  mx_basic <- matrix(NA,nrow = (length(inventory_return[i,])-1),ncol = 2)
  mx_basic[,1] <- 1:(length(inventory_return[i,])-1)
  mx_basic[,2] <- as.matrix(inventory_return[i,])[2:length(inventory_return[i,])]
  mx_basic <- mx_basic[which(is.na(mx_basic[,2])!=1),]
  
  # 2.根据ratio进行
  max_index <- which(calendar_day==trade_day[i])
  temp_frame <- inventory_ratio[max_index,as.numeric(mx_basic[,1])+1]
  compare_max <- matrix(NA,nrow = (length(temp_frame)),ncol = 2)
  compare_max[,1] <- mx_basic[,1]
  compare_max[,2] <- as.matrix(temp_frame)[1:length(temp_frame)]
  compare_max <- compare_max[which(is.na(compare_max[,2])!=1),]
  compare_max <- compare_max[order(as.numeric(compare_max[,2]),decreasing=T),]
  # compare_max <- matrix(NA,nrow = (length(temp_frame)-1),ncol = 2)
  # compare_max[,1] <- 1:(length(temp_frame)-1)
  # compare_max[,2] <- as.matrix(temp_frame)[2:length(temp_frame)]
  # compare_max <- compare_max[which(is.na(compare_max[,2])!=1),]
  # compare_max <- compare_max[order(as.numeric(compare_max[,2]),decreasing=T),]
  
  len <- nrow(compare_max)
  
  for (layer in 1:5)
  {
    if(len<5)
      break
    if(i+4>length(trade_day))
      break
    # 3.根据五等分计算收益
    if(layer<5)
    {
      max_index <- compare_max[((layer-1)*floor(len/5)+1):(layer*floor(len/5)),1]
    }
    else
    {
      max_index <- compare_max[(len-(floor(len/5))+1):len,1]
    }
    # temp_return <- inventory_return[i:(i+4),as.numeric(max_index)+1]
    temp_return <- inventory_return[i:(i+cycle-1),as.numeric(max_index)+1]
    
    if (layer==1)
    {
      long_max[i,1] <- as.character(trade_day[i])
      long_max[i,2:(length(as.numeric(max_index)+1)+1)] <- as.character(as.numeric(max_index)+1)
    }
    
    if (layer==5)
    {
      short_max[i,1] <- as.character(trade_day[i])
      short_max[i,2:(length(as.numeric(max_index)+1)+1)] <- as.character(as.numeric(max_index)+1)
    }
    # temp_return[which(is.na(temp_return)==1, arr.ind=T)] <- 0
    ret <- rowSums(as.matrix(temp_return)/length(max_index))
    # ret <- rowSums(as.matrix(temp_return))

    # quantile_max[(trade_day[i]:trade_day[i+1]-1), layer] <- ret
    # 修改周期的时候需要相应修改这个范围
    quantile_max[(i:(i+cycle-1)), layer] <- ret
  }

  # df_return <- inventory_return[i:(i+4),]
  # mx_return <- as.matrix(df_return)[,as.numeric(compare_max[,1])+1]
  # total_profit <- sum(colSums(apply(mx_return,2,as.numeric)))
  # ret[i] <- total_profit
  # inventory_ratio[max_index,]
}

# ret[is.na(ret)] <- 0
# plot(cumsum(ret[1:length(ret)]),type="l")


long_max[which(is.na(long_max)==1)] <- ''
short_max[which(is.na(short_max)==1)] <- ''
write.csv(long_max, "./data/long_tradelist.csv")
write.csv(short_max, "./data/short_tradelist.csv")

quantile_max[which(is.na(quantile_max)==1)] <- 0

# plot(cumsum(quantile_max[1:length(quantile_max)]),type="l")
# plot(cumsum(quantile_max[488:nrow(quantile_max),5]-quantile_max[488:nrow(quantile_max),1]),type="l",ylim = c(-1.0,1), xaxt="n")
# axis(1,at=488:length(trade_day[488:length(trade_day)]),labels=trade_day[1:length(trade_day)])

plot(cumsum(quantile_max[488:nrow(quantile_max),5]-quantile_max[488:nrow(quantile_max),1]),type="l",ylim = c(-1.0,2), xaxt="n")
axis(1,at=1:length(trade_day[488:length(trade_day)]),labels=trade_day[488:length(trade_day)])
grid(nx=NA,ny=8,lwd=1,lty=2,col="black")

plot(cumsum(quantile_max[1:nrow(quantile_max),1]),type="l",ylim = c(-2.0,2), xaxt="n")
lines(cumsum(quantile_max[1:nrow(quantile_max),2]),type="l",col="red")
lines(cumsum(quantile_max[1:nrow(quantile_max),3]),type="l",col="blue")
lines(cumsum(quantile_max[1:nrow(quantile_max),4]),type="l",col="green")
lines(cumsum(quantile_max[1:nrow(quantile_max),5]),type="l",col="purple")
lines(cumsum(quantile_max[1:nrow(quantile_max),5]-quantile_max[1:nrow(quantile_max),1]),type="l",col="pink")
axis(1,at=1:length(trade_day[1:length(trade_day)]),labels=trade_day[1:length(trade_day)])


