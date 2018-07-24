library(RODBC)    # RODBC是R与sql的接口
library(quantmod) # 调用quantmod
library(tseries)
library(lubridate)
library(plotrix)
library(stringr)
library(openxlsx)
JYDB <- odbcConnect("stock_research","xxb","xxb") 

sqlstr <- paste("SELECT [ID]
                ,[CompanyCode]
                ,[InfoPublDate]
                ,[EndDate]
                ,[ForcastType]
                ,[ForcastReason]
                ,[ResultStatement]
                ,[ForcastContent]
                ,[XGRQ]
                ,[JSID]
                ,[EGrowthRateFloor]
                ,[EGrowthRateCeiling]
                ,[EEarningFloor]
                ,[EEarningCeiling]
                ,[EProfitFloor]
                ,[EProfitCeiling]
                ,[EEPSFloor]
                ,[EEPSCeiling]
                ,[ForecastObject]
                ,[NPYOYConsistentForecast]
                ,[EGrowRateFloorC]
                ,[EGrowthRateCeilC]
                FROM [JYDB].[dbo].[LC_PerformanceForecast]
                where InfoPublDate>='2008-01-09'",  sep="")   
TableAll <- sqlQuery(JYDB,sqlstr,as.is = F) 
