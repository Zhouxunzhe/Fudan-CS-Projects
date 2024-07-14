from flask import *
from flask_cors import *
from controller import bluep
from utils.type_transfer import date_transfer
from utils.checker import check_openshop, check_add_good, check_modify_good
from utils.token import parse_token
from service import ShopperBehaviorObject
from datetime import datetime
import decimal


@bluep.route('/open_shop', methods=['POST', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def open_shop():
    """
    商户开店
    :return:
    dict {
        key: "isSuccess", value: bool
        key: "message", value: str
    }
    """
    token = request.headers.get('Authorization')
    json_data = request.get_json()
    # 检测登录信息
    payload, msg = parse_token(token)
    if msg is not None:
        return json.dumps({'isSuccess': False, 'message': msg})
    if not payload['is_shop']:
        return json.dumps({'isSuccess': False, 'message': "您不是商家，无法拥有该权限"})
    # 对填写信息进行验证
    json_data['register_date'] = date_transfer(json_data['register_date'])
    if check_openshop(json_data) is False:
        return json.dumps({'isSuccess': False, 'message': "不好意思，您填写的信息格式有误"})
    json_data['id_num'] = payload['id_num']
    json_data['request_date'] = str(datetime.fromtimestamp(0))
    json_data['register_capital'] = decimal.Decimal(json_data['register_capital'])
    msg = ShopperBehaviorObject.open_shop(json_data)
    if msg['err'] is not None:
        return json.dumps({'isSuccess': False, 'message': msg['err']})
    return json.dumps({'isSuccess': True, 'message': "开店成功"})


@bluep.route('/close_shop', methods=['POST', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def close_shop():
    """
    商户关店
    :return:
    dict {
        key: "isSuccess", value: bool
        key: "message", value: str
    }
    """
    token = request.headers.get('Authorization')
    json_data = {}
    # 检测登录信息
    payload, msg = parse_token(token)
    if msg is not None:
        return json.dumps({'isSuccess': False, 'message': msg})
    if not payload['is_shop']:
        return json.dumps({'isSuccess': False, 'message': "您不是商家，无法拥有该权限"})
    json_data['id_num'] = payload['id_num']
    json_data['request_date'] = str(datetime.fromtimestamp(0))
    msg = ShopperBehaviorObject.close_shop(json_data)
    if msg['err'] is not None:
        return json.dumps({'isSuccess': False, 'message': msg['err']})
    return json.dumps({'isSuccess': True, 'message': "关店成功"})


@bluep.route('/shop/myshop', methods=['GET', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def show_my_shop():
    """
    展示商户商店
    :return:
    dict {
        key: "result", value: {
            key: "total", value: int
            key: "goods", value: array
            key: "isnull", value: bool
            key: "shopname", value: str
        }
        key: "isSuccess", value: bool
        key: "message", value: str
    }
    """
    token = request.headers.get('Authorization')
    # 检测登录信息
    payload, msg = parse_token(token)
    if msg is not None:
        return json.dumps({'result': None, 'isSuccess': False, 'message': msg})
    if not payload['is_shop']:
        return json.dumps({'result': None, 'isSuccess': False, 'message': "您不是商户,无法拥有该权限"})
    json_data = {}
    json_data['id_num'] = payload['id_num']
    msg = ShopperBehaviorObject.show_my_shop(json_data)
    if msg['err'] is not None:
        return json.dumps({'result': None, 'isSuccess': False, 'message': msg['err']})
    return json.dumps({'result': msg['result'], 'isSuccess': True, 'message': "展示商店成功"})


@bluep.route('/add_goods', methods=['POST', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def add_goods():
    """
    商户添加商品
    :return:
    dict {
        key: "isSuccess", value: bool
        key: "message", value: str
    }
    """
    token = request.headers.get('Authorization')
    json_data = request.get_json()
    # 检测登录信息
    payload, msg = parse_token(token)
    if msg is not None:
        return json.dumps({'isSuccess': False, 'message': msg})
    if not payload['is_shop']:
        return json.dumps({'isSuccess': False, 'message': "您不是商家，无法拥有该权限"})
    # 对输入信息进行检验
    json_data['images'] = ",".join(json_data['images'])
    json_data['request_date'] = json_data['request_date'].replace('/', '-')
    if check_add_good(json_data) is False:
        return json.dumps({'isSuccess': False, 'message': "输入信息格式有误"})
    json_data['id_num'] = payload['id_num']
    json_data['price'] = decimal.Decimal(json_data['price'])
    msg = ShopperBehaviorObject.add_good(json_data)
    if msg['err'] is not None:
        return json.dumps({'isSuccess': False, 'message': msg['err']})
    return json.dumps({'isSuccess': True, 'message': "添加商品成功"})


@bluep.route('/ban_good', methods=['POST', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def delete_good():
    """
    商户下架商品
    :return:
    dict {
        key: "isSuccess", value: bool
        key: "message", value: str
    }
    """
    token = request.headers.get('Authorization')
    json_data = request.get_json()
    # 检测登录信息
    payload, msg = parse_token(token)
    if msg is not None:
        return json.dumps({'isSuccess': False, 'message': msg})
    if not payload['is_shop']:
        return json.dumps({'isSuccess': False, 'message': "您不是商家，无法拥有该权限"})
    msg = ShopperBehaviorObject.delete_good(json_data)
    if msg['err'] is not None:
        return json.dumps({'isSuccess': False, 'message': msg['err']})
    return json.dumps({'isSuccess': True, 'message': "下架商品成功"})


@bluep.route('/modify_goods', methods=['POST', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def modify_good():
    """
    商户修改商品
    :return:
    dict {
        key: "isSuccess", value: bool
        key: "message", value: str
    }
    """
    token = request.headers.get('Authorization')
    json_data = request.get_json()
    # 检测登录信息
    payload, msg = parse_token(token)
    if msg is not None:
        return json.dumps({'isSuccess': False, 'message': msg})
    if not payload['is_shop']:
        return json.dumps({'isSuccess': False, 'message': "您不是商家，无法拥有该权限"})
    # 对输入信息进行检验
    json_data['images'] = ",".join(json_data['images'])
    json_data['request_date'] = json_data['request_date'].replace('/', '-')
    if check_modify_good(json_data) is False:
        return json.dumps({'isSuccess': False, 'message': "输入信息格式有误"})
    msg = ShopperBehaviorObject.modify_good(json_data)
    if msg['err'] is not None:
        return json.dumps({'isSuccess': False, 'message': msg['err']})
    return json.dumps({'isSuccess': True, 'message': "修改商品成功"})


@bluep.route('/shop/good_application_record', methods=['GET', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def show_good_request():
    """
    商户获取商品申请记录
    :return:
    dict {
        key: "result", value: array
        key: "isSuccess", value: bool
        key: "message", value: str
    }
    """
    token = request.headers.get('Authorization')
    json_data = {}
    # 检测登录信息
    payload, msg = parse_token(token)
    if msg is not None:
        return json.dumps({'result': None, 'isSuccess': False, 'message': msg})
    if not payload['is_shop']:
        return json.dumps({'result': None, 'isSuccess': False, 'message': "您不是商家，无法拥有该权限"})
    json_data['id_num'] = payload['id_num']
    msg = ShopperBehaviorObject.get_good_request(json_data)
    if msg['err'] is not None:
        return json.dumps({'result': None, 'isSuccess': False, 'message': msg['err']})
    return json.dumps({'result': msg['result'], 'isSuccess': True, 'message': "获取商品申请记录成功"})


@bluep.route('/shop/shop_account', methods=['GET', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def show_shop_account():
    """
    商户获取商店账户信息
    :return:
    dict {
        key: "result", value:
        key: "isSuccess", value: bool
        key: "message", value: str
    }
    """
    token = request.headers.get('Authorization')
    json_data = {}
    # 检测登录信息
    payload, msg = parse_token(token)
    if msg is not None:
        return json.dumps({'isSuccess': False, 'message': msg})
    if not payload['is_shop']:
        return json.dumps({'isSuccess': False, 'message': "您不是商家，无法拥有该权限"})
    json_data['id_num'] = payload['id_num']
    msg = ShopperBehaviorObject.get_shop_account(json_data)
    if msg['err'] is not None:
        return json.dumps({'result': None, 'isSuccess': False, 'message': msg['err']})
    return json.dumps({'result': {'amount': msg['result']}, 'isSuccess': True, 'message': "获取商品申请记录成功"})
