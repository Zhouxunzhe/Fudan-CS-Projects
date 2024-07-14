# Tool kit for technical analysis and risk management
def calculate_SMA(data, window):
    return data['收盘'].rolling(window=window).mean()

def calculate_RSI(data, window):
    delta = data['收盘'].diff()
    up, down = delta.copy(), delta.copy()
    up[up < 0] = 0
    down[down > 0] = 0
    gain = up.rolling(window=window).mean()
    loss = down.abs().rolling(window=window).mean()
    RS = gain / loss
    return 100 - (100 / (1 + RS))


class RiskManagement:
    def __init__(self, initial_capital, risk_per_trade):
        self.initial_capital = initial_capital
        self.risk_per_trade = risk_per_trade

    def calculate_position_size(self, price, stop_loss):
        # 计算可用资金的风险百分比
        risk_amount = self.initial_capital * self.risk_per_trade
        position_size = risk_amount / (price - stop_loss)
        return position_size