import json
from flask import *
from flask_cors import *
from controller import bluep
from utils.token import *
from utils.checker import (
    check_register, 
    check_login, 
    check_modify_userinfo,
    check_recharge
)
from utils.type_transfer import date_transfer
from service import UserBehaviorObject
import decimal
from datetime import datetime


@bluep.route('/login', methods=['POST', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def login():
    """
    用户登录
    :return:
    dict {
        key: "isSuccess", value: bool
        key: "message", value: str
        key: "result", value: object
    }
    """
    json_data = request.get_json()
    if check_login(json_data) is False:
        return json.dumps({'isSuccess': False, 'message': "用户名或密码格式出错", 'result': None})
    msg = UserBehaviorObject.login(json_data)
    if msg['err'] is not None:
        return json.dumps({'isSuccess': False, 'message': msg['err'], 'result': None})
    return json.dumps({'isSuccess': True, 'message': "登录成功", 'result': msg['result']})


@bluep.route('/register', methods=['POST', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def register():
    """
    用户注册
    :return:
    dict {
        key: "isSuccess", value: bool
        key: "message", value: str
    }
    """
    json_data = request.get_json()
    if check_register(json_data) is False:
        return json.dumps({'isSuccess': False, 'message': "注册信息填写格式有误"})
    msg = UserBehaviorObject.register(json_data)
    if msg['err'] is not None:
        return json.dumps({'isSuccess': False, 'message': msg['err']})
    return json.dumps({'isSuccess': True, 'message': "注册成功"})


@bluep.route('/modify_userinfo', methods=['POST', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def modify_userinfo():
    """
    修改用户信息
    :return:
    dict {
        key: "isSuccess", value: bool
        key: "message", value: str
    }
    """
    token = request.headers['Authorization']
    json_data = request.get_json()
    # 检测登录信息
    payload, msg = parse_token(token)
    if msg is not None:
        return json.dumps({'result': None, 'isSuccess': False, 'message': msg})
    # 对输入信息进行检验
    if check_modify_userinfo(json_data) is False:
        return json.dumps({'isSuccess': False, 'message': "输入信息格式有误"})
    json_data['id_num'] = payload['id_num']
    msg = UserBehaviorObject.modify_userinfo(json_data)
    if msg['err'] is not None:
        return json.dumps({'isSuccess': False, 'message': msg['err']})
    return json.dumps({'isSuccess': True, 'message': "修改用户信息成功"})


@bluep.route('/recharge', methods=['POST', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def recharge():
    """
    修改用户信息
    :return:
    dict {
        key: "isSuccess", value: bool
        key: "message", value: str
    }
    """
    token = request.headers['Authorization']
    json_data = request.get_json()
    # 检测登录信息
    payload, msg = parse_token(token)
    if msg is not None:
        return json.dumps({'result': None, 'isSuccess': False, 'message': msg})
    if check_recharge(json_data) is False:
        return json.dumps({'result': None, 'isSuccess': False, 'message': '输入金额有误'})        
    json_data['id_num'] = payload['id_num']
    json_data['amount'] = decimal.Decimal(json_data['amount'])
    json_data['bill_date'] = json_data['bill_date'].replace('/', '-')
    msg = UserBehaviorObject.recharge(json_data)
    if msg['err'] is not None:
        return json.dumps({'isSuccess': False, 'message': msg['err']})
    return json.dumps({'isSuccess': True, 'message': "充值成功"})


@bluep.route('/user/user_account', methods=['GET', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def show_user_account():
    """
    展示个人账户
    :return:
    dict {
        key: "result", value: object
        key: "isSuccess", value: bool
        key: "message", value: str
    }
    """
    token = request.headers['Authorization']
    # 检测登录信息
    payload, msg = parse_token(token)
    if msg is not None:
        return json.dumps({'result': None, 'isSuccess': False, 'message': msg})
    json_data = {}
    json_data['id_num'] = payload['id_num']
    msg = UserBehaviorObject.show_account(json_data)
    if msg['err'] is not None:
        return json.dumps({'result': None, 'isSuccess': False, 'message': msg['err']})
    result = msg['result']
    result['id_num'] = payload['id_num']
    result['is_shop'] = payload['is_shop']
    return json.dumps({'result': result, 'isSuccess': True, 'message': "获取账户信息成功"})


@bluep.route('/add_to_cart', methods=['POST', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def add_to_cart():
    """
    将商品加入购物车
    :return:
    dict {
        key: "isSuccess", value: bool
        key: "message", value: str
    }
    """
    token = request.headers['Authorization']
    json_data = request.get_json()
    # 检测登录信息
    payload, msg = parse_token(token)
    if msg is not None:
        return json.dumps({'isSuccess': False, 'message': msg})
    json_data['id_num'] = payload['id_num']
    msg = UserBehaviorObject.add_to_cart(json_data)
    if msg['err'] is not None:
        return json.dumps({'isSuccess': False, 'message': msg['err']})
    return json.dumps({'isSuccess': True, 'message': "加入购物车成功"})


@bluep.route('/home/cart', methods=['GET', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def show_shoppingcart():
    """
    展示购物车
    :return:
    dict {
        key: "result", value: list
        key: "isSuccess", value: bool
        key: "message", value: str
    }
    """
    token = request.headers['Authorization']
    json_data = {}
    # 检测登录信息
    payload, msg = parse_token(token)
    if msg is not None:
        return json.dumps({'result': None, 'isSuccess': False, 'message': msg})
    json_data['id_num'] = payload['id_num']
    msg = UserBehaviorObject.show_cart(json_data)
    if msg['err'] is not None:
        return json.dumps({'result': None, 'isSuccess': False, 'message': msg['err']})
    return json.dumps({'result': msg['result'], 'isSuccess': True, 'message': "成功"})


@bluep.route('/delete_from_cart', methods=['DELETE', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def delete_from_cart():
    """
    将商品从购物车中删除
    :return:
    dict {
        key: "isSuccess", value: bool
        key: "message", value: str
    }
    """
    token = request.headers['Authorization']
    json_data = request.get_json()
    # 检测登录信息
    payload, msg = parse_token(token)
    if msg is not None:
        return json.dumps({'isSuccess': False, 'message': msg})
    json_data['id_num'] = payload['id_num']
    msg = UserBehaviorObject.delete_from_cart(json_data)
    if msg['err'] is not None:
        return json.dumps({'isSuccess': False, 'message': msg['err']})
    return json.dumps({'isSuccess': True, 'message': "从购物车中删除成功"})


@bluep.route('/updatecart', methods=['PUT', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def update_cart():
    """
    更新购物车
    :return:
    dict {
        key: "isSuccess", value: bool
        key: "message", value: str
    }
    """
    token = request.headers['Authorization']
    json_data = request.get_json()
    # 检测登录信息
    payload, msg = parse_token(token)
    if msg is not None:
        return json.dumps({'isSuccess': False, 'message': msg})
    json_data['id_num'] = payload['id_num']
    msg = UserBehaviorObject.update_cart(json_data)
    if msg['err'] is not None:
        return json.dumps({'isSuccess': False, 'message': msg['err']})
    return json.dumps({'isSuccess': True, 'message': "更新购物车成功"})


@bluep.route('/cart/selectall', methods=['PUT', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def select_all_cart():
    # TODO 前端似乎不需要使用这个函数
    pass

@bluep.route('/user/submit_order', methods=['POST', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def submit_order():
    token = request.headers['Authorization']
    json_data = request.get_json()
    payload, msg = parse_token(token)
    if msg is not None:
        return json.dumps({'isSuccess': False, "result": None, 'message': msg})
    json_data['id_num'] = payload['id_num']
    msg = UserBehaviorObject.submit_order(json_data)
    if msg['err'] is not None:
        return json.dumps({'isSuccess': False, "result": None, 'message': msg['err']})
    return json.dumps({'isSuccess': True, "result": {"order_id": msg['order_id']},'message': "提交订单成功"})


@bluep.route('/user/cancel_order', methods=['POST', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def cancel_order():
    token = request.headers['Authorization']
    json_data = request.get_json()
    payload, msg = parse_token(token)
    if msg is not None:
        return json.dumps({'isSuccess': False, 'message': msg})
    msg = UserBehaviorObject.cancel_order(json_data)
    if msg['err'] is not None:
        return json.dumps({'isSuccess': False, 'message': msg['err']})
    return json.dumps({'isSuccess': True, 'message': "取消订单成功"})


@bluep.route('/user/pay_order', methods=['POST', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def pay_order():
    token = request.headers['Authorization']
    json_data = request.get_json()
    payload, msg = parse_token(token)
    if msg is not None:
        return json.dumps({'isSuccess': False, 'message': msg})
    json_data['id_num'] = payload['id_num']
    json_data['bill_date'] = str(datetime.fromtimestamp(0))
    msg = UserBehaviorObject.pay_order(json_data)
    if msg['err'] is not None:
        return json.dumps({'isSuccess': False, 'message': msg['err']})
    return json.dumps({'isSuccess': True, 'message': "支付订单成功"})


@bluep.route('/user/get_order', methods=['GET', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def get_order():
    token = request.headers['Authorization']
    payload, msg = parse_token(token)
    if msg is not None:
        return json.dumps({'isSuccess': False, "result": None, 'message': msg})
    json_data = {"id_num": payload['id_num']}
    msg = UserBehaviorObject.show_order(json_data)
    if msg['err'] is not None:
        return json.dumps({'isSuccess': False, "result": None, 'message': msg['err']})
    return json.dumps({'isSuccess': True, "result": msg['results'], 'message': "成功"})

@bluep.route('/user/get_order_by_id', methods=['POST', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def get_order_by_id():
    token = request.headers['Authorization']
    payload, msg = parse_token(token)
    json_data = request.get_json()
    if msg is not None:
        return json.dumps({'isSuccess': False, "result": None, 'message': msg})
    json_data["id_num"] = payload['id_num']
    msg = UserBehaviorObject.get_order_by_id(json_data)
    if msg['err'] is not None:
        return json.dumps({'isSuccess': False, "result": None, 'message': msg['err']})
    return json.dumps({'isSuccess': True, "result": msg['result'], 'message': "成功"})
