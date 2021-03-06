//+------------------------------------------------------------------+
//| MACD(Moving Average Convergence / Divergence):指数平滑移动平均线
//| 参数：12,26,9
//| 计算表达式：
//|   快线DIF:EMA(close，12) - EMA(close，26)
//|   慢线DEA:EMA(DIF,9)
//|   MACD柱：（DIF-DEA）*2
//+------------------------------------------------------------------+

#property indicator_buffers 6
#property indicator_separate_window
#property indicator_level1 0
#property indicator_color1 White
#property indicator_color2 Red
#property indicator_color3 Red
#property indicator_color4 Lime
#property indicator_color5 Yellow
#property indicator_color6 Blue

//---- buffers
double Buffer1[];
double Buffer2[];
double Buffer3[];
double Buffer4[];
double UP[];
double DO[];

extern int Fast = 12;
extern int Slow = 26;
extern int Signal = 9;
extern bool Alert_Switch=true;
static double SX;


//+------------------------------------------------------------------+
//| Custom indicator initialization function |
//+------------------------------------------------------------------+
int init()
  {
//---- indicators
   //IndicatorBuffers(3);
   SetIndexStyle(0,DRAW_LINE,0,1);
   SetIndexStyle(1,DRAW_LINE,0,1);
   SetIndexStyle(2,DRAW_HISTOGRAM,0,2);
   SetIndexStyle(3,DRAW_HISTOGRAM,0,2);
   SetIndexStyle(4,DRAW_ARROW);
   SetIndexStyle(5,DRAW_ARROW);
   SetIndexArrow(4,233);
   SetIndexArrow(5,234);

   SetIndexBuffer(0,Buffer1);
   SetIndexBuffer(1,Buffer2);
   SetIndexBuffer(2,Buffer3); 
   SetIndexBuffer(3,Buffer4); 
   SetIndexBuffer(4,UP);
   SetIndexBuffer(5,DO);


   IndicatorShortName("MACD("+Fast+","+Slow+","+Signal+")");
   SetIndexLabel(0,"MACD_MAIN");
   SetIndexLabel(1,"MACD_SIGNAL");
   SetIndexLabel(2,"MAIN-SIGNAL");
   SetIndexLabel(3,"MAIN-SIGNAL");
   SetIndexLabel(4,"BUY_SIGNAL");
   SetIndexLabel(5,"SELL-SIGNAL");
   IndicatorDigits(Digits+2);
//----
   return(0);
  }
//+------------------------------------------------------------------+
//| Custor indicator deinitialization function                       |
//+------------------------------------------------------------------+
int deinit()
{
return(0);
}
//+------------------------------------------------------------------+
//| Custom indicator iteration function                              |
//+------------------------------------------------------------------+
int start()
  {
   int limit,counted_bars=IndicatorCounted();
//---- check for possible errors
   if(counted_bars<0) return(-1);
//---- last counted bar will be recounted
   if(counted_bars>0) counted_bars--;
   limit=Bars-counted_bars;
   double B_Temp;
  //---- main loop
   for(int i=0; i<limit; i++)
     {
      Buffer1[i]=iMACD(NULL,0,Fast,Slow,Signal,PRICE_CLOSE,MODE_MAIN,i); 
      Buffer2[i]=iMACD(NULL,0,Fast,Slow,Signal,PRICE_CLOSE,MODE_SIGNAL,i); 
      B_Temp=Buffer1[i] - Buffer2[i];
      if (B_Temp>=0)
      {
        Buffer3[i]=B_Temp;
        Buffer4[i]=EMPTY_VALUE;
      }
      else
      {
        Buffer4[i]=B_Temp;
        Buffer3[i]=EMPTY_VALUE;
      }
     }
   for(i=0; i<limit; i++)
     {
      UP[i]=EMPTY_VALUE;
      DO[i]=EMPTY_VALUE;
      if (Buffer1[i]>Buffer2[i]  &&  Buffer1[i+1]<Buffer2[i+1]) 
      UP[i]=Buffer2[i];
      if (Buffer1[i]<Buffer2[i]  &&  Buffer1[i+1]>Buffer2[i+1]) 
      DO[i]=Buffer2[i];
      if (Buffer1[i]>Buffer2[i]  &&  Buffer1[i+1]==Buffer2[i+1]  &&  Buffer1[i+2]<Buffer2[i+2]) 
      UP[i]=Buffer2[i];
      if (Buffer1[i]<Buffer2[i]  &&  Buffer1[i+1]==Buffer2[i+1]  &&  Buffer1[i+2]>Buffer2[i+2]) 
      DO[i]=Buffer2[i];
     }
   if (Alert_Switch==true && Buffer1[0]>Buffer2[0]  &&  Buffer1[1]<Buffer2[1] && SX!=Time[0]) 
   {
     Alert(Symbol(),"  ",Period(),":","MACD金叉");
     SX=Time[0];
   }
   if (Alert_Switch==true && Buffer1[0]<Buffer2[0]  &&  Buffer1[1]>Buffer2[1] && SX!=Time[0]) 
   {
     SX=Time[0];
     Alert(Symbol(),"  ",Period(),":","MACD死叉");
   }
   if (Alert_Switch==true && Buffer1[0]>Buffer2[0]  &&  Buffer1[1]==Buffer2[1]  &&  Buffer1[2]<Buffer2[2] && SX!=Time[0]) 
   {
     SX=Time[0];
     Alert(Symbol(),"  ",Period(),":","MACD金叉");
   }
   if (Alert_Switch==true && Buffer1[0]<Buffer2[0]  &&  Buffer1[1]==Buffer2[1]  &&  Buffer1[2]>Buffer2[2] && SX!=Time[0]) 
   {
     SX=Time[0];
     Alert(Symbol(),"  ",Period(),":","MACD死叉");
   }
//----
   return(0);
  }
//+------------------------------------------------------------------+ 

