from orders import *

INTERVAL = 10
MARGIN = 3
DEPTH = 5
MAX_ORDER = 5
POSITION_LIMIT = 0.05
weight = [0.35, 0.25, 0.15, 0.10, 0.05]
cancel_price_diff = 0.00002
tick_size = 0.01

# TODO:don't know why only price_precision <= 1/2?  works
price_precision = 1
quantity_precision = 5


def get_position_size():
    endpoint = '/fapi/v2/positionRisk'
    params = {
        'symbol': SYMBOL,
    }
    response = send_signed_request(
        'GET', endpoint, api_key, api_secret, params)
    if response:
        position_size = float(response[0]['positionAmt'])
    else:
        position_size = 0
    return position_size


def get_account_information():
    endpoint = '/fapi/v2/account'
    response = send_signed_request('GET', endpoint, api_key, api_secret)
    return response


def get_account_trade_list():
    endpoint = '/fapi/v1/userTrades'
    params = {
        'symbol': SYMBOL,
    }
    response = send_signed_request(
        'GET', endpoint, api_key, api_secret, params)
    return response


# TODO: return []
# get_account_trade_list()

def get_ticker_price():
    """ get the latest ticker price for a specific symbol"""
    endpoint = '/fapi/v1/ticker/price'
    params = {
        'symbol': SYMBOL,
    }
    response = send_signed_request(
        'GET', endpoint, api_key, api_secret, params)
    return response


def get_klines_data(limit=1000):
    endpoint = '/fapi/v1/klines'
    params = {
        'symbol': SYMBOL,
        'limit': limit,
        # LIMIT参数（最大1500）   权重
        # [1,100)	            1
        # [100, 500)	        2
        # [500, 1000]	        5
        # > 1000	            10
        'interval': '1h',
        # Available intervals: '1m', '3m', '5m', '15m', '30m',
        # '1h', '2h', '4h', '6h', '8h', '12h', '1d', '3d', '1w', '1M'
    }
    response = send_signed_request(
        'GET', endpoint, api_key, api_secret, params)
    # [
    #     [
    # 0        1499040000000, // 开盘时间
    # 1        "0.01634790", // 开盘价
    # 2        "0.80000000", // 最高价
    # 3        "0.01575800", // 最低价
    # 4        "0.01577100", // 收盘价(当前K线未结束的即为最新价)
    # 5        "148976.11427815", // 成交量
    # 6        1499644799999, // 收盘时间
    # 7        "2434.19055334", // 成交额
    # 8        308, // 成交笔数
    # 9        "1756.87402397", // 主动买入成交量
    # 10       "28.46694368", // 主动买入成交额
    # 11       "17928899.62484339" // 请忽略该参数
    #      ]
    # ]
    return response


def get_order_book_depth():
    endpoint = '/fapi/v1/depth'
    params = {
        'symbol': SYMBOL,
        'limit': DEPTH,
    }
    response = send_signed_request(
        'GET', endpoint, api_key, api_secret, params)
    return response
