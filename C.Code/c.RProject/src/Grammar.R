# 1.基本功能测试
for(x in 1:5) print(x^2)

i=1
repeat{
  print(i^2)
  i=i+1
  if(i>5)break
}
data<-read.csv("data/test.csv")
sal <- max(data$salary)
print(sal)

# 2.测试读取csv文件
rm(list=ls()) 

secu_code <- read.csv("test/test.csv",header = FALSE)
secu_code <- substr(secu_code$V1,2,7)

# 3.测试SQL语句查询
library(RODBC) 
rm(list=ls()) 

JYDB <- odbcConnect("stock_research","xxb","xxb") 
sqlstr <- paste("SELECT TOP 3 * FROM JYDB.dbo.SecuMain", sep="")

Table11 <- sqlQuery(JYDB,sqlstr,as.is=c(F,T,rep(F,6)))
company <- Table11$CompanyCode
innercode <- Table11$InnerCode

# 4.画图功能
rm(list=ls()) 
zz_re <- read.csv("F:/RProject/Fundamental/zz_re.csv",header = FALSE)
plot(cumsum(zz_re$V1[1:length(zz_re$V1)]),type="l",col="red")
lines(cumsum(zz_re$V1[1886:length(zz_re$V1)]),type="l",col="blue")

# 5.矩阵
rm(list=ls()) 
compare_max <- matrix(0,nrow=10,ncol = 4)
