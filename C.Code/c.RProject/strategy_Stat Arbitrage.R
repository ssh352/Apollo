# 统计套利策略

library(lubridate)
library(quantmod)
library(tseries)
library(WindR)

rm(list=ls())

list_contracts<-list(
  CU<-c('CU.SHF','W00030SPT.NM'),
  AL<-c('AL.SHF','W00031SPT.NM'),
  ZN<-c('ZN.SHF','W00032SPT.NM'),
  PB<-c('PB.SHF','W00033SPT.NM'),
  AG<-c('AG.SHF','W00034SPT.NM'),
  NI<-c('NI.SHF','W00042SPT.NM'),
  SN<-c('SN.SHF','W00041SPT.NM'),
  RB<-c('RB.SHF','W00035SPT.NM'),
  SM<-c('SM.CZC','W00027SPT.NM'),
  WR<-c('WR.SHF','W00036SPT.NM'),
  HC<-c('HC.SHF','W00037SPT.NM'),
  FU<-c('FU.SHF','W00038SPT.NM'),
  BU<-c('BU.SHF','W00039SPT.NM'),
  RU<-c('RU.SHF','W00040SPT.NM'),
  M<-c('M.DCE','W00003SPT.NM'),
  Y<-c('Y.DCE','W00004SPT.NM'),
  A<-c('A.DCE','W00002SPT.NM'),
  B<-c('B.DCE','W00002SPT.NM'),
  P<-c('P.DCE','W00005SPT.NM'),
  C<-c('C.DCE','W00001SPT.NM'),
  J<-c('J.DCE','W00012SPT.NM'),
  V<-c('V.DCE','W00010SPT.NM'),
  I<-c('I.DCE','W00014SPT.NM'),
  BB<-c('BB.DCE','W00007SPT.NM'),
  FB<-c('FB.DCE','W00008SPT.NM'),
  SF<-c('SF.CZC','W00028SPT.NM'),
  RM<-c('RM.CZC','W00024SPT.NM'),
  JM<-c('JM.DCE','W00013SPT.NM'),
  JD<-c('JD.DCE','W00006SPT.NM'),
  CS<-c('CS.DCE','W00043SPT.NM'),
  SR<-c('SR.CZC','W00017SPT.NM'),
  CF<-c('CF.CZC','W00016SPT.NM'),
  OI<-c('OI.CZC','W00019SPT.NM'),
  ZC<-c('ZC.CZC','W00025SPT.NM'),
  FG<-c('FG.CZC','W00022SPT.NM'),
  TA<-c('TA.CZC','W00018SPT.NM'),
  MA<-c('MA.CZC','W00021SPT.NM'),
  WH<-c('WH.CZC','W00015SPT.NM'),
  PM<-c('PM.CZC','W00015SPT.NM'),
  RI<-c('RI.CZC','W00029SPT.NM'),
  LR<-c('LR.CZC','W00021SPT.NM'),
  JR<-c('JR.CZC','W00026SPT.NM'),
  RS<-c('RS.CZC','W00024SPT.NM')
)

char_startday<-'2009-01-01'
char_endday<-'2018-09-10'

# 获取交易日列表
w_tdays_data<-w.tdays(char_startday,char_endday,"TradingCalendar=SHFE")
char_calendar<-w_tdays_data[[2]]$DATETIME
num_calendar<-year(char_calendar)*10000+month(char_calendar)*100+day(char_calendar)

w.start()

# 获取基差价格比数据
get_matrix_basis<-function()
{
  matrix_basis<-matrix(NA,nrow=length(num_calendar),ncol=length(list_contracts)+1)
  matrix_basis[,1]<-num_calendar
  
  for(i in 1:length(list_contracts))
  {
    fields<-list_contracts[[i]][1]
    
    for(j in 2:length(list_contracts[[i]]))
    {
      fields<-paste(fields,',',list_contracts[[i]][j],sep = "")
    }
    
    w_wsd_data<-w.wsd(fields,"close",char_startday,char_endday)
    
    # R中有3种方式访问列表：[[索引]]、[[名称]]、列表名称$元素名称
    if(list_contracts[[i]][[1]]=='JD.DCE')
    {
      matrix_basis[,i+1]<-(w_wsd_data[[2]][[2]]-w_wsd_data[[2]][[3]]*1000)/w_wsd_data[[2]][[2]]
    }
    else
    {
      matrix_basis[,i+1]<-(w_wsd_data[[2]][[2]]-w_wsd_data[[2]][[3]])/w_wsd_data[[2]][[2]]
    }
    
    #===========================================================#
    # 均值回归测试
    # num_mean<-apply(matrix_basis,2,mean,na.rm=T)
    # num_mean[i+1]
    
    # 时间序列平稳性检验
    # vector_basis<-na.remove(matrix_basis[,i+1])
    # list_result = adf.test(vector_basis)
    # if(list_result[[4]][[1]] <= 0.01)
    # {
    #   print(list_contracts[[i]][[1]])
    # }
    #===========================================================#
  }
  
  return(matrix_basis)
}
# test_fun<-get_matrix_basis(1,2)

