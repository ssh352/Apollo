library(WindR)
w.start()
rm(list=ls())
begin_day<-'2009-01-01'
end_day<-'2018-08-10'

w_tdays_data<-w.tdays(begin_day,end_day,"Days=Alldays")
cal_date<-length(w_tdays_data$Data$DATETIME)

CU<-c('S0049493','S0029752','S0114144')
AL<-c('S0049494','S0029756')

variety<-list(CU) # PB,ZN,NI,SN,AU,AG,RU,RB,HC,BU,A,M,Y,P,C,CS,L,PP,PVC,JM,J,I,CF,SR,OI,RM,ZC,MA,PTA)

for(temp in variety)
{
  w_edb_data<-w.edb(temp,begin_day,end_day,'Fill=Previous')
  
  print(mode(w_tdays_data$Data$DATETIME))
  print(mode(w_edb_data$Data$DATETIME))
  
  diff_date<-setdiff(w_tdays_data$Data$DATETIME),ymd_hms(w_edb_data$Data$DATETIME))
  
  # for(temp_item in temp)
  # {
  #   w_edb_data<-w.edb(temp_item,begin_day,end_day,'Fill=Previous')
  #   
  # }
}
  






CU<-c('S0049493','S0029752','S0114144')

'AL': ['S0049494', 'S0029756'], 'PB': ['S0167708', 'S0029764'],
'ZN': ['S0049498', 'S0029760'], 'NI': ['S0213018', 'S0029772'], 'SN': ['S0213019', 'S0029768'],
'AU': ['S0049505', 'S0114145'], 'AG': ['S0174908', 'S0114146'], 'RU': ['S0049495'],
'RB': ['S0181750'], 'HC': ['S0110143'], 'BU': ['S0204821'], 'A': ['S5041136', 'S0117164'],
'M': ['S5041144'], 'Y': ['S5063139', 'S5028184'], 'P': ['S5006381', 'S5029336'],
'C': ['S0049491', 'S5063274', 'S5063275'], 'CS': ['S0243229'], 'L': ['S0049492', 'S5446196'],
'PP': ['S0205112', 'S5471283'], 'PVC': ['S0180500', 'S5471265'], 'JM': ['S5125686', 'S5133418'],
'J': ['S5120058', 'S5116629', 'S5116630', 'S5136709', 'S5133852'], 'I': ['S0110152'],
'CF': ['S0049502', 'S0049517'], 'SR': ['S0049502', 'S0049517'], 'OI': ['S5041913', 'S0164192'],
'RM': ['S0182089'], 'ZC': ['S5116621', 'S5118163', 'S5103725', 'S5103728'],
'MA': ['S5436526', 'S5436527'], 'PTA': ['S0049499'], 'FG': ['S5914176']







w_edb_data<-w.edb('S0049499','2009-01-01','2018-08-10','Fill=Previous')
w_tdays_data<-w.tdays("2009-01-01","2018-08-10","Days=Alldays")

