from werkzeug.security import check_password_hash
from utils.token import *
from model.models import Superusers
from model.enum_type import *
from model import session
from model import SuperUserOperationObject, ShopRequestOperationObject, \
    GoodRequestOperationObject, ShopOperationObject, GoodOperationObject, \
    AccountOperationObject, BillOperationObject


class AdminBehavior:
    """
    管理员操作
    """

    def login(self, data):
        """
        管理员登录
        :param:
        data(dict) {
            key: "username", value: str
            key: "password", value: str
        }
        :return:
        dict {
            key: "token", value: str
            key: "err", value: str
        }
        """
        r = SuperUserOperationObject.get_user(data)
        if r['err'] is not None:
            session.rollback()
            return {'token': None, 'err': r['err']}
        session.commit()
        result: Superusers = r['result']
        if (check_password_hash(result.password, data['password'])) is False:
            return {'token': None, 'err': "用户名或密码错误"}
        return {'token': create_token(0, False, True), 'err': None}

    def check_shop_request(self, data):
        """
        管理员审批商店请求
        :param:
        data(dict) {
            key: "request_id", value: int
            key: "approval", value: bool
            key: "bill_date", value: date
        }
        :return:
        dict {
            key: "err", value: str
        }
        """
        comment = None
        request_msg = ShopRequestOperationObject.get_request(data)
        if request_msg['err'] is not None:
            session.rollback()
            return {'err': request_msg['err']}
        data['shop_id'] = request_msg['result'].shop_id
        if data['approval'] is True:
            comment = CommentType.approve
        if data['approval'] is False:
            comment = CommentType.refuse
        if request_msg['result'].request_type == RequestType.open:
            if data['approval'] is True:
                # 获取注册资金
                shop_msg = ShopOperationObject.get_shop(data)
                if shop_msg['err'] is not None:
                    session.rollback()
                    return {'err': shop_msg['err']}
                # 将注册资金从中间账户转到商城利润账户
                middle_data = {
                    'account_type': AccountType.middle,
                    'amount': shop_msg['result'].register_capital
                }
                middle_msg = AccountOperationObject.decrease_balance(middle_data)
                if middle_msg['err'] is not None:
                    session.rollback()
                    return {'err': middle_msg['err']}
                mall_data = {
                    'account_type': AccountType.mall,
                    'amount': shop_msg['result'].register_capital
                }
                mall_msg = AccountOperationObject.increase_balance(mall_data)
                if mall_msg['err'] is not None:
                    session.rollback()
                    return {'err': mall_msg['err']}
                bill_data = {
                    'amount': shop_msg['result'].register_capital,
                    'sender_id': middle_msg['result'].account_id,
                    'receiver_id': mall_msg['result'].account_id,
                    'bill_date': data['bill_date'],
                    'bill_type': BillType.open_success
                }
                bill_msg = BillOperationObject.add_bill(bill_data)
                if bill_msg['err'] is not None:
                    session.rollback()
                    return {'err': bill_msg['err']}
            if data['approval'] is False:
                shop_msg = ShopOperationObject.get_shop(data)
                if shop_msg['err'] is not None:
                    session.rollback()
                    return {'err': shop_msg['err']}
                msg = ShopOperationObject.modify_shop({'shop_id': data['shop_id'], 'is_open': False,
                                                       'shopname': shop_msg['result'].shopname + '@' +
                                                                   str(shop_msg['result'].shop_id)})
                if msg['err'] is not None:
                    session.rollback()
                    return {'err': msg['err']}
                # 中间账户表
                middle_account_data = {
                    'account_type': AccountType.middle,
                    'amount': shop_msg['result'].register_capital
                }
                middle_msg = AccountOperationObject.decrease_balance(middle_account_data)
                if middle_msg['err'] is not None:
                    session.rollback()
                    return {'err': middle_msg['err']}
                # 商户账户表
                shopper_account_data = {
                    'account_type': AccountType.user,
                    'id_num': shop_msg['result'].id_num,
                    'amount': shop_msg['result'].register_capital
                }
                shopper_msg = AccountOperationObject.increase_balance(shopper_account_data)
                if shopper_msg['err'] is not None:
                    session.rollback()
                    return {'err': shopper_msg['err']}
                bill_data = {
                    'amount': shop_msg['result'].register_capital,
                    'sender_id': middle_msg['result'].account_id,
                    'receiver_id': shopper_msg['result'].account_id,
                    'bill_date': data['bill_date'],
                    'bill_type': BillType.open_fail
                }
                bill_msg = BillOperationObject.add_bill(bill_data)
                if bill_msg['err'] is not None:
                    session.rollback()
                    return {'err': bill_msg['err']}

        # 审批商店申请
        msg = ShopRequestOperationObject.check_request({'request_id': data['request_id'], 'comment': comment})
        if msg['err'] is not None:
            session.rollback()
            return {'err': msg['err']}
        if data['approval'] is True:
            # 修改商店状态
            if request_msg['result'].request_type == RequestType.open:
                # 如果批准开店请求，则只修改店铺开启状态
                msg = ShopOperationObject.modify_shop({'shop_id': data['shop_id'], 'is_open': True})
            elif request_msg['result'].request_type == RequestType.close:
                # 如果批准关店请求，则需要将商店名进行修改，将开店状态更新为False
                # 获取商店名
                shop_msg = ShopOperationObject.get_shop(data)
                if shop_msg['err'] is not None:
                    session.rollback()
                    return {'err': shop_msg['err']}
                msg = ShopOperationObject.modify_shop({'shop_id': data['shop_id'], 'is_open': False,
                                                       'shopname': shop_msg['result'].shopname + '@' +
                                                                   str(shop_msg['result'].shop_id)})
                if msg['err'] is not None:
                    session.rollback()
                    return {'err': msg['err']}
                # 删除商店中的所有商品
                msg = GoodOperationObject.delete_shop_good({'shop_id': data['shop_id']})
        if data['approval'] is False:
            # 修改商店状态
            if request_msg['result'].request_type == RequestType.open:
                # 如果拒绝开店请求，则需要更新商店名以防止重复申请
                # 获取商店名
                pass
            elif request_msg['result'].request_type == RequestType.close:
                # 如果拒绝关店申请，则需要将店铺开启状态修改为开启
                # 获取商店名
                shop_msg = ShopOperationObject.get_shop(data)
                if shop_msg['err'] is not None:
                    session.rollback()
                    return {'err': shop_msg['err']}
                msg = ShopOperationObject.modify_shop({'shop_id': data['shop_id'], 'is_open': True})
                if msg['err'] is not None:
                    session.rollback()
                    return {'err': msg['err']}
        if msg['err'] is not None:
            session.rollback()
            return {'err': msg['err']}
        session.commit()
        return {'err': None}

    def check_good_request(self, data):
        """
        管理员审批商品请求
        :param:
        data(dict) {
            key: "request_id", value: int
            key: "approval", value: bool
        }
        :return:
        dict {
            key: "err", value: str
        }
        """
        comment = None
        if data['approval'] is True:
            comment = CommentType.approve
        if data['approval'] is False:
            comment = CommentType.refuse
        # 审批审批请求
        msg = GoodRequestOperationObject.check_request({'request_id': data['request_id'], 'comment': comment})
        if msg['err'] is not None:
            session.rollback()
            return {'err': msg['err']}
        # 获取good_id
        request_msg = GoodRequestOperationObject.get_request({'request_id': data['request_id']})
        if request_msg['err'] is not None:
            session.rollback()
            return {'err': request_msg['err']}
        # 修改商品状态
        if request_msg['result'].request_type == RequestType.open:
            if comment == CommentType.approve:
                msg = GoodOperationObject.modify_good({'good_id': request_msg['result'].good_id, 'is_legal': True})
            else:
                msg = GoodOperationObject.modify_good({'good_id': request_msg['result'].good_id, 'is_legal': False})
        if request_msg['result'].request_type == RequestType.modify:
            if comment == CommentType.approve:
                msg = GoodOperationObject.modify_good({'good_id': request_msg['result'].good_id, 'update': True})
        if msg['err'] is not None:
            session.rollback()
            return {'err': msg['err']}
        session.commit()
        return {'err': None}

    def get_shop_request(self, data):
        """
        获取商店申请记录
        :param:
        data(dict) {
            key: "is_admin", value: bool
        }
        :return:
        dict {
            key: "result", value: list
            key: "err", value: str
        }
        """
        # 获取shop_request
        request_msg = ShopRequestOperationObject.show_requests(data)
        if request_msg['err'] is not None:
            session.rollback()
            return {'result': None, 'err': request_msg['err']}
        result = []
        for rm in request_msg['results']:
            # 获取shopname
            data['shop_id'] = rm.shop_id
            shop_msg = ShopOperationObject.get_shop(data)
            if shop_msg['err'] is not None:
                session.rollback()
                return {'result': None, 'err': shop_msg['err']}
            r = {
                'request_id': rm.request_id,
                'request_type': rm.request_type.value,
                'shopname': shop_msg['result'].shopname,
                'is_check': rm.is_check,
                'is_open': shop_msg['result'].is_open,
                'comment': rm.comment.value if rm.comment is not None else None,
                'request_date': str(rm.request_date),
                'info': rm.info
            }
            result.append(r)
        session.commit()
        return {'result': result, 'err': None}

    def get_good_request(self, data):
        """
        获取商品申请记录
        :param:
        data(dict) {
            key: "is_admin", value: bool
        }
        :return:
        dict {
            key: "result", value: list
            key: "err", value: str
        }
        """
        # 获取good_request
        request_msg = GoodRequestOperationObject.show_requests(data)
        if request_msg['err'] is not None:
            session.rollback()
            return {'result': None, 'err': request_msg['err']}
        result = []
        for rm in request_msg['results']:
            # 获取goodname
            data['good_id'] = rm.good_id
            good_msg = GoodOperationObject.get_good(data)
            if good_msg['err'] is not None:
                session.rollback()
                return {'result': None, 'err': good_msg['err']}
            # 获取shopname
            data['shop_id'] = good_msg['result'].shop_id
            shopname_msg = ShopOperationObject.get_shop(data)
            if shopname_msg['err'] is not None:
                session.rollback()
                return {'result': None, 'err': shopname_msg['err']}
            r = {
                'request_id': rm.request_id,
                'request_type': rm.request_type.value,
                'is_check': rm.is_check,
                'comment': rm.comment.value if rm.comment is not None else None,
                'request_date': str(rm.request_date),
                'info': rm.info,
                'good_id': rm.good_id,
                'shopname': shopname_msg['result'].shopname,
                'is_open': shopname_msg['result'].is_open,
                'goodname': good_msg['result'].goodname
            }
            result.append(r)
        session.commit()
        return {'result': result, 'err': None}

    def recharge(self, data):
        """
        充值
        params:
        data(dict) {
            key: "amount", value: float
        }
        return:
        dict {
            key: "err", value: str
        }
        """
        account_data = {
            'account_type': AccountType.user,
            'id_num': "-1",
            'amount': data['amount']
        }
        msg = AccountOperationObject.increase_balance(account_data)
        if msg['err'] is not None:
            session.rollback()
            return {'err': msg['err']}
        session.commit()
        return {'err': None}

    def get_account(self, data):
        """
        查询管理员个人账号和商城账号
        return:
        dict {
            key: "result", value {
                key: "profit_amount", value float
                key: "median_amount", value float
                key: "admin_amount", value float
            }
            key: "err", value: str
        }
        """
        profit_data = {
            'account_type': AccountType.mall
        }
        profit_msg = AccountOperationObject.get_balance(profit_data)
        if profit_msg['err'] is not None:
            session.rollback()
            return {'result': None, 'err': profit_msg['err']}
        median_data = {
            'account_type': AccountType.middle
        }
        median_msg = AccountOperationObject.get_balance(median_data)
        if median_msg['err'] is not None:
            session.rollback()
            return {'result': None, 'err': median_msg['err']}
        admin_data = {
            'account_type': AccountType.user,
            'id_num': "-1"
        }
        admin_msg = AccountOperationObject.get_balance(admin_data)
        if admin_msg['err'] is not None:
            session.rollback()
            return {'result': None, 'err': admin_msg['err']}
        session.commit()
        return {'result': {'profit_amount': str(profit_msg['amount']),
                           'median_amount': str(median_msg['amount']),
                           'admin_amount': str(admin_msg['amount'])}, 'err': None}
