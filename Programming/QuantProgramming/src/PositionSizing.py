import numpy as np

class _TargetInfo:
    """记录标的物的胜率与赔率"""
    def __init__(self, winProb, rateWin, rateLoss):
        self.winProb = winProb
        self.rateWin = rateWin
        self.rateLoss = rateLoss
        self.__positionSize = 1
    
    @property
    def DSize(self):
        """动态计算仓位规模"""
        first = self.winProb/self.rateLoss
        second = (1 - self.winProb)/self.rateWin
        return first - second
    
    @property
    def positionSize(self):
        return self.__positionSize
    
    @positionSize.setter
    def positionSize(self, value):
        self.__positionSize = value

class PositionSizing:
    """训练标的物并记录胜率与赔率"""
    def __init__(self, start_date, end_date):
        self.start_date = start_date
        self.end_date = end_date
        self.target = {}

    def __getitem__(self, key):
        return self.target[key]
    
    def train(self, ticker, asset):
        if asset == 'Bond':
            self.trainBond(ticker)
        elif asset == 'ETF':
            self.trainETF(ticker)
        elif asset == 'Fund':
            self.trainFund(ticker)
        elif asset == "Future":
            self.trainFuture(ticker)
        elif asset == "Option":
            self.trainOption(ticker)
        elif asset == 'Stock':
            self.trainStock(ticker)

    @staticmethod
    def calculatePWL(target):
        """计算胜率和盈亏比"""
        # 胜利次数
        winNum = np.where(target.data['Signal']*target.data['Market Return'] > 0, 1, 0).sum()
        # 行动次数
        actNum = np.where(target.data['Signal'] != 0, 1, 0).sum()
        # 胜率
        winProb = winNum / actNum
        # 平均盈利率
        rateWin = np.where(target.data["Strategy Return"] > 0, target.data["Strategy Return"], 0).mean()
        # 平均亏损率
        rateLoss = np.where(target.data["Strategy Return"] < 0, -target.data["Strategy Return"], 0).mean()
        return winProb, rateWin, rateLoss

    def trainBond(self, ticker):
        from investment import BondInvestment
        bond = BondInvestment(ticker)
        bond.fetch_data(self.start_date, self.end_date, 'bonds')
        bond.calculate_technical_indicators()
        bond.generate_signals()
        bond.backtest_strategy()
        winProb, rateWin, rateLoss = self.calculatePWL(bond)
        bondInfo = _TargetInfo(winProb, rateWin, rateLoss)
        self.target[ticker] = bondInfo
    
    def trainETF(self, ticker):
        from investment import ETFInvestment
        etf = ETFInvestment(ticker)
        etf.fetch_data(self.start_date, self.end_date, 'etfs')
        etf.calculate_technical_indicators()
        etf.generate_signals()
        etf.backtest_strategy()
        winProb, rateWin, rateLoss = self.calculatePWL(etf)
        etfInfo = _TargetInfo(winProb, rateWin, rateLoss)
        self.target[ticker] = etfInfo
    
    def trainFund(self, ticker):
        from investment import FundInvestment
        fund = FundInvestment(ticker)
        fund.fetch_data(self.start_date, self.end_date, 'funds')
        fund.calculate_technical_indicators()
        fund.generate_signals()
        fund.backtest_strategy()
        winProb, rateWin, rateLoss = self.calculatePWL(fund)
        fundInfo = _TargetInfo(winProb, rateWin, rateLoss)
        self.target[ticker] = fundInfo

    def trainFuture(self, ticker):
        from investment import FutureInvestment
        future = FutureInvestment(ticker)
        future.fetch_data(self.start_date, self.end_date, 'futures')
        future.calculate_technical_indicators()
        future.generate_signals()
        future.backtest_strategy()
        winProb, rateWin, rateLoss = self.calculatePWL(future)
        futureInfo = _TargetInfo(winProb, rateWin, rateLoss)
        self.target[ticker] = futureInfo

    def trainOption(self, ticker):
        from investment import OptionInvestment
        option = OptionInvestment(ticker)
        option.fetch_data(self.start_date, self.end_date, 'options')
        option.calculate_technical_indicators()
        option.generate_signals()
        option.backtest_strategy()
        winProb, rateWin, rateLoss = self.calculatePWL(option)
        optionInfo = _TargetInfo(winProb, rateWin, rateLoss)
        self.target[ticker] = optionInfo
    
    def trainStock(self, ticker):
        from investment import StockInvestment
        stock = StockInvestment(ticker)
        stock.fetch_data(self.start_date, self.end_date, 'stocks')
        stock.calculate_technical_indicators()
        stock.generate_signals()
        stock.backtest_strategy()
        winProb, rateWin, rateLoss = self.calculatePWL(stock)
        stockInfo = _TargetInfo(winProb, rateWin, rateLoss)
        self.target[ticker] = stockInfo

    