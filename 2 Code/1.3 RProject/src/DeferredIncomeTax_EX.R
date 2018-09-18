rm(list=ls())     # 清空历史变量
library(RODBC)    # RODBC是R与sql的接口
library(quantmod) # 调用quantmod
library(tseries)
library(lubridate)
library(plotrix)
library(stringr)
library(openxlsx)
JYDB <- odbcConnect("stock_research","chendan","chendan") 

secu_code <- read.csv("F:/RProject/data/all_code.csv",header = FALSE)
secu_code <- substr(secu_code$V1,2,7)
return_max <- as.matrix(read.csv("F:/RProject/data/return_max.csv",header = FALSE))
zz_re <- read.csv("F:/RProject/data/zz_re.csv",header = FALSE)
floatcap_max <- as.matrix(read.csv("F:/RProject/data/turnovercap_float.csv",header = FALSE))

company <- c()
innercode <- c()
for (i in 1:length(secu_code))
{
  str_name <- secu_code[i]
  sqlstr <- paste("SELECT * FROM [JYDB].[dbo].[SecuMain]
                  where SecuCode like '",str_name,"'
                  and SecuMarket in (83,90)
                  and SecuCategory=1", sep="")
  Table11 <- sqlQuery(JYDB,sqlstr,as.is=c(F,T,rep(F,6)))
  company[i] <- Table11$CompanyCode
  innercode[i] <- Table11$InnerCode
}

sqlstr <- paste("SELECT *
                FROM [JYDB].[dbo].[LC_QFinancialIndexNew]
                where AccountingStandards=1
                and BulletinType=20
                and Mark=2
                order by InfoPublDate",  sep="")  
TableAll <- sqlQuery(JYDB,sqlstr,as.is = F) 
TableAll_time <- year(TableAll$InfoPublDate)*10000+month(TableAll$InfoPublDate)*100+
  day(TableAll$InfoPublDate)


tradeday <- read.csv("F:/RProject/data/tradeday.csv",header = FALSE)
tradeday <- tradeday$V1
tradeday_num <- c()
for (i in 1:length(tradeday))
{
  temp <- unlist(strsplit(as.character(tradeday[i]),split="/"))
  tradeday_num[i] <- as.numeric(temp[1])*10000+as.numeric(temp[2])*100+
    as.numeric(temp[3])
}

# 创建一个以全部日期数量为行，列数为5的矩阵
quantile_max <- matrix(NA,nrow = length(tradeday_num),ncol = 5)
# 创建从50到数组结束的每个20天的日期数组（包括数组最后一个元素）
change_index <- unique(c(seq(50,length(tradeday_num),20),length(tradeday_num)))
# 根据选择的间隔日期统计相依的交易数据
for (i in 1:(length(change_index)-1))
{
  sign_time <- tradeday_num[change_index[i]]
  # 获取财务发布时间小于遍历时间的所有单季财务指标数据
  temp_frame <- TableAll[which(TableAll_time<=sign_time),]
  # 创建以所有股票数量为行数，列数为7的矩阵
  compare_max <- matrix(0,nrow=length(secu_code),ncol = 7)
  # 把公司代码（非股票代码）当作第一列
  compare_max[,1] <- company
  for (j in 1:nrow(compare_max))
  {
    #==========================================================================#
    # 只是查询指定文件中的股票信息
    temp_frame_1 <- temp_frame[which(temp_frame$CompanyCode==compare_max[j,1]),]
    frame_date <- year(temp_frame_1$InfoPublDate)*10000+
      month(temp_frame_1$InfoPublDate)*100+day(temp_frame_1$InfoPublDate)
    
    final_index <- which(frame_date<=sign_time)[length(which(frame_date<=sign_time))]
    final_frame <- temp_frame_1[final_index,]
    if (length(final_frame$ID)!=0)
    {
      # 研究的因子
      compare_max[j,2] <- final_frame$ROE
      compare_max[j,3] <- year(final_frame$EndDate)*10000+month(final_frame$EndDate)*100+
        day(final_frame$EndDate)
    }else
    {
      compare_max[j,2] <- NA
      compare_max[j,3] <- NA
    }
  }
  
  for (kk in 1:nrow(compare_max))
  {
    # Q 获取KK行股票，根据市值列表，计算换仓日前30天的平均市值
    compare_max[kk,4] <- log(sum(floatcap_max[(change_index[i]-30+1):change_index[i],kk]))
  }
  compare_max[,5] <- scale(compare_max[,2])
  compare_max[,6] <- scale(compare_max[,4])
  # Q 剔除第2列是NA的行
  compare_max <- compare_max[which(is.na(compare_max[,2])!=1),]
  compare_max <- compare_max[which(compare_max[,5]<=4),]
  compare_max <- compare_max[which(is.na(compare_max[,6])!=1),]
  temp <- lm(compare_max[,5]~1+compare_max[,6]) 
  # Q 根据去中心化和标准化的市值、收益率计算残差
  compare_max[,7] <- residuals(temp)
  
  len <- nrow(compare_max)
  compare_max <- compare_max[order(compare_max[,7],decreasing=T),]
  
  for (kk in 1:5)
  {
    # Q 根据残差排序，把股票分成5等分
    max_index <- compare_max[((kk-1)*floor(len/5)+1):(kk*floor(len/5)),1]
    temp_re_max <- return_max[(change_index[i]+1):(change_index[i+1]),match(max_index,company)]
    # Q 剔除收益率矩阵中存在NA值得行
    temp_re_max[which(is.na(temp_re_max)==1,arr.ind = T)] <- 0
    ret <- rowSums(temp_re_max)/length(max_index)
    # 根据分等级完之后的股票计算收益率，并分别存储在各自列中
    quantile_max[(change_index[i]+1):(change_index[i+1]),kk] <- ret
  }
}
plot(cumsum(quantile_max[1500:nrow(quantile_max),1]),type="l",ylim = c(-0.2,1),xaxt="n")
lines(cumsum(quantile_max[1500:nrow(quantile_max),2]),type="l",col="red")
lines(cumsum(quantile_max[1500:nrow(quantile_max),3]),type="l",col="blue")
lines(cumsum(quantile_max[1500:nrow(quantile_max),4]),type="l",col="green")
lines(cumsum(quantile_max[1500:nrow(quantile_max),5]),type="l",col="purple")
axis(1,at=1:length(tradeday_num[1500:length(tradeday_num)]),labels=tradeday_num[1500:length(tradeday_num)])

plot(cumsum(quantile_max[51:nrow(quantile_max),1]),type="l",ylim = c(-0.5,2.6),xaxt="n")
lines(cumsum(quantile_max[51:nrow(quantile_max),2]),type="l",col="red")
lines(cumsum(quantile_max[51:nrow(quantile_max),3]),type="l",col="blue")
lines(cumsum(quantile_max[51:nrow(quantile_max),4]),type="l",col="green")
lines(cumsum(quantile_max[51:nrow(quantile_max),5]),type="l",col="purple")
lines(cumsum(zz_re$V1[51:length(zz_re$V1)]),type="l",col="yellow")
axis(1,at=1:length(tradeday_num[21:length(tradeday_num)]),labels=tradeday_num[21:length(tradeday_num)])

plot(cumsum(quantile_max[51:length(quantile_max[,1]),1]-zz_re$V1[51:length(zz_re$V1)]),type="l")