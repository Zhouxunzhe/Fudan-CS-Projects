from config import api_key, api_secret, SYMBOL
from request import send_signed_request


def send_cancel_order(ORDER_ID):
    endpoint = '/fapi/v1/order'
    params = {
        'symbol': SYMBOL,
        'orderId': ORDER_ID,  # Update this line
    }
    response = send_signed_request(
        'DELETE', endpoint, api_key, api_secret, params)
    return response


def get_order(ORDER_ID):
    endpoint = '/fapi/v1/order'
    params = {
        'symbol': SYMBOL,
        'orderId': ORDER_ID,
    }
    response = send_signed_request(
        'GET', endpoint, api_key, api_secret, params)
    return response


def get_open_orders():
    endpoint = '/fapi/v1/openOrders'
    params = {
        'symbol': SYMBOL.upper(),
    }
    response = send_signed_request(
        'GET', endpoint, api_key, api_secret, params)
    return response


def get_all_orders():
    endpoint = '/fapi/v1/allOrders'
    params = {
        'symbol': SYMBOL,
    }
    response = send_signed_request(
        'GET', endpoint, api_key, api_secret, params)
    return response


def place_market_order(symbol, side, position_side, quantity):
    # Place a market order for BTCUSD futures
    endpoint = '/fapi/v1/order'
    params = {
        'symbol': symbol,
        'side': side,
        'type': 'MARKET',
        'quantity': quantity,
        # 'positionSide': position_side,
    }
    response = send_signed_request(
        'POST', endpoint, api_key, api_secret, params)
    print(response)


def place_limit_order(symbol, side, position_side, quantity, price):
    # Place a limit order for BTCUSD futures
    endpoint = '/fapi/v1/order'
    params = {
        'symbol': symbol,
        'side': side,
        'type': 'LIMIT',
        'timeInForce': 'GTC',  # Good Till Cancelled
        'quantity': quantity,
        'price': price,
        # 'positionSide': position_side,
    }
    response = send_signed_request(
        'POST', endpoint, api_key, api_secret, params)
    print("quantity:", quantity, "price:", price)
    # print("Request Params:", params)
    print(response)
    return response.get('orderId')
