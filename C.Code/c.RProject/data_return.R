# 加载依赖包
library(WindR)

# 清空历史变量
rm(list=ls()) 

# 设置获取数据时间
startday <- '2008-12-31'
endday <- Sys.Date()

# variety<-c('CU.SHF', 'AL.SHF', 'ZN.SHF', 'PB.SHF', 'AU.SHF', 'AG.SHF', 'NI.SHF', 'SN.SHF', 'RB.SHF',
#            'WR.SHF', 'HC.SHF', 'FU.SHF', 'BU.SHF', 'RU.SHF', 'M.DCE', 'Y.DCE', 'A.DCE', 'B.DCE', 
#            'P.DCE', 'C.DCE', 'J.DCE', 'V.DCE', 'I.DCE', 'BB.DCE','FB.DCE', 'L.DCE', 'PP.DCE', 'JM.DCE',
#            'JD.DCE','CS.DCE','CY.CZC', 'SR.CZC', 'CF.CZC', 'ZC.CZC', 'FG.CZC', 'TA.CZC', 'MA.CZC',
#            'WH.CZC', 'PM.CZC', 'RI.CZC', 'LR.CZC', 'JR.CZC', 'RS.CZC', 'OI.CZC', 'RM.CZC', 'SF.CZC',
#            'SM.CZC')

variety<-c('A.DCE','AG.SHF','AL.SHF','AU.SHF','BU.SHF','C.DCE','CF.CZC','CS.DCE','CU.SHF','FG.CZC',
           'HC.SHF','I.DCE','J.DCE','JM.DCE','L.DCE','M.DCE','MA.CZC','NI.SHF','OI.CZC','P.DCE',
           'PB.SHF','PP.DCE','TA.CZC','V.DCE','RB.SHF','RM.CZC','RU.SHF','SN.SHF','SR.CZC','Y.DCE',
           'ZC.CZC','ZN.SHF')
           
w.start()

w_tdays_data<-w.tdays(startday,endday)

nrows <- length(w_tdays_data[[2]]$DATETIME)
ncols <- length(variety)+1

matrix_return <- matrix(NA,nrow=nrows,ncol=ncols)  
matrix_return[,1] <- year(w_tdays_data[[2]]$DATETIME)*10000+month(w_tdays_data[[2]]$DATETIME)*100+day(w_tdays_data[[2]]$DATETIME)
matrix_rate <- matrix(NA,nrow=nrows,ncol=ncols)  
matrix_rate[,1] <- year(w_tdays_data[[2]]$DATETIME)*10000+month(w_tdays_data[[2]]$DATETIME)*100+day(w_tdays_data[[2]]$DATETIME)

for(i in 1:length(variety))
{
  w_wsd_data<-w.wsd(variety[i],"close",startday,endday)
  matrix_return[,i+1] <- w_wsd_data[[2]]$CLOSE
}

for(j in 2:ncol(matrix_return))
{
  for(k in 2:nrow(matrix_return))
  {
    if(is.na(matrix_return[k,j]) || is.na(matrix_return[k-1,j]))
    {
      matrix_rate[k,j] <- 1
    }
    else
    {
      matrix_rate[k,j] <- matrix_return[k,j]/matrix_return[k-1,j]
    }
  }
}

# 导出数据
write.table(matrix_rate,'matrix_rate.csv',sep = ",",col.names = F,row.names = F)


