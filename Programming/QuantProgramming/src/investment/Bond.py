import akshare as ak
import sys
sys.path.append('.')
sys.path.append('..')
from src.investment.Investment import InvestmentClass
import matplotlib.pyplot as plt


class BondInvestment(InvestmentClass):
    def __init__(self, symbol, capital=100000):
        super().__init__(symbol)
        self.initial_capital = capital
        self.capital = capital
        self.position_size = 0

    def fetch_from_akshare(self, start_date, end_date):
        # 使用akshare的fund_etf_hist_sina获取基金数据
        from akshare.bond.cons import (
            zh_sina_bond_hs_hist_url,
        )
        import datetime
        import requests

        r = requests.get(
            zh_sina_bond_hs_hist_url.format(
                self.symbol, datetime.datetime.now().strftime("%Y_%m_%d")
            )
        )
        if r.status_code == 404:
            self.data = None
        else:
            self.data = ak.bond_zh_hs_daily(symbol=self.symbol)

    def calculate_technical_indicators(self):
        # 计算技术指标，例如移动平均线和波动率
        self.data['SMA20'] = self.data['close'].rolling(window=20).mean()
        self.data['Volatility'] = self.data['close'].rolling(window=20).std()

    def generate_signals(self):
        # 生成交易信号
        self.data['Signal'] = 0
        self.data.loc[(self.data['close'] > self.data['SMA20']) & (
                    self.data['Volatility'] < self.data['Volatility'].rolling(window=10).mean()), 'Signal'] = 1
        self.data.loc[(self.data['close'] < self.data['SMA20']) | (
                    self.data['Volatility'] > self.data['Volatility'].rolling(window=10).mean()), 'Signal'] = -1
        self.data['Position'] = self.data['Signal'].diff()

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
        self.calculate_technical_indicators()
        self.generate_signals()
        self.simulate_trading()
        return (self.data['Portfolio_value'].iloc[-1] / self.initial_capital,
                self.data['close'].iloc[-1] / self.data['close'].iloc[0])

# Using the example
if __name__ == '__main__':
    bond = BondInvestment('sh010107', capital=100000)  # Example bond symbol
    bond.fetch_data('20200101', '20201231', 'bonds')
    bond.calculate_technical_indicators()
    bond.generate_signals()
    strategy_return, market_return = bond.backtest_strategy()
    print(f'策略最终回报: {strategy_return * 100:.2f}%')
    print(f'市场最终回报: {market_return * 100:.2f}%')

    # 绘制图表
    plt.figure(figsize=(10, 5))
    # 绘制资产总值
    plt.plot(bond.data['date'], bond.data['Portfolio_value'], label='Portfolio Value', marker='o')
    plt.xlabel('Date')
    plt.ylabel('Value')
    plt.title('Portfolio Value Over Time')
    plt.grid(True)
    # 绘制市场价格
    plt.twinx()  # 使用双坐标轴
    plt.plot(bond.data['date'], bond.data['close'], label='Market Price', color='red', linestyle='--', marker='x')
    plt.ylabel('Market Price')
    # 添加图例
    plt.legend(loc="upper left")
    plt.show()
