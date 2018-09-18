# 鍔犺浇渚濊禆鍖?
library(WindR)
library(RODBC)  
library(quantmod) 
library(tseries)
library(lubridate)
library(plotrix)
library(stringr)
library(openxlsx)
library(xlsx)
#library(DMwR)

Stock_COde<-list(
  CU<-c('CU','S0031724','S0031723'),
  AL<-c('AL','S0031730','S0031729'),
  PB<-c('PB','S0048086'),
  ZN<-c('ZN','S0048087','S0105514'),
  NI<-c('NI','S0105517','S0048090'),
  # SN<-c('SN','S0213019','S0029768'),
  # AU<-c('AU','S0049505','S0114145'),
  # AG<-c('AG','S0174908','S0114146'),
  RU<-c('RU','S5016816','S5016817','S5016818','S5438342','S5438343','S5438344'),
  RB<-c('RB','S0033213','S5704770','S0033232'),
  HC<-c('HC','S0033259','S0073209','S5708177'),
  A<-c('A','S5005846','S5005848','S5005849'),
  # BU<-c('BU','S0204821'),
  M<-c('M','S5006064','S5006032','S5006038','S5006042','S5006045','S5006057','S5006056','S5006030'),
  Y<-c('Y','S5005983','S5005982','S5005980','S5005979','S5005981'),
  P<-c('P','S5006006','S5006008','S5006009'),
  C<-c('C','S5006537','S5082168','S5005780','S5005775'),
  CS<-c('CS','S5006501','S5006502','S5006506','S5006505','S5028990'),
  L<-c('L','S5431301','S5431314','S5431333'),
  PP<-c('PP','S5443869','S5431318') ,
  # PVC<-c('PVC','S0180500', 'S5471265'),
  JM<-c('JM','S5112240','S5112232'),
  J<-c('J','S5118429','S5118197','S0033507'),
  I<-c('I','S0110172','S0110182','S0174655','S0167713'),
  CF<-c('CF','S0031714'),
  SR<-c('SR','S5023650','S5023651','S5023652','S5023659','S5023671','S5023678'),
  OI<-c('OI','S0142926','S0142924','S0142923','S0142921'),
  RM<-c('RM','S5005872','S5005866'),
  ZC<-c('ZC','S5101377','S5120089'),
  MA<-c('MA','S5443795','S5443797','S5443799','S5443800'),
  # PTA<-c('PTA','S0049499'),
  FG<-c('FG','S5914402','S5914392')
)

Stock_weight<-list(
  CU<-c(0,0),
  AL<-c(0,0),
  PB<-c(0),
  ZN<-c(0,0),
  NI<-c(0,0),
  # SN<-c('SN','S0213019','S0029768'),
  # AU<-c('AU','S0049505','S0114145'),
  # AG<-c('AG','S0174908','S0114146'),
  RU<-c(0,0,0,0,0,0),
  RB<-c(0,0,0),
  HC<-c(0,0,0),
  A<-c(0,0,0),
  # BU<-c('BU','S0204821'),
  M<-c(0,50,30,0,0,0,0,0),
  Y<-c(0,0,50,0,0),
  P<-c(0,-50,-100),
  C<-c(0,0,0,0),
  CS<-c(0,0,0,0,0),
  L<-c(-200,-200,-100),
  PP<-c(-200,-300) ,
  # PVC<-c('PVC','S0180500', 'S5471265'),
  JM<-c(0,0),
  J<-c(0,0,0),
  I<-c(0,0,0,0),
  CF<-c(0),
  SR<-c(0,0,100,-180,-150,-90),
  OI<-c(0,0,0,-200),
  RM<-c(0,100),
  ZC<-c(0,0),
  MA<-c(600,260,200,0),
  # PTA<-c('PTA','S0049499'),
  FG<-c(0,-130)
)

w.start()

startday<-'2010-01-01'
endday<-'2018-09-18'

#===========================鑾峰緱鏃ュ巻鏃?==================================#
w_tdays_data<-w.tdays(startday,endday,"")
trade_day<-w_tdays_data[[2]]$DATETIME
trade_day<-year(trade_day)*10000+month(trade_day)*100+day(trade_day)
#=======================================================================#

#============设置工作路径================#
setwd("F:/R-Quant/Xianba")

for(i in 1:length(Stock_COde))
{
  matrix_xianhuo<-matrix(NA,nrow=length(trade_day),ncol=2)
  matrix_xianhuo[,1]<-trade_day
  
  #=====================================#
  fileName<-paste(Stock_COde[[i]][1],'_Stock','.csv',sep = "")
  fields<-Stock_COde[[i]][2]
  if(length(Stock_COde[[i]])>2)
  {
    for(j in 3:length(Stock_COde[[i]]))
    {
      fields<-paste(fields,',',Stock_COde[[i]][j],sep = "")
    }
  }
  
  w_edb_data<-w.edb(fields,startday,endday,'Fill=Previous')
  
  tmp_table<-w_edb_data$Data
  
  for(k in 2:length(Stock_COde[[i]]))
  {
    tmp_table[k]<-tmp_table[k]-Stock_weight[[i]][k-1]
  }
  tmp_table1<-tmp_table[,-1]
  if(is.vector(tmp_table1))
  {
    mean_table<-tmp_table1
  }else
  {
    mean_table<-apply(tmp_table1,1,mean)
  }
  
  tmp_day<-tmp_table[,1]
  tmp_dayNum<-year(tmp_day)*10000+month(tmp_day)*100+day(tmp_day)
  
  tmpMatrix<-matrix(NA,nrow=length(trade_day),ncol=2)
  tmpMatrix[,1]<-c(1:length(trade_day))
  tmpMatrix[,2]<-trade_day
  
  indexTmp<-match(tmpMatrix[,2],tmp_dayNum)
  tmpMatrix[,2]<-indexTmp
  tmpMatrix<-tmpMatrix[which(is.na(tmpMatrix[,2])!=1),]
  #=====================================#
  matrix_xianhuo[tmpMatrix[,1],2]<-mean_table[tmpMatrix[,2]]
  
  #=======如果出现NA的值，那么插入======#
  matrix_xianhuo[,2]<-InsertNA(matrix_xianhuo[,2])
  #=====================================#
  
  write.table(matrix_xianhuo,fileName,sep = ",",col.names = F,row.names = F)
}

InsertNA<-function(x)
{
  indexNA<-which(is.na(x)==1)
  if(length(indexNA)==0)
  {
    return (x)
  }
  
  for(i in 1:length(x))
  {
    tmpValue<-x[i]
    if(is.na(tmpValue)==1)
    {
      #找到前面不为NA的BeforeIndex，找到后面不为NA的AfterIndex;
      tmpIndex<-which(is.na(x[1:(i-1)])!=1)
      tmpIndex1<-which(is.na(x[(i+1):length(x)])!=1)
      if(length(tmpIndex)>0)
      {
        if(length(tmpIndex1)>0)
        {
          BeforeIndex<-tmpIndex[length(tmpIndex)]
          AfterIndex<-(i+tmpIndex1[1])
          x[i]<-x[BeforeIndex]+(x[AfterIndex]-x[BeforeIndex])/(AfterIndex-BeforeIndex)
        }else{
          x[i]<-x[i-1]
        }
      }
    }
  }
  return (x)
}
