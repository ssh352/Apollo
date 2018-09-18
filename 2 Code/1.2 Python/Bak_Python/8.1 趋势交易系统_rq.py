import talib

def init(context):
    context.s1 = ['600010.XSHG','000001.XSHE','002252.XSHE','601818.XSHG','601288.XSHG',
                  '000166.XSHE','601857.XSHG','600519.XSHG','600018.XSHG','601601.XSHG',
                  '000858.XSHE','002739.XSHE','600637.XSHG','600795.XSHG','601618.XSHG',
                  '002736.XSHE','600030.XSHG','601633.XSHG','002415.XSHE','600837.XSHG',
                  '600887.XSHG','000063.XSHE','600016.XSHG','002594.XSHE','601111.XSHG',
                  '000538.XSHE','601766.XSHG','002142.XSHE','002352.XSHE','601985.XSHG',
                  '001979.XSHE','300059.XSHE','600900.XSHG','600036.XSHG','601006.XSHG',
                  '600663.XSHG','600340.XSHG','601727.XSHG','601169.XSHG',
                  '002558.XSHE','601901.XSHG','601018.XSHG','601088.XSHG','601198.XSHG',
                  '600061.XSHG','601788.XSHG','601336.XSHG','600958.XSHG','601390.XSHG',
                  '601899.XSHG','601668.XSHG','600585.XSHG','600606.XSHG','601800.XSHG',
                  '600023.XSHG','601988.XSHG','000002.XSHE','002024.XSHE',
                  '601628.XSHG','601211.XSHG','600115.XSHG','600690.XSHG','601009.XSHG',
                  '601225.XSHG','002027.XSHE','600518.XSHG','000725.XSHE','600485.XSHG',
                  '600015.XSHG','000651.XSHE','002673.XSHE','601328.XSHG','601166.XSHG',
                  '000333.XSHE','600048.XSHG','000625.XSHE','601989.XSHG',
                  '002304.XSHE','601688.XSHG','600000.XSHG','600276.XSHG',
                  '000895.XSHE','600050.XSHG','601186.XSHG','000776.XSHE','601398.XSHG',
                  '600893.XSHG','600028.XSHG','600999.XSHG','600104.XSHG','300104.XSHE',
                  '600019.XSHG','601998.XSHG','000069.XSHE','601669.XSHG','601318.XSHG']

    context.SHORTPERIOD = 12
    context.LONGPERIOD = 26
    context.SMOOTHPERIOD = 9
    context.OBSERVATION = 100
    context.PRICEAVERAGE = 60
    context.VOLUMEAVERAGE = 5

def handle_bar(context, bar_dict):

    for stock in context.s1:

        prices = history_bars(stock,context.OBSERVATION,'15m','close')
        volume = history_bars(stock,context.OBSERVATION,'15m','volume')
        
        ma60 = talib.MA(prices,context.PRICEAVERAGE)
        volume5 = talib.MA(volume,context.VOLUMEAVERAGE)
    
        # 用Talib计算MACD取值，得到三个时间序列数组，分别为macd, signal 和 hist
        macd, signal, hist = talib.MACD(prices, context.SHORTPERIOD,
                                        context.LONGPERIOD, context.SMOOTHPERIOD)
    
        plot("macd", macd[-1])
        plot("macd signal", signal[-1])
        plot("macd hist", hist[-1])
        
        # macd 是长短均线的差值，signal是macd的均线，使用macd策略有几种不同的方法，我们这里采用macd线突破signal线的判断方法
    
        # 如果macd从上往下跌破macd_signal
    
        if (macd[-1] - signal[-1] < 0 and macd[-2] - signal[-2] > 0) or prices[-1] - ma60[-1] < 0:
            curPosition = context.portfolio.positions[stock].quantity
            if curPosition > 0:
                order_target_value(stock, 0)
    
        # 如果短均线从下往上突破长均线，为入场信号
        if macd[-1] - signal[-1] >= 0 and macd[-2] - signal[-2] <= 0 and hist[-1] >= 0 and prices[-1] - ma60[-1] >= 0 and volume[-1] - volume5[-1] >= 0:
            # 满仓入股
            curPosition = context.portfolio.positions[stock].quantity
            if curPosition <= 0:
                order_lots(stock, 1)
        
        