# 差除：类似差分相除的数据
diff_ratio<-function (x, lag = 1L, differences = 1L, ...) 
{
  ismat <- is.matrix(x)
  xlen <- if (ismat) 
    dim(x)[1L]
  else length(x)
  if (length(lag) != 1L || length(differences) > 1L || lag < 
      1L || differences < 1L) 
    stop("'lag' and 'differences' must be integers >= 1")
  if (lag * differences >= xlen) 
    return(x[0L])
  r <- unclass(x)
  i1 <- -seq_len(lag)
  if (ismat) 
    for (i in seq_len(differences)) r <- r[i1, , drop = FALSE] / 
    r[-nrow(r):-(nrow(r) - lag + 1L), , drop = FALSE]
  else for (i in seq_len(differences)) r <- r[i1] / r[-length(r):-(length(r) - 
                                                                     lag + 1L)]
  class(r) <- oldClass(x)
  r
}
# diff_ratio(matrix_test)

# 获取收益率比值
get_matrix_return<-function()
{
  matrix_price<-matrix(NA,nrow=length(num_calendar),ncol=length(list_contracts))
  matrix_return<-matrix(NA,nrow=length(num_calendar),ncol=length(list_contracts)+1)
  matrix_return[,1]<-num_calendar
  
  for(i in 1:length(list_contracts))
  {
    w_wsd_data<-w.wsd(list_contracts[[i]][1],"close",char_startday,char_endday)
    matrix_price[,i]<-w_wsd_data[[2]][[2]]
  }
  
  matrix_return[2:(length(num_calendar)),2:(length(list_contracts)+1)]<-diff_ratio(matrix_price[])
  
  return(matrix_return)
}
# matrix_test<-get_matrix_return()
  
#=================================回测模块=================================#
num_holdcycle<-5

matrix_indicate<-get_matrix_basis()
matrix_return<-get_matrix_return()

matrix_quantile<-matrix(NA,nrow=length(num_calendar),ncol=5)

for(i in seq(2,(length(num_calendar)-num_holdcycle),num_holdcycle))
{
  matrix_index_return<-matrix(NA,nrow=length(list_contracts),ncol=2)
  
  matrix_index_return[,1]<-1:length(list_contracts)
  matrix_index_return[,2]<-matrix_return[i,2:(length(list_contracts)+1)]
  
  index_return<-which(is.na(matrix_index_return[,2])!=1)
  
  # 如果指标和收益表格索引不一致，需要做一个一一对应关系
  # index_indicate<-which(matrix_indicate[,1] == matrix_return[i,1])
  
  matrix_index_indicate<-matrix(NA,nrow=length(list_contracts),ncol=2)
  
  matrix_index_indicate[,1]<-1:length(list_contracts)
  matrix_index_indicate[,2]<-matrix_indicate[i,2:(length(list_contracts)+1)]
  
  index_indicate<-which(is.na(matrix_index_indicate[,2])!=1)
  
  index_inter<-intersect(index_return, index_indicate)
  
  index_inter<-index_inter+1
  
  # 临时索引排序矩阵
  matrix_index_sort<-matrix(NA,nrow=length(index_inter),ncol=2)
  matrix_index_sort[,1]<-index_inter
  matrix_index_sort[,2]<-matrix_indicate[i,index_inter]
  
  matrix_index_sort<-matrix_index_sort[order(matrix_index_sort[,2],decreasing=T),]
  
  len_matrix<-nrow(matrix_index_sort)
  
  # 分层测试
  for(per in 1:5)
  {
    # 改进优化：为保证数据的全面性后续按冗余数组进行分层
    per_index<-matrix_index_sort[((per-1)*floor(len_matrix/5)+1):(per*floor(len_matrix/5)),1]
    
    matrix_cumprod<-apply(matrix_return[i:(i+4),per_index],2,cumprod)
    matrix_rbind<-rbind(1,matrix_cumprod)
    matrix_diff<-diff(matrix_rbind)
    
    matrix_quantile[i:(i+4),per]<-rowSums(matrix_diff)/ncol(matrix_diff)
  }
}

#=================================回测结果分析=================================#
# 按日期/按交易品种


#=================================收益曲线=================================#
matrix_quantile[which(is.na(matrix_quantile)==1)]<-0

plot(cumsum(matrix_quantile[1:nrow(matrix_quantile),1]),type='l',ylim=c(0,2),xaxt="n")
lines(cumsum(matrix_quantile[1:nrow(matrix_quantile),2]),type="l",col="red")
lines(cumsum(matrix_quantile[1:nrow(matrix_quantile),3]),type="l",col="blue")
lines(cumsum(matrix_quantile[1:nrow(matrix_quantile),4]),type="l",col="green")
lines(cumsum(matrix_quantile[1:nrow(matrix_quantile),5]),type="l",col="purple")
axis(1,at=1:length(num_calendar[1:length(num_calendar)]),labels=num_calendar[1:length(num_calendar)])

plot(cumsum(matrix_quantile[1:nrow(matrix_quantile),1]-matrix_quantile[1:nrow(matrix_quantile),2]),type='l',ylim=c(0,2),xaxt="n")
# lines(cumsum(matrix_quantile[1:nrow(matrix_quantile),2]),type="l",col="red")
# lines(cumsum(matrix_quantile[1:nrow(matrix_quantile),3]),type="l",col="blue")
# lines(cumsum(matrix_quantile[1:nrow(matrix_quantile),4]),type="l",col="green")
# lines(cumsum(matrix_quantile[1:nrow(matrix_quantile),5]),type="l",col="purple")
axis(1,at=1:length(num_calendar[1:length(num_calendar)]),labels=num_calendar[1:length(num_calendar)])
  
