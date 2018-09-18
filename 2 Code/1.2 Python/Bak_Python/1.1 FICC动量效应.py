'''
交易策略：
  1.等权做多策略（基准）：买入所有满足条件的商品期货主力合约。
  2.横截面动量策略
    a.做多累计涨幅排名前20%的品种，做空累计涨幅排名后20%的品种。
    b.R=15,H=10
  3.展期收益率与期限结构策略
    a.展期收益率（度量现货溢价幅度）{问题1}
    b.做多展期收益率排名前20%的品种，做空展期收益率排名后20%的品种。
    c.H=20,TS=TS2(近月和主力)
  4.动量与期限结构相结合的策略
    a.动量策略换手率远高期限结构策略（交易费用比较跟图14上一致）
    b.因子构建
    
投资逻辑：
  1.横截面存在动量效应规律，即同一时间周期内，涨幅大的品种未来涨幅空间比涨幅小的品种更大，且具有连续性（宏观经济、政策不变的情况）。
  2.期货市场期限结构现象是常态，即不同月份合约价格连接起来要么是单边上涨，要么是单边下降。最终是一个扩散的状态，如果布局多空组合，最终是一个喇叭形态，
    为获取持续稳定的收益提供基础，本质上还是动量效应，但两者的相关性很低只有23%左右，所以可以看做独立的两个互不影响的因子，组合起来使用。
  3.交易的核心有两点：
    a.同时做多前20%，做空后20%的品种，一个是扩大收益，一个是为了消除系统性风险。
b.期限结构类型的选取，其一是期限结构类型的选取越平稳，越有规律，盈利的可能性和收益就越大；其二是换手率也是影响收益的一个因素
'''
# 名称：FICC动量效应策略
# 作者：xiaxb
# 时间：2017-09-12
# 平台：米筐(www.ricequant.com)

import datetime

# 全局变量：国内46个商品期货品种代码
commodity_list = ['CU','AL','ZN','PB','AU','AG','RB','WR','NI',
                  'FU','BU','RU','BB','FB','L','V','PP','SN',
                  'J','JM','I','M','Y','A','B','P','C','CS','HC',
                  'JD','RI','LR','JR','RS','OI','RM','SF','SM','SR',
                  'CF','ZC','FG','TA','MA','WH','PM']

sort_period = 10  # 排序期(R)：计算涨跌幅的时间窗口
hold_period = 10  # 持有期(H)：调仓周期

def init(context):
    
    context.bench_date = context.now
    context.pool_long = []
    context.pool_short = []

# before_trading此函数会在每天策略交易开始前被调用，当天只会被调用一次
def before_trading(context):
    pass

# 交易函数，每个时间周期调用一次
def handle_bar(context, bar_dict):
    
    # 1.如果没有仓位，则根据池子中品种下单
    if(context.future_account.cash == context.future_account.total_value):
        
        context.bench_date = context.now
        context.pool_long,context.pool_short = get_pool()
        
        for to_buy in context.pool_long:
            to_buy += '88'
            buy_open(to_buy,1)
            
        for to_sell in context.pool_short:
            to_sell += '88'
            sell_open(to_sell,1)
    
    tmp_date = context.bench_date
    for k in range(hold_period):
        term_date = get_next_trading_date(tmp_date)
        tmp_date = term_date
        
    if(context.now > term_date and context.future_account.cash != context.future_account.total_value):
        
        context.bench_date = context.now
        current_pool_long,current_pool_short = get_pool()
        
        # 2.先平仓，平掉已经不在池子中的品种
        close_long_list = list(set(context.pool_long).difference(set(current_pool_long)))
        close_short_list = list(set(context.pool_short).difference(set(current_pool_short)))
        
        for close_long in close_long_list:
            close_long += "88"
            sell_close(close_long,1)
            
        for close_short in close_short_list:
            close_short += "88"
            buy_close(close_short,1)
            
        # 3.再开仓，交易池子中新加的品种
        open_long_list = list(set(current_pool_long).difference(set(context.pool_long)))
        open_short_list = list(set(current_pool_short).difference(set(context.pool_short)))
        
        for open_long in open_long_list:
            open_long += "88"
            buy_open(open_long,1)
            
        for open_short in open_short_list:
            open_short += "88"
            sell_open(open_short,1)

# after_trading函数会在每天交易结束后被调用，当天只会被调用一次
def after_trading(context):
    pass

# 计算单个期货品种主力合约的涨跌幅
def cal_gain_rate(commodity, frequency):
    
    rate = 0.0
    commodity += '88'
    
    iPrice = history_bars(commodity, frequency, '1d', 'close').tolist()[0]
    tPrice = history_bars(commodity, frequency, '1d', 'close').tolist()[sort_period-1]
    
    rate = (tPrice - iPrice)/iPrice
    
    return rate
   
# 计算所有商品期货在同个时间段的涨幅 
def cal_all_rate(frequency):
    
    dicts = {}
    rate = 0.0
    
    for key in commodity_list:
        rate = cal_gain_rate(key, frequency)
        dicts[key] = rate
        
    return dicts
    
# 获取股票池品种
def get_pool():
    
    pool_long = []
    pool_short = []
    
    dicts = cal_all_rate(sort_period)
    pool_short += sorted(dicts.items(), key=lambda item:item[1])[:8]
    pool_long += sorted(dicts.items(), key=lambda item:item[1])[-8:]
    pool_list_long = dict(pool_long).keys()
    pool_list_short = dict(pool_short).keys()
    
    return pool_list_long,pool_list_short
    
