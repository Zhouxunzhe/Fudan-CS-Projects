import akshare as ak
import sys
sys.path.append('.')
sys.path.append('..')
from src.investment.Investment import InvestmentClass
import matplotlib.pyplot as plt

class ETFInvestment(InvestmentClass):
    def __init__(self, symbol, capital=100000):
        super().__init__(symbol)
        self.initial_capital = capital
        self.capital = capital
        self.position_size = 0

    def fetch_from_akshare(self, start_date, end_date):
        # 使用akshare的stock_zh_index_daily获取ETF指数数据
        self.data = ak.stock_zh_index_daily(symbol=self.symbol)

    def calculate_technical_indicators(self):
        # 计算MACD指标
        self.data['EMA12'] = self.data['close'].ewm(span=12, adjust=False).mean()
        self.data['EMA26'] = self.data['close'].ewm(span=26, adjust=False).mean()
        self.data['MACD'] = self.data['EMA12'] - self.data['EMA26']
        self.data['Signal'] = self.data['MACD'].ewm(span=9, adjust=False).mean()

    def generate_signals(self):
        # 生成交易信号
        self.data['Position'] = 0
        self.data.loc[self.data['MACD'] > self.data['Signal'], 'Position'] = 1
        self.data.loc[self.data['MACD'] < self.data['Signal'], 'Position'] = -1

    def simulate_trading(self):
        # 模拟交易
        self.data['Portfolio_value'] = self.capital
        for i, row in self.data.iterrows():
            if row['Position'] > 0 and self.capital > 1:
                position = self.capital / row['close']
                self.capital -= position * row['close']
                self.position_size += position
                # print(f"buy: {self.capital}, {self.position_size}, {row['close']}")
            elif row['Position'] < 0 and self.position_size > 0:
                self.capital += self.position_size * row['close']
                self.position_size = 0
                # print(f"sell: {self.capital}, {self.position_size}, {row['close']}")
            self.data['Portfolio_value'] = self.data['Portfolio_value'].astype(float)
            self.data.at[i, 'Portfolio_value'] = self.capital + (self.position_size * row['close'])

    def backtest_strategy(self):
        # 策略回测
        self.calculate_technical_indicators()
        self.generate_signals()
        self.simulate_trading()
        return (self.data['Portfolio_value'].iloc[-1] / self.initial_capital,
                self.data['close'].iloc[-1] / self.data['close'].iloc[0])

# 使用示例
if __name__ == '__main__':
    etf = ETFInvestment('sz399551', capital=100000)  # 示例ETF指数代码
    etf.fetch_data('2020-01-01', '2020-12-31', 'etfs')
    etf.calculate_technical_indicators()
    etf.generate_signals()
    strategy_return, market_return = etf.backtest_strategy()
    print(f'策略最终回报: {strategy_return * 100:.2f}%')
    print(f'市场最终回报: {market_return * 100:.2f}%')

    # 绘制图表
    plt.figure(figsize=(10, 5))
    # 绘制资产总值
    plt.plot(etf.data['date'], etf.data['Portfolio_value'], label='Portfolio Value', marker='o')
    plt.xlabel('Date')
    plt.ylabel('Value')
    plt.title('Portfolio Value Over Time')
    plt.grid(True)
    # 绘制市场价格
    plt.twinx()  # 使用双坐标轴
    plt.plot(etf.data['date'], etf.data['close'], label='Market Price', color='red', linestyle='--', marker='x')
    plt.ylabel('Market Price')
    # 添加图例
    plt.legend(loc="upper left")
    plt.show()
