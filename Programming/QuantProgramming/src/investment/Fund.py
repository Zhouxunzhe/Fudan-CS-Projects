import akshare as ak
import sys
sys.path.append('.')
sys.path.append('..')
from src.investment.Investment import InvestmentClass
import matplotlib.pyplot as plt

class FundInvestment(InvestmentClass):
    def __init__(self, symbol, capital=100000):
        super().__init__(symbol)
        self.initial_capital = capital
        self.capital = capital
        self.position_size = 0

    def fetch_from_akshare(self, start_date, end_date):
        # 使用akshare的fund_etf_hist_sina获取基金数据
        self.data = ak.fund_etf_hist_sina(symbol=self.symbol)

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
        # 策略回测
        self.calculate_technical_indicators()
        self.generate_signals()
        self.simulate_trading()
        return (self.data['Portfolio_value'].iloc[-1] / self.initial_capital,
                self.data['close'].iloc[-1] / self.data['close'].iloc[0])

# 使用示例
if __name__ == '__main__':
    # print(ak.fund_etf_category_sina(symbol="LOF基金"))
    fund = FundInvestment('sz169105', capital=100000)  # 示例基金代码
    fund.fetch_data('20200101', '20201231', 'funds')
    fund.calculate_technical_indicators()
    fund.generate_signals()
    strategy_return, market_return = fund.backtest_strategy()
    print(f'策略最终回报: {strategy_return * 100:.2f}%')
    print(f'市场最终回报: {market_return * 100:.2f}%')

    # 绘制图表
    plt.figure(figsize=(10, 5))
    # 绘制资产总值
    plt.plot(fund.data['date'], fund.data['Portfolio_value'], label='Portfolio Value', marker='o')
    plt.xlabel('Date')
    plt.ylabel('Value')
    plt.title('Portfolio Value Over Time')
    plt.grid(True)
    # 绘制市场价格
    plt.twinx()  # 使用双坐标轴
    plt.plot(fund.data['date'], fund.data['close'], label='Market Price', color='red', linestyle='--', marker='x')
    plt.ylabel('Market Price')
    # 添加图例
    plt.legend(loc="upper left")
    plt.show()
