import akshare as ak
import sys
sys.path.append('.')
sys.path.append('..')
from src.investment.Investment import InvestmentClass
import matplotlib.pyplot as plt


class StockInvestment(InvestmentClass):
    def __init__(self, symbol, capital=100000):
        super().__init__(symbol)
        self.capital = capital
        self.initial_capital = capital
        self.position_size = 0
        self.active_trade = False

    def fetch_from_akshare(self, start_date, end_date):
        # 获取股票数据，调整为后复权价格
        self.data = ak.stock_zh_a_daily(symbol=self.symbol, start_date=start_date, end_date=end_date, adjust="hfq")

    def calculate_technical_indicators(self):
        # 计算技术指标：10日和50日均线
        self.data['SMA10'] = self.data['close'].rolling(window=10).mean()
        self.data['SMA50'] = self.data['close'].rolling(window=50).mean()

    def generate_signals(self):
        # 生成交易信号
        self.data['Signal'] = 0
        self.data.loc[self.data['SMA10'] > self.data['SMA50'], 'Signal'] = 1
        self.data.loc[self.data['SMA10'] < self.data['SMA50'], 'Signal'] = -1
        self.data['Position'] = self.data['Signal'].diff()

    def apply_stop_loss_and_take_profit(self):
        # 应用止损和止盈策略
        risk_per_trade = 0.02
        profit_target = 0.05
        self.data['Stop_loss'] = self.data['close'] * (1 - risk_per_trade)
        self.data['Take_profit'] = self.data['close'] * (1 + profit_target)

    def calculate_kelly_criterion(self):
        # 根据每日价格变动计算凯利准则的胜率和盈亏比
        self.data['Daily_Returns'] = self.data['close'].pct_change()
        winning_trades = self.data[self.data['Daily_Returns'] > 0]
        losing_trades = self.data[self.data['Daily_Returns'] < 0]
        if len(winning_trades) == 0 or len(losing_trades) == 0:
            # 防止除以零的情况
            return 0
        win_prob = len(winning_trades) / len(self.data.dropna(subset=['Daily_Returns']))
        win_loss_ratio = winning_trades['Daily_Returns'].mean() / abs(losing_trades['Daily_Returns'].mean())
        # 凯利公式：f = p - (q / b)
        return win_prob - ((1 - win_prob) / win_loss_ratio)

    def simulate_trading(self):
        # 交易仿真
        self.data['Portfolio_value'] = self.initial_capital
        for i, row in self.data.iterrows():
            if row['Position'] < 0 and self.capital > 1:
                # buy
                # kelly_fraction = self.calculate_kelly_criterion()
                kelly_fraction = 1.0
                position = self.capital * kelly_fraction / row['close']
                self.capital -= position * row['close']
                self.position_size += position
                # print(f"buy: {self.capital}, {self.position_size}, {row['close']}")
            elif row['Position'] > 0:
                # sell
                self.capital += self.position_size * row['close']
                self.position_size = 0
                # print(f"sell: {self.capital}, {self.position_size}, {row['close']}")
            self.data['Portfolio_value'] = self.data['Portfolio_value'].astype(float)
            self.data.at[i, 'Portfolio_value'] = self.capital + self.position_size * row['close']

    def backtest_strategy(self):
        # 策略回测
        self.calculate_technical_indicators()
        self.generate_signals()
        self.apply_stop_loss_and_take_profit()
        self.simulate_trading()
        return (self.data['Portfolio_value'].iloc[-1] / self.initial_capital,
                self.data['close'].iloc[-1] / self.data['close'].iloc[0])


# 使用示例
if __name__ == '__main__':
    stock = StockInvestment('sh600592')  # 示例股票代码
    stock.fetch_data('20200101', '20201231', 'stocks')
    strategy_return, market_return = stock.backtest_strategy()
    print(f'策略最终回报: {strategy_return * 100:.2f}%')
    print(f'市场最终回报: {market_return * 100:.2f}%')
    # 绘制图表
    plt.figure(figsize=(10, 5))
    # 绘制资产总值
    plt.plot(stock.data['date'], stock.data['Portfolio_value'], label='Portfolio Value', marker='o')
    plt.xlabel('Date')
    plt.ylabel('Value')
    plt.title('Portfolio Value Over Time')
    plt.grid(True)
    # 绘制市场价格
    plt.twinx()  # 使用双坐标轴
    plt.plot(stock.data['date'], stock.data['close'], label='Market Price', color='red', linestyle='--', marker='x')
    plt.ylabel('Market Price')
    # 添加图例
    plt.legend(loc="upper left")
    plt.show()
