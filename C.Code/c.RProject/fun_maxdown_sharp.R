library(tseries)
library(zoo)
library(xts)
library(PerformanceAnalytics)

getmaxdown <- function (x) 
{
  if (NCOL(x) > 1) 
    stop("x is not a vector.")
  if (any(is.na(x))) 
    stop("NA in x.")
  cmaxx <- cummax(x) - x
  mdd <- max(cmaxx)
  to <- which(mdd == cmaxx)
  from <- double(NROW(to))
  for (i in 1:NROW(to)) from[i] <- max(which(cmaxx[1:to[i]] == 0))
  
  return(list(maxdrawdown = mdd, from = from, to = to))
}

# a<-c(50,100,150,50,200,50)
# getmaxdown(a)

getsharpbyday <- function(x)
{
  if (NCOL(x) > 1) 
    stop("x is not a vector.")
  if (any(is.na(x))) 
    stop("NA in x.")
  erp <- mean(x,trim=0,na.rm=TRUE)
  erpsd <- sd(x,na.rm=TRUE)
  
  sharp <- erp/erpsd*sqrt(252)
  
  return(sharp)
}

# a<-c(50,100,150,50,200,50)
# getsharpbyday(a)

getsharpbyyear <- function(x)
{
  if (NCOL(x) > 1) 
    stop("x is not a vector.")
  if (any(is.na(x))) 
    stop("NA in x.")
  erp <- mean(x,trim=0,na.rm=TRUE)
  erpsd <- sd(x,na.rm=TRUE)
  
  sharp <- erp/erpsd
  
  return(sharp)
}

# a<-c(50,100,150,50,200,50)
# getsharpbyyear(a)


