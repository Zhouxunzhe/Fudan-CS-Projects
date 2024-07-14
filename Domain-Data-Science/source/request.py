import requests
import hmac
import hashlib
import time
from config import base_url


def sign_request(params, api_secret):
    query_string = '&'.join(
        [f"{key}={value}" for key, value in params.items()])
    signature = hmac.new(api_secret.encode(
        'utf-8'), query_string.encode('utf-8'), hashlib.sha256).hexdigest()
    return signature


def send_signed_request(method, endpoint, api_key, api_secret, params=None):
    headers = {
        'X-MBX-APIKEY': api_key
    }

    if params is None:
        params = {}

    params['timestamp'] = int(time.time() * 1000)
    params['signature'] = sign_request(params, api_secret)

    url = base_url + endpoint

    if method == 'GET':
        response = requests.get(url, headers=headers, params=params)
    elif method == 'POST':
        response = requests.post(url, headers=headers, data=params)
    elif method == 'DELETE':  # Add this condition to handle the DELETE method
        response = requests.delete(url, headers=headers, params=params)
    else:
        raise ValueError('Invalid method')

    return response.json()
