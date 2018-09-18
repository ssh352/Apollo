# 加载依赖包
library(WindR)
library(RODBC)  
library(quantmod) 
library(tseries)
library(lubridate)
library(plotrix)
library(stringr)
library(openxlsx)
library(xlsx)

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

startday<-'2009-01-01'
endday<-'2018-09-16'

#===========================获得日历日==================================#
w_tdays_data<-w.tdays(startday,endday,"")
calendar_day<-w_tdays_data[[2]]$DATETIME
calendar_day<-year(calendar_day)*10000+month(calendar_day)*100+day(calendar_day)
#=======================================================================#

#根据生成
setwd("F:/RProject")

for(i in 1:length(Stock_COde))
{
  matrix_xianhuo<-matrix(NA,nrow=length(calendar_day),ncol=2)
  matrix_xianhuo[,1]<-calendar_day
  
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
  
  matrix_variety<-matrix(NA,nrow=nrow(w_edb_data$Data),ncol=2)
  data_day<-year(w_edb_data$Data$DATETIME)*10000+month(w_edb_data$Data$DATETIME)*100+day(w_edb_data$Data$DATETIME)
  matrix_variety[,1]<-data_day
  
  for(k in 2:length(Stock_COde[[i]]))
  {
    tmp_table[k]<-tmp_table[k]-Stock_weight[[i]][k-1]
  }
  
  tmp_table<-tmp_table[,-1]
  
  if(is.null(ncol(tmp_table)))
  {
    mean_table<-tmp_table
  }
  else
  {
    mean_table<-apply(tmp_table,1,mean)
  }
  
  length_mean_table<-length(mean_table)
  
  matrix_variety[(nrow(w_edb_data$Data)-length(mean_table)+1):nrow(w_edb_data$Data),2]<-mean_table
  
  # 最后一步处理
  for(m in 1:length(calendar_day))
  {
    object_index<-which(match(matrix_variety,calendar_day[m])==1)
    if(length(object_index)!=0)
    {
      matrix_xianhuo[m,2]<-matrix_variety[object_index,2]
    }
    
  }
  
  # dayTmp<-w_edb_data[[2]]$DATETIME
  # dayTmp<-year(dayTmp)*10000+month(dayTmp)*100+day(dayTmp)
  # 
  # Matrix_TMP<-matrix(NA,nrow=length(calendar_day),ncol=(length(Stock_COde[[i]])))
  # Matrix_TMP[,1]<-calendar_day;
  # 
  # # SameDayIndex<-match(dayTmp,calendar_day)
  # for(j in 2:length(Stock_COde[[i]]))
  # {
  #   tmpVector<-w_edb_data[[2]][[j]]
  #   Matrix_TMP[,j]<-tmpVector;
  # }
  
  #循环迭代
  for(n in 2:ncol(matrix_xianhuo))
  {
    matrix_xianhuo[,n]<-fillNA(matrix_xianhuo[,n])
  }
  
  #写入矩阵
  #write.csv(Matrix_TMP,fileName)
  # xianhuo<-as.matrix(tmp_table)
  
  write.table(matrix_xianhuo,fileName,sep = ",",col.names = F,row.names = F)
}

fillNA<-function(x)
{
  indexNA<-which(is.na(x)==1)
  if(length(indexNA)==0)
  {
    return (x)
  }
  
  
  for(i in 1:length(indexNA))
  {
    sumNA=0
    endValue=0
    dataTmp<-x[1:indexNA[i]]
    index1<-which(is.na(dataTmp)!=1)
    if(length(index1)>0)
    {
      for(j in 1:length(indexNA))
      {
        if(is.na(x[index1[length(index1)]+j])==1)
        {
          sumNA=sumNA+1
        }
        else
        {
          endValue=x[index1[length(index1)]+j]
          break
        }
        
        x[indexNA[i]]<-dataTmp[index1[length(index1)]]+(endValue-dataTmp[index1[length(index1)]])/sumNA
      }
      
      # x[indexNA[i]]<-dataTmp[index1[length(index1)]]+(endValue-dataTmp[index1[length(index1)]])/sumNA
    }
  }
  return (x)
}

