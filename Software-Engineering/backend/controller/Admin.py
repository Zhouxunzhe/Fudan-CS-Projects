import json
from flask import *
from flask_cors import *
from controller import bluep
from utils.token import *
from utils.checker import check_login
from service import AdminBehaviorObject
from datetime import datetime


@bluep.route('/admin_login', methods=['POST', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def admin_login():
    """
    管理员登录
    :return:
    dict {
        key: "isSuccess", value: bool
        key: "message", value: str
        key: "token", value: str
    }
    """
    json_data = request.get_json()
    if check_login(json_data) is False:
        return json.dumps({'isSuccess': False, 'message': "用户名或密码格式出错", 'token': None})
    msg = AdminBehaviorObject.login(json_data)
    if msg['err'] is not None:
        return json.dumps({'isSuccess': False, 'message': msg['err'], 'token': None})
    return json.dumps({'isSuccess': True, 'message': "登录成功", 'token': msg['token']})


@bluep.route('/admin/get_shop_request', methods=['GET', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def admin_get_shop_request():
    """
    管理员获取商店申请信息
    :return:
    dict {
        key: "result", value: array
        key: "isSuccess", value: bool
        key: "message", value: str
    }
    """
    token = request.headers['Authorization']
    # 检测登录信息
    payload, msg = parse_token(token)
    if msg is not None:
        return json.dumps({'result': None, 'isSuccess': False, 'message': msg})
    if not payload['superuser']:
        return json.dumps({'result': None, 'isSuccess': False, 'message': "您不是管理员，无法拥有该权限"})
    json_data = {}
    json_data['is_admin'] = payload['superuser']
    msg = AdminBehaviorObject.get_shop_request(json_data)
    if msg['err'] is not None:
        return json.dumps({'result': None, 'isSuccess': False, 'message': msg['err']})

    return json.dumps({'result': msg['result'], 'isSuccess': True, 'message': "获取商店申请成功"})


@bluep.route('/admin/approve_shop_request', methods=['POST', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def admin_check_shop_request():
    """
    管理员审批商店信息
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
    if not payload['superuser']:
        return json.dumps({'isSuccess': False, 'message': "您不是管理员，无法拥有该权限"})
    # 查询请求状态
    request_id = json_data['request_id']
    approval = json_data['approval']
    json_data['bill_date'] = str(datetime.fromtimestamp(0))
    if request_id is None or approval is None:
        return json.dumps({'isSuccess': False, 'message': "传入数据出错"})
    msg = AdminBehaviorObject.check_shop_request(json_data)
    if msg['err'] is not None:
        return json.dumps({'isSuccess': False, 'message': msg['err']})
    return json.dumps({'isSuccess': True, 'message': "审批成功"})


@bluep.route('/admin/get_good_request', methods=['GET', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def admin_get_good_request():
    """
        管理员获取商品申请信息
        :return:
        dict {
            key: "result", value: array
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
    if not payload['superuser']:
        return json.dumps({'result': None, 'isSuccess': False, 'message': "您不是管理员，无法拥有该权限"})
    json_data['is_admin'] = True
    msg = AdminBehaviorObject.get_good_request(json_data)
    if msg['err'] is not None:
        return json.dumps({'result': None, 'isSuccess': False, 'message': msg['err']})
    return json.dumps({'result': msg['result'], 'isSuccess': True, 'message': "获取商品申请成功"})


@bluep.route('/admin/approve_good_request', methods=['POST', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def admin_check_good_request():
    """
    管理员批准商品信息
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
    if not payload['superuser']:
        return json.dumps({'isSuccess': False, 'message': "您不是管理员，无法拥有该权限"})
    # 查询请求状态
    request_id = json_data['request_id']
    approval = json_data['approval']
    if request_id is None or approval is None:
        return json.dumps({'isSuccess': False, 'message': "传入数据出错"})
    msg = AdminBehaviorObject.check_good_request(json_data)
    if msg['err'] is not None:
        return json.dumps({'isSuccess': False, 'message': msg['err']})
    return json.dumps({'isSuccess': True, 'message': "审批成功"})


@bluep.route('/admin_recharge', methods=['POST', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def admin_recharge():
    """
    管理员充值
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
    if not payload['superuser']:
        return json.dumps({'isSuccess': False, 'message': "您不是管理员，无法拥有该权限"})
    msg = AdminBehaviorObject.recharge(json_data)
    if msg['err'] is not None:
        return json.dumps({'isSuccess': False, 'message': msg['err']})
    return json.dumps({'isSuccess': True, 'message': "充值成功"})


@bluep.route('/admin_account', methods=['POST', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def admin_account():
    """
    获取管理员个人账号和商城账号信息
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
    if not payload['superuser']:
        return json.dumps({'result': None, 'isSuccess': False, 'message': "您不是管理员，无法拥有该权限"})
    
    msg = AdminBehaviorObject.get_account({})
    if msg['err'] is not None:
        return json.dumps({'result': None, 'isSuccess': False, 'message': msg['err']})
    return json.dumps({'result': msg['result'], 'isSuccess': True, 'message': "查询成功"})
