variety<-c('CU.SHF', 'AL.SHF', 'ZN.SHF', 'PB.SHF', 'AU.SHF', 'AG.SHF', 'NI.SHF', 'SN.SHF', 'RB.SHF',
              'WR.SHF', 'HC.SHF', 'FU.SHF', 'BU.SHF', 'RU.SHF', 'M.DCE', 'Y.DCE', 'A.DCE', 'B.DCE', 
           'P.DCE', 'C.DCE', 'J.DCE', 'V.DCE', 'I.DCE', 'BB.DCE','FB.DCE', 'L.DCE', 'PP.DCE', 'JM.DCE',
           'JD.DCE','CS.DCE','CY.CZC', 'SR.CZC', 'CF.CZC', 'ZC.CZC', 'FG.CZC', 'TA.CZC', 'MA.CZC',
           'WH.CZC', 'PM.CZC', 'RI.CZC', 'LR.CZC', 'JR.CZC', 'RS.CZC', 'OI.CZC', 'RM.CZC', 'SF.CZC',
           'SM.CZC')

startday<-'2009-01-01'
endday<-'2018-08-10'

for(i in variety)
{
  fileName<-paste(i,'.csv',sep = "")
  w_wsd_data<-w.wsd(i,"volume",startday,endday)
  
  # write.csv(w_wsd_data[[2]],fileName)
  write.table(w_wsd_data[[2]],fileName,sep = ",",col.names = F,row.names = F)
}

