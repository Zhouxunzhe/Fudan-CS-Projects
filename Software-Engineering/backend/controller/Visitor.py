from flask_cors import *
from flask import *
from controller import bluep
import json
from service import VisitorBehaviorObject
from model.models import (
    Goods,
    Shops
)
from utils.checker import (
    check_limit,
    check_shop_id,
    check_good_id
)
from utils.json_transfer import good_transfer

@bluep.route('/', methods=['GET', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def show_main_page():
    return json.dumps({})

@bluep.route('/home/shops', methods=['GET', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def show_shops():
    json_data = request.args.to_dict()
    json_data['limit'] = int(json_data['limit'])
    if check_limit(json_data) is False:
        return json.dumps({'isSuccess': False, 'result': None, 'message': "奇怪的事情发生了"})
    msg = VisitorBehaviorObject.show_shops(json_data)
    if msg['err'] is not None:
        return json.dumps({'isSuccess': False, 'result': msg['result'], 'message': msg['err']})
    
    result = []
    for item in msg['result']:
        result.append(item.to_dict())
    msg['result'] = result
    
    return json.dumps({'isSuccess': True, 'result': msg['result'], 'message': "成功"})

@bluep.route('/home/shophome', methods=['GET', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def show_shop_home():
    json_data = request.args.to_dict()
    json_data['shop_id'] = int(json_data['shop_id'])
    if check_shop_id(json_data) is False:
        return json.dumps({'isSuccess': False, 'result': None, 'message': "奇怪的事情发生了"})
    msg = VisitorBehaviorObject.show_shop_home(json_data)
    if msg['err'] is not None:
        return json.dumps({'isSuccess': False, 'result': msg['result'], 'message': msg['err']})
    msg['result'] = msg['result'].to_dict()
    return json.dumps({'isSuccess': True, 'result': msg['result'], 'message': "成功"})

@bluep.route('/home/allgoods', methods=['GET', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def show_all_good():
    msg = VisitorBehaviorObject.show_all_goods()
    if msg['err'] is not None:
        return json.dumps({'isSuccess': False, 'goods': msg['result'], 'message': msg['err']})
    result = []
    for item in msg['result']:
        temp = item.to_dict()
        good_transfer(temp)
        result.append(temp)
    msg['result'] = result
    return json.dumps({'isSuccess': True, 'goods': msg['result'], 'message': "成功"})

@bluep.route('/home/get_hotgoods', methods=['GET', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def show_hot_goods():
    json_data = request.args.to_dict()
    json_data['limit'] = int(json_data['limit'])
    if check_limit(json_data) is False:
        return json.dumps({'isSuccess': False, 'goods': None, 'message': "奇怪的事情发生了"})
    msg = VisitorBehaviorObject.show_hot_goods(json_data)
    if msg['err'] is not None:
        return json.dumps({'isSuccess': False, 'goods': msg['result'], 'message': msg['err']})
    result = []
    for item in msg['result']:
        temp = item.to_dict()
        good_transfer(temp)
        result.append(temp)
    msg['result'] = result
    return json.dumps({'isSuccess': True, 'goods': msg['result'], 'message': "成功"})


@bluep.route('/home/preference', methods=['GET', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def show_preference():
    json_data = request.args.to_dict()
    json_data['limit'] = int(json_data['limit'])
    msg = VisitorBehaviorObject.show_preference(json_data)
    if msg['err'] is not None:
        return json.dumps({'isSuccess': False, 'goods': msg['result'], 'message': msg['err']})
    result = []
    for item in msg['result']:
        temp = item.to_dict()
        good_transfer(temp)
        result.append(temp)
    msg['result'] = result

    return json.dumps({'isSuccess': True, 'goods': msg['result'], 'message': "成功"})

@bluep.route('/home/search', methods=['POST', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def search():
    pass

@bluep.route('/home/allshops', methods=['GET', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def show_all_shops():
    msg = VisitorBehaviorObject.show_all_shops()
    if msg['err'] is not None:
        return json.dumps({'isSuccess': False, 'result': msg['result'], 'message': msg['err']})
    result = []
    for item in msg['result']:
        result.append(item.to_dict())
    msg['result'] = result
    return json.dumps({'isSuccess': True, 'result': msg['result'], 'message': "成功"})



@bluep.route('/home/allshops/search', methods=['POST', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def search_shops():
    pass


@bluep.route('/home/goodhome', methods=['GET', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def show_good_home():
    json_data = request.args.to_dict()
    json_data['good_id'] = int(json_data['good_id'])
    if check_good_id(json_data) is False:
        return json.dumps({'isSuccess': False, 'good': None, 'message': "奇怪的事情发生了"})
    msg = VisitorBehaviorObject.show_good_home(json_data)
    if msg['err'] is not None:
        return json.dumps({'isSuccess': False, 'good': None, 'message': msg['err']})
    good_transfer(msg['result'])
    return json.dumps({'isSuccess': True, 'good': msg['result'], 'message': "成功"})

@bluep.route('/home/allgoods/search', methods=['POST', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def search_goods():
    pass

@bluep.route('/home/category/head', methods=['GET', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def category():
    pass
