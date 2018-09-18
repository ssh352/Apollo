Stock_COde<-list(
  CU<-c('CU','S0049493','S0029752','S0114144'),
  AL<-c('AL','S0049494','S0029756'),
  PB<-c('PB','S0167708','S0029764'),
  ZN<-c('ZN','S0049498','S0029760'),
  NI<-c('NI','S0213018','S0029772'),
  SN<-c('SN','S0213019','S0029768'),
  AU<-c('AU','S0049505','S0114145'),
  AG<-c('AG','S0174908','S0114146'),
  RU<-c('RU','S0049495'),
  RB<-c('RB','S0181750'),
  HC<-c('HC','S0110143'),
  A<-c('A','S5041136', 'S0117164'),
  BU<-c('BU','S0204821'),
  M<-c('M','S5041144'),
  Y<-c('Y','S5063139', 'S5028184'),
  P<-c('P','S5006381', 'S5029336'),
  C<-c('C','S0049491', 'S5063274', 'S5063275'),
  CS<-c('CS','S0243229'),
  L<-c('L','S0049492', 'S5446196'),
  PP<-c('PP','S0205112', 'S5471283') ,
  PVC<-c('PVC','S0180500', 'S5471265'),
  JM<-c('JM','S5125686', 'S5133418'),
  J<-c('J','S5120058', 'S5116629', 'S5116630', 'S5136709', 'S5133852'),
  I<-c('I','S0110152'),
  CF<-c('CF','S0049502', 'S0049517'),
  SR<-c('SR','S0049502', 'S0049517'),
  OI<-c('OI','S5041913', 'S0164192'),
  RM<-c('RM','S0182089'),
  ZC<-c('ZC','S5116621', 'S5118163', 'S5103725', 'S5103728'),
  MA<-c('MA','S5436526', 'S5436527'),
  PTA<-c('PTA','S0049499'),
  FG<-c('FG','S5914176')
)

startday<-'2009-01-01'
endday<-'2018-08-10'

#===========================获得日历日==================================#
w_tdays_data<-w.tdays(startday,endday,"Days=Alldays")
calendar_day<-w_tdays_data[[2]]$DATETIME
calendar_day<-year(calendar_day)*10000+month(calendar_day)*100+day(calendar_day)
#=======================================================================#


#根据生成
setwd("G:/R-Basic")

for(i in 1:length(Stock_COde))
{
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
  dayTmp<-w_edb_data[[2]]$DATETIME
  dayTmp<-year(dayTmp)*10000+month(dayTmp)*100+day(dayTmp)
  
  Matrix_TMP<-matrix(NA,nrow=length(calendar_day),ncol=(length(Stock_COde[[i]])))
  Matrix_TMP[,1]<-calendar_day;
  
  SameDayIndex<-match(dayTmp,calendar_day)
  for(j in 2:length(Stock_COde[[i]]))
  {
    tmpVector<-w_edb_data[[2]][[j]]
    Matrix_TMP[SameDayIndex,j]<-tmpVector;
  }
  
  #循环迭代
  for(j in 2:ncol(Matrix_TMP))
  {
    Matrix_TMP[,j]<-fillNA(Matrix_TMP[,j])
  }
  
  #写入矩阵
  #write.csv(Matrix_TMP,fileName)
  write.table(Matrix_TMP,fileName,sep = ",",col.names = F,row.names = F)
}

fillNA<-function(x)
{
  indexNA<-which(is.na(x)==1)
  for(i in 1:length(indexNA))
  {
    dataTmp<-x[1:indexNA[i]]
    index1<-which(is.na(dataTmp)!=1)
    if(length(index1)>0)
    {
      x[indexNA[i]]<-dataTmp[index1[length(index1)]]
    }
  }
  return (x)
}
