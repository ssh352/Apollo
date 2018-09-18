################################################
# Date: 2018-5-14                              #
# Name: DeferredIncomTax.R                     #
# Author: Xia Xianba, Hexin inc,               # 
# Description: To Search Deferred income tax wi#
# -th Rate of return.                          #
################################################

# 1.Load third party packages
library(RODBC)    
library(quantmod) 
library(tseries)
library(lubridate)
library(plotrix)
library(stringr)
library(openxlsx)

# 2.Clear all variables
rm(list=ls()) 

# 3.Connect to a ODBC database
JYDB <- odbcConnect("stock_research","xxb","xxb") 

# 4.Read data from the csv file
secu_code <- read.csv("F:/RProject/data/all_code.csv",header = FALSE)
secu_code <- substr(secu_code$V1,2,7)
return_max <- as.matrix(read.csv("F:/RProject/data/return_max.csv",header = FALSE))
zz_re <- read.csv("F:/RProject/data/zz_re.csv",header = FALSE)
floatcap_max <- as.matrix(read.csv("F:/RProject/data/turnovercap_float.csv",header = FALSE))
tradeday <- read.csv("F:/RProject/data/tradeday.csv",header = FALSE)

company <- c()
innercode <- c()
for (i in 1:length(secu_code))
{
  str_name <- secu_code[i]
  sqlstr <- paste("SELECT [CompanyCode],[InnerCode],[SecuCode],[ChiName],[ChiNameAbbr]
                  FROM [JYDB].[dbo].[SecuMain] where SecuCode like '",str_name,"'
                  and SecuMarket in (83,90) and SecuCategory=1", sep="")
  
  # 5.get HS stock market A shares data
  Table11 <- sqlQuery(JYDB,sqlstr,as.is=c(F,T,rep(F,6)))
  company[i] <- Table11$CompanyCode
  innercode[i] <- Table11$InnerCode
}

sqlstr <- paste("SELECT * FROM [JYDB].[dbo].[LC_BalanceSheetAll]
                where IfAdjusted=2
                and IfMerged=1
                and BulletinType=20
                order by InfoPublDate",  sep="")   
TableAll <- sqlQuery(JYDB,sqlstr,as.is = F) 
# 6.get year,month,day data from DB and convert to a 8-digit number like 20180516
TableAll_time <- year(TableAll$InfoPublDate)*10000+month(TableAll$InfoPublDate)*100+
  day(TableAll$InfoPublDate)
  
# 7.assign the value of first column(default:V1) to tradeday
tradeday <- tradeday$V1
tradeday_num <- c()
for (i in 1:length(tradeday))
{
  # 8.get date data from csv file and convert to a 8-digit number
  temp <- unlist(strsplit(as.character(tradeday[i]),split="/"))
  tradeday_num[i] <- as.numeric(temp[1])*10000+as.numeric(temp[2])*100+
    as.numeric(temp[3])
}

ret <- c()
# 9.delete duplicate elements
change_index <- unique(c(seq(1885,length(tradeday_num),20),length(tradeday_num)))
for (i in 1:(length(change_index)-1))
{
  sign_time <- tradeday_num[change_index[i]]
  temp_frame <- TableAll[which(TableAll_time<=sign_time),]
  
  compare_max <- matrix(0,nrow=length(secu_code),ncol = 4)
  compare_max[,1] <- company
  for (j in 1:nrow(compare_max))
  {
    temp_frame_1 <- temp_frame[which(temp_frame$CompanyCode==compare_max[j,1]),]
    frame_date <- year(temp_frame_1$InfoPublDate)*10000+
      month(temp_frame_1$InfoPublDate)*100+day(temp_frame_1$InfoPublDate)
    final_index <- which(frame_date<=sign_time)[length(which(frame_date<=sign_time))]
    final_frame <- temp_frame_1[final_index,]
    if (length(final_frame$ID)!=0)
    {
      compare_max[j,2] <- final_frame$DeferredTaxAssets/final_frame$DeferredTaxLiability
      compare_max[j,3] <- year(final_frame$EndDate)*10000+month(final_frame$EndDate)*100+
        day(final_frame$EndDate)
    }else
    {
      compare_max[j,2] <- NA
      compare_max[j,3] <- NA
    }
  }
 
  compare_max <- compare_max[which(is.na(compare_max[,2])!=1),]
  compare_max <- compare_max[which(compare_max[,2]!=Inf),]
  len <- nrow(compare_max)
  # 10.Order according to the deferred income tax ratio
  compare_max <- compare_max[order(compare_max[,2],decreasing=T),]
  compare_max <- compare_max[(ceiling(len*0.005)):(len-ceiling(len*0.005)),]

  max_index <- compare_max[1:50,1]
  # 10.Get the class information under the specified row
  temp_re_max <- return_max[(change_index[i]+1):(change_index[i+1]),match(max_index,company)]
  temp_re_max[which(is.na(temp_re_max)==1,arr.ind = T)] <- 0
  # 11.The sum of all the lines and the average value
  ret[(change_index[i]+1):(change_index[i+1])] <- rowSums(temp_re_max)/50
}

# 12.Drawing the yield curve according to the rate of return
plot(cumsum(ret[1886:length(ret)]),type="l")
lines(cumsum(zz_re$V1[1886:length(zz_re$V1)]),type="l",col="red")
plot(cumsum(ret[1886:length(ret)]-zz_re$V1[1886:length(zz_re$V1)]),type="l")