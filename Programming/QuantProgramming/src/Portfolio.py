from TechnicalLib import RiskManagement

class InvestmentPortfolio:
    def __init__(self, initial_capital, sizing):
        self.initial_capital = initial_capital
        self.sizing = sizing
        self.investments = []
        self.risk_management = RiskManagement(initial_capital, 0.02)

    def add_investment(self, investment, capital, num=0):
        self.investments.append((investment, capital, num))

    def run(self, result=False, plot=False):
        for investment in self.investments:
            investment[0].run(result=result, plot=plot)

    def portfolio_performance(self):
        totalcapital = 0
        for inv in self.investments:
            invcapital = inv[1]
            invnum = inv[2]
            if 'Cumulative Strategy Returns' in inv[0].data.columns:
                for day in range(len(inv[0].data)):
                    if inv[0].data['Signal'].iloc[day] == 1:
                        buynum = int((invcapital * self.sizing[inv[0].ticker].positionSize) / inv[0].data['收盘'].iloc[day])
                        invnum += buynum
                        invcapital -= buynum * inv[0].data['收盘'].iloc[day]
                    elif inv[0].data['Signal'].iloc[day] == -1:
                        invcapital += invnum * inv[0].data['收盘'].iloc[day]
                        invnum = 0
            totalcapital += (invcapital + inv[0].data['收盘'].iloc[-1] * invnum)
        total_return = (totalcapital - self.initial_capital) / self.initial_capital
        print(f"Total Portfolio Performance: {total_return:.2%}")
            
    def reallocate_capital(self):
        # 根据每个投资的表现和风险重新分配资本
        # 这是一个示例，实际应用时应更复杂
        pass


