import time
from models.LSTM import price_lstm
from models.ML import swing
from models.ARIMA import price_arima
from models.RL import *

#  "minPrice": "0.00000100",
#  "maxPrice": "100000.00000000",
#  "tickSize": "0.00000100"


# symbol_info = get_symbol_info(SYMBOL)
# price_precision = int(symbol_info['quoteAssetPrecision'])
# print(price_precision) # 8

# tick_size = float(symbol_info['filters'][0]['tickSize'])
# print(tick_size)  # 0.01

# min_qty, max_qty, step_size = get_symbol_limits(symbol_info)
# print(min_qty, max_qty, step_size)  # 1e-05 9000.0 1e-05


if __name__ == '__main__':
    # swing('xgboost')
    price_lstm()
    # price_arima()
    # dqn()
    # autotrader = AutoTrader()
    # place_market_order(SYMBOL, 'SELL', 'SHORT', 5)
    # place_market_order(SYMBOL, 'BUY', 'LONG', 5)
    # while True:
    #     autotrader.work()
    #     time.sleep(INTERVAL)
