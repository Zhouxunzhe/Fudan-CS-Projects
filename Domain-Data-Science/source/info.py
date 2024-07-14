import requests


def get_symbol_info(symbol):
    url = f'https://api.binance.com/api/v3/exchangeInfo'
    response = requests.get(url)
    data = response.json()
    for asset in data['symbols']:
        if asset['symbol'] == symbol:
            return asset
    return None


def get_symbol_limits(symbol_info):
    filters = symbol_info['filters']
    for filter in filters:
        if filter['filterType'] == 'LOT_SIZE':
            min_qty = float(filter['minQty'])
            max_qty = float(filter['maxQty'])
            step_size = float(filter['stepSize'])
            return min_qty, max_qty, step_size
    return None, None, None
