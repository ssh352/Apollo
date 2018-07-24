rm(list=ls())     # 清空历史变量
library(RODBC)    # RODBC是R与sql的接口
library(quantmod) # 调用quantmod
library(tseries)
library(lubridate)
library(plotrix)
library(stringr)
library(openxlsx)
JYDB <- odbcConnect("stock_research","chendan","chendan") 

secu_code <- read.csv("G:/research/all_code.csv",header = FALSE)
secu_code <- substr(secu_code$V1,2,7)
return_max <- as.matrix(read.csv("G:/research/return_max.csv",header = FALSE))
zz_re <- read.csv("G:/research/zz_re.csv",header = FALSE)
floatcap_max <- as.matrix(read.csv("G:/research/turnovercap_float.csv",header = FALSE))



company <- c()
innercode <- c()
for (i in 1:length(secu_code))
{
  str_name <- secu_code[i]
  sqlstr <- paste("SELECT [CompanyCode]
                  ,[InnerCode]
                  ,[SecuCode]
                  ,[ChiName]
                  ,[ChiNameAbbr]
                  FROM [JYDB].[dbo].[SecuMain]
                  where SecuCode like '",str_name,"'
                  and SecuMarket in (83,90)
                  and SecuCategory=1", sep="")
  Table11 <- sqlQuery(JYDB,sqlstr,as.is=c(F,T,rep(F,6)))
  company[i] <- Table11$CompanyCode
  innercode[i] <- Table11$InnerCode
}

sqlstr <- paste("SELECT [ID]
      ,[InfoPublDate]
                ,[InfoSource]
                ,[BulletinType]
                ,[CompanyCode]
                ,[EndDate]
                ,[IfAdjusted]
                ,[IfMerged]
                ,[AccountingStandards]
                ,[EnterpriseType]
                ,[CashEquivalents]
                ,[ClientDeposit]
                ,[TradingAssets]
                ,[BillReceivable]
                ,[DividendReceivable]
                ,[InterestReceivable]
                ,[AccountReceivable]
                ,[OtherReceivable]
                ,[AdvancePayment]
                ,[Inventories]
                ,[BearerBiologicalAssets]
                ,[DeferredExpense]
                ,[NonCurrentAssetIn1Year]
                ,[OtherCurrentAssets]
                ,[CAExceptionalItems]
                ,[CAAdjustmentItems]
                ,[TotalCurrentAssets]
                ,[HoldForSaleAssets]
                ,[HoldToMaturityInvestments]
                ,[InvestmentProperty]
                ,[LongtermEquityInvest]
                ,[LongtermReceivableAccount]
                ,[FixedAssets]
                ,[ConstructionMaterials]
                ,[ConstruInProcess]
                ,[FixedAssetsLiquidation]
                ,[BiologicalAssets]
                ,[OilGasAssets]
                ,[IntangibleAssets]
                ,[SeatCosts]
                ,[DevelopmentExpenditure]
                ,[GoodWill]
                ,[LongDeferredExpense]
                ,[DeferredTaxAssets]
                ,[OtherNonCurrentAssets]
                ,[NCAExceptionalItems]
                ,[NCAAdjustmentItems]
                ,[TotalNonCurrentAssets]
                ,[LoanAndAccountReceivables]
                ,[SettlementProvi]
                ,[ClientProvi]
                ,[DepositInInterbank]
                ,[RMetal]
                ,[LendCapital]
                ,[DerivativeAssets]
                ,[BoughtSellbackAssets]
                ,[LoanAndAdvance]
                ,[InsuranceReceivables]
                ,[ReceivableSubrogationFee]
                ,[ReinsuranceReceivables]
                ,[ReceivableUnearnedR]
                ,[ReceivableClaimsR]
                ,[ReceivableLifeR]
                ,[ReceivableLTHealthR]
                ,[InsurerImpawnLoan]
                ,[FixedDeposit]
                ,[RefundableDeposit]
                ,[RefundableCapitalDeposit]
                ,[IndependenceAccountAssets]
                ,[OtherAssets]
                ,[AExceptionalItems]
                ,[AAdjustmentItems]
                ,[TotalAssets]
                ,[ShortTermLoan]
                ,[ImpawnedLoan]
                ,[TradingLiability]
                ,[NotesPayable]
                ,[AccountsPayable]
                ,[STBondsPayable]
                ,[AdvanceReceipts]
                ,[SalariesPayable]
                ,[DividendPayable]
                ,[TaxsPayable]
                ,[InterestPayable]
                ,[OtherPayable]
                ,[AccruedExpense]
                ,[DeferredProceeds]
                ,[NonCurrentLiabilityIn1Year]
                ,[OtherCurrentLiability]
                ,[CLExceptionalItems]
                ,[CLAdjustmentItems]
                ,[TotalCurrentLiability]
                ,[LongtermLoan]
                ,[BondsPayable]
                ,[LongtermAccountPayable]
                ,[SpecificAccountPayable]
                ,[EstimateLiability]
                ,[DeferredTaxLiability]
                ,[OtherNonCurrentLiability]
                ,[NCLExceptionalItems]
                ,[NCLAdjustmentItems]
                ,[TotalNonCurrentLiability]
                ,[BorrowingFromCentralBank]
                ,[DepositOfInterbank]
                ,[BorrowingCapital]
                ,[DerivativeLiability]
                ,[SoldBuybackSecuProceeds]
                ,[Deposit]
                ,[ProxySecuProceeds]
                ,[SubIssueSecuProceeds]
                ,[DepositsReceived]
                ,[AdvanceInsurance]
                ,[CommissionPayable]
                ,[ReinsurancePayables]
                ,[CompensationPayable]
                ,[PolicyDividendPayable]
                ,[InsurerDepositInvestment]
                ,[UnearnedPremiumReserve]
                ,[OutstandingClaimReserve]
                ,[LifeInsuranceReserve]
                ,[LTHealthInsuranceLR]
                ,[IndependenceLiability]
                ,[OtherLiability]
                ,[LExceptionalItems]
                ,[LAdjustmentItems]
                ,[TotalLiability]
                ,[PaidInCapital]
                ,[CapitalReserveFund]
                ,[SurplusReserveFund]
                ,[RetainedProfit]
                ,[TreasuryStock]
                ,[OrdinaryRiskReserveFund]
                ,[ForeignCurrencyReportConvDiff]
                ,[UncertainedInvestmentLoss]
                ,[OtherReserves]
                ,[SpecificReserves]
                ,[SEExceptionalItems]
                ,[SEAdjustmentItems]
                ,[SEWithoutMI]
                ,[MinorityInterests]
                ,[OtherItemsEffectingSE]
                ,[TotalShareholderEquity]
                ,[LEExceptionalItems]
                ,[LEAdjustmentItems]
                ,[TotalLiabilityAndEquity]
                ,[SpecialFieldRemark]
                ,[UpdateTime]
                ,[JSID]
                ,[IfComplete]
                ,[LPreferStock]
                ,[LPerpetualDebt]
                ,[OtherEquityinstruments]
                ,[EPreferStock]
                ,[EPerpetualDebt]
                ,[OtherCompositeIncome]
                ,[HoldAndFSAssets]
                ,[HoldAndFSLi]
                ,[LongSalariesPay]
                ,[LongDeferIncome]
                FROM [JYDB].[dbo].[LC_BalanceSheetAll]
                where IfAdjusted=2
                and IfMerged=1
                and BulletinType=20
                order by InfoPublDate",  sep="")   
TableAll <- sqlQuery(JYDB,sqlstr,as.is = F) 
TableAll_time <- year(TableAll$InfoPublDate)*10000+month(TableAll$InfoPublDate)*100+
  day(TableAll$InfoPublDate)
  

tradeday <- read.csv("G:/research/tradeday.csv",header = FALSE)
tradeday <- tradeday$V1
tradeday_num <- c()
for (i in 1:length(tradeday))
{
  temp <- unlist(strsplit(as.character(tradeday[i]),split="/"))
  tradeday_num[i] <- as.numeric(temp[1])*10000+as.numeric(temp[2])*100+
    as.numeric(temp[3])
}

ret <- c()
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
  for (k in 1:nrow(compare_max))
  {
     sign_index <- which(company==compare_max[k,1])
     compare_max[k,4] <- log(sum(floatcap_max[(change_index[i]-50):change_index[i],sign_index]))
  }
  compare_max <- compare_max[which(is.na(compare_max[,2])!=1),]
  len <- nrow(compare_max)
  compare_max <- compare_max[order(compare_max[,2],decreasing=T),]
  compare_max <- compare_max[(ceiling(len*0.005)):(len-ceiling(len*0.005)),]

  max_index <- compare_max[1:50,1]
  temp_re_max <- return_max[(change_index[i]+1):(change_index[i+1]),match(max_index,company)]
  temp_re_max[which(is.na(temp_re_max)==1,arr.ind = T)] <- 0
  ret[(change_index[i]+1):(change_index[i+1])] <- rowSums(temp_re_max)/50
}
plot(cumsum(ret[1886:length(ret)]),type="l")
lines(cumsum(zz_re$V1[1886:length(zz_re$V1)]),type="l",col="red")
plot(cumsum(ret[1886:length(ret)]-zz_re$V1[1886:length(zz_re$V1)]),type="l")