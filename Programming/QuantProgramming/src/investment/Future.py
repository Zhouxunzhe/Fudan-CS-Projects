import sys
sys.path.append('.')
sys.path.append('..')
from src.investment.Investment import InvestmentClass
import akshare as ak
import matplotlib.pyplot as plt


class FuturesInvestment(InvestmentClass):
    def __init__(self, symbol, capital=100000):
        super().__init__(symbol)
        self.initial_capital = capital
        self.capital = capital
        self.position_size = 0
        self.active_position = False

    def fetch_from_akshare(self, start_date, end_date):
        self.data = ak.futures_zh_daily_sina(symbol=self.symbol)

    def calculate_technical_indicators(self):
        self.data['RSI'] = self.calculate_rsi(self.data['close'], 14)
        self.data['Volatility'] = self.data['close'].rolling(window=10).std()

    def generate_signals(self):
        self.data['Signal'] = 0
        self.data.loc[(self.data['RSI'] > 70) & (
                    self.data['Volatility'] < self.data['Volatility'].rolling(window=10).mean()), 'Signal'] = -1
        self.data.loc[(self.data['RSI'] < 30) & (
                    self.data['Volatility'] < self.data['Volatility'].rolling(window=10).mean()), 'Signal'] = 1
        self.data['Position'] = self.data['Signal'].diff()

    def calculate_rsi(self, prices, n=14):
        delta = prices.diff()
        gain = (delta.where(delta > 0, 0)).rolling(window=n).mean()
        loss = (-delta.where(delta < 0, 0)).rolling(window=n).mean()
        rs = gain / loss
        rsi = 100 - (100 / (1 + rs))
        return rsi

    def simulate_trading(self):
        self.data['Portfolio_value'] = self.capital
        for i, row in self.data.iterrows():
            if row['Position'] > 0 and self.capital > 1:
                # kelly_fraction = self.calculate_kelly_criterion()
                kelly_fraction = 1.0
                position = self.capital * kelly_fraction / row['close']
                self.capital -= position * row['close']
                self.position_size += position
                # print(f"buy: {self.capital}, {self.position_size}, {row['close']}")
            elif row['Position'] < 0:
                self.capital += self.position_size * row['close']
                self.position_size = 0
                # print(f"sell: {self.capital}, {self.position_size}, {row['close']}")
            self.data['Portfolio_value'] = self.data['Portfolio_value'].astype(float)
            self.data.at[i, 'Portfolio_value'] = self.capital + (self.position_size * row['close'])

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

    def backtest_strategy(self):
        self.calculate_technical_indicators()
        self.generate_signals()
        self.simulate_trading()
        return (self.data['Portfolio_value'].iloc[-1] / self.initial_capital,
                self.data['close'].iloc[-1] / self.data['close'].iloc[0])


# 使用示例
if __name__ == '__main__':
    future = FuturesInvestment('B0')  # 示例期货代码
    future.fetch_data('20200101', '20201231', 'futures')
    future.calculate_technical_indicators()
    future.generate_signals()
    strategy_return, market_return = future.backtest_strategy()
    print(f'策略最终回报: {strategy_return * 100:.2f}%')
    print(f'市场最终回报: {market_return * 100:.2f}%')
    # 绘制图表
    plt.figure(figsize=(10, 5))
    # 绘制资产总值
    plt.plot(future.data['date'], future.data['Portfolio_value'], label='Portfolio Value', marker='o')
    plt.xlabel('Date')
    plt.ylabel('Value')
    plt.title('Portfolio Value Over Time')
    plt.grid(True)
    # 绘制市场价格
    plt.twinx()  # 使用双坐标轴
    plt.plot(future.data['date'], future.data['close'], label='Market Price', color='red', linestyle='--', marker='x')
    plt.ylabel('Market Price')
    # 添加图例
    plt.legend(loc="upper left")
    plt.show()
