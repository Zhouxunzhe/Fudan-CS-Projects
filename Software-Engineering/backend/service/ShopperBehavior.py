from model import (
    session,
    ShopOperationObject,
    ShopRequestOperationObject,
    GoodOperationObject,
    GoodRequestOperationObject,
    AccountOperationObject,
    BillOperationObject
)
from model.enum_type import RequestType, AccountType, BillType


class ShopperBehavior:
    """
    商户操作
    """
    def open_shop(self, data):
        """
        商户申请开店
        :param:
        data(dict) {
            key: "shopname", value: str
            key: "kind", value: str
            key: "id_num", value: str
            key: "intro", value: str
            key: "address", value: str
            key: "register_capital", value: str
            key: "register_date", value: str
            key: "request_date", value: str
        }
        :return:
        dict {
            key: "err", value: str
        }
        """
        # 查看当前用户是否申请了商店
        request_msg = ShopRequestOperationObject.get_current_request(data)
        if request_msg['err'] is not None:
            session.rollback()
            return {'err': request_msg['err']}
        if len(request_msg['results']) != 0:
            session.rollback()
            return {'err': '您已有正在申请的商店'}
        # 商店表
        shop_msg = ShopOperationObject.add_shop(data)
        if shop_msg['err'] is not None:
            session.rollback()
            return {'err': shop_msg['err']}
        # 获取shop_id
        new_shop = shop_msg['result']
        request_data = {
            'request_type': RequestType.open,
            'shop_id': new_shop.shop_id,
            'request_date': data['request_date']
        }
        # 商店申请表
        new_request_msg = ShopRequestOperationObject.add_request(request_data)
        if new_request_msg['err'] is not None:
            session.rollback()
            return {'err': new_request_msg['err']}
        # 商户账户表
        shopper_account_data = {
            'account_type': AccountType.user,
            'id_num': new_shop.id_num,
            'amount': new_shop.register_capital
        }
        shopper_account_msg = AccountOperationObject.decrease_balance(shopper_account_data)
        if shopper_account_msg['err'] is not None:
            session.rollback()
            return {'err': '账户余额不足'}
            # return {'err': shopper_account_msg['err']}
        # 中间账户表
        middle_account_data = {
            'account_type': AccountType.middle,
            'amount': new_shop.register_capital
        }
        middle_msg = AccountOperationObject.increase_balance(middle_account_data)
        if middle_msg['err'] is not None:
            session.rollback()
            return {'err': middle_msg['err']}
        # 商店账户表
        shop_account_data = {
            'account_type': AccountType.shop,
            'shop_id': new_shop.shop_id
        }
        shop_account_msg = AccountOperationObject.create_account(shop_account_data)
        if shop_account_msg['err'] is not None:
            session.rollback()
            return {'err': shop_account_msg['err']}
        bill_data = {
            'amount': shop_msg['result'].register_capital,
            'sender_id': shopper_account_msg['result'].account_id,
            'receiver_id': middle_msg['result'].account_id,
            'bill_date': data['request_date'],
            'bill_type': BillType.open
        }
        bill_msg = BillOperationObject.add_bill(bill_data)
        if bill_msg['err'] is not None:
            session.rollback()
            return {'err': bill_msg['err']}
        session.commit()
        return {'err': None}

    def close_shop(self, data):
        """
        商户申请关店
        :param:
        data(dict) {
            key: "id_num", value: str
            key: "request_date", value: str
        }
        :return:
        dict {
            key: "err", value: str
        }
        """
        # 获取shop_id
        msg = ShopOperationObject.get_shop({"id_num": data['id_num'], "is_open": True})
        if msg['err'] is not None:
            session.rollback()
            return {'err': msg['err']}
        data['shop_id'] = msg['result'].shop_id
        # 商店申请表
        request_data = {
            'request_type': RequestType.close,
            'shop_id': data['shop_id'],
            'request_date': data['request_date']
        }
        msg = ShopRequestOperationObject.add_request(request_data)
        if msg['err'] is not None:
            session.rollback()
            return {'err': msg['err']}
        # 获取商店账户信息
        shop_account_data = {
            'account_type': AccountType.shop,
            'id_num': data['id_num'],
            'shop_id': data['shop_id']
        }
        msg = AccountOperationObject.get_balance(shop_account_data)
        if msg['err'] is not None:
            session.rollback()
            return {'err': msg['err']}
        shop_amount = msg['amount']
        # 商店账户表
        shop_account_data['amount'] = shop_amount
        msg = AccountOperationObject.decrease_balance(shop_account_data)
        if msg['err'] is not None:
            session.rollback()
            return {'err': msg['err']}
        # 商户账户表
        shopper_account_data = {
            'account_type': AccountType.user,
            'id_num': data['id_num'],
            'shop_id': data['shop_id'],
            'amount': shop_amount
        }
        msg = AccountOperationObject.increase_balance(shopper_account_data)
        if msg['err'] is not None:
            session.rollback()
            return {'err': msg['err']}
        session.commit()
        return {'err': None}

    def add_good(self, data):
        """
        商户申请添加商品
        :param:
        data(dict) {
            key: "id_num", value: str
            key: "goodname", value: str
            key: "intro", value: str
            key: "price", value: dec
            key: "images", value: array
            key: "goodamount", value: int
            key: "request_date", value: str
        }
        :return:
        dict {
            key: "err", value: str
        }
        """
        # 获取shop_id
        msg = ShopOperationObject.get_shop({"id_num": data['id_num'], "is_open": True})
        if msg['err'] is not None:
            session.rollback()
            return {'err': msg['err']}
        if msg['result'] is None or msg['result'].is_open is False:
            session.rollback()
            return {'err': "没有合法商店"}
        data['shop_id'] = msg['result'].shop_id
        # 添加商品
        msg = GoodOperationObject.add_good(data)
        if msg['err'] is not None:
            session.rollback()
            return {'err': msg['err']}
        new_good = msg['result']
        request_data = {
            'request_type': RequestType.open,
            'good_id': new_good.good_id,
            'request_date': data['request_date']
        }
        msg = GoodRequestOperationObject.add_request(request_data)
        if msg['err'] is not None:
            session.rollback()
            return {'err': msg['err']}
        session.commit()
        return {'err': None}

    def delete_good(self, data):
        """
        商户申请删除商品
        :param:
        data(dict) {
            key: "good_id", value: int
        }
        :return:
        dict {
            key: "err", value: str
        }
        """
        msg = GoodOperationObject.delete_good({'good_id': data['good_id']})
        if msg['err'] is not None:
            session.rollback()
            return {'err': msg['err']}
        session.commit()
        return {'err': None}
    
    def modify_good(self, data):
        """
        商户申请修改商品信息
        :param:
        data(dict) {
            key: "good_id", value: int
            key: "goodname", value: str
            key: "intro", value: str
            key: "price", value: dec
            key: "image", value: array
            key: "request_date", value: str
        }
        :return:
        dict {
            key: "err", value: str
        }
        """
        for key in list(data.keys()):
            if not data[key]:
                del data[key]
        msg = GoodOperationObject.modify_temp_good(data)
        if msg['err'] is not None:
            session.rollback()
            return {'err': msg['err']}
        request_data = {
            'request_type': RequestType.modify,
            'good_id': data['good_id'],
            'request_date': data['request_date']
        }
        msg = GoodRequestOperationObject.add_request(request_data)
        if msg['err'] is not None:
            session.rollback()
            return {'err': msg['err']}
        session.commit()
        return {'err': None}

    def show_my_shop(self, data):
        """
        商户申请添加商品
        :param:
        data(dict) {
            key: "id_num", value: str
            key: "goodname", value: str
            key: "intro", value: str
            key: "price", value: dec
            key: "images", value: array
            key: "goodamount", value: int
            key: "request_date", value: str
        }
        :return:
        dict {
            key: "result", value dict{
                key: "total", value: int
                key: "goods", value: array
                key: "shopname", value: str
                key: "isnull", value: bool
            }
            key: "err", value: str
        }
        """
        # 获取shop_id
        shop_msg = ShopOperationObject.get_shop({"id_num": data['id_num'], "is_open": True})
        if shop_msg['err'] is not None:
            session.rollback()
            return {'result': None, 'err': shop_msg['err']}
        if shop_msg['result'] is None or shop_msg['result'].is_open is False:
            session.rollback()
            return {'result': None, 'err': "没有正在开的店"}
        data['shop_id'] = shop_msg['result'].shop_id
        good_msg = GoodOperationObject.show_goods(data)
        if good_msg['err'] is not None:
            session.rollback()
            return {'result': None, 'err': good_msg['err']}
        session.commit()
        r = []
        for item in good_msg['result']:
            r.append(item.to_dict())
        result = {
            'total': len(good_msg['result']),
            'goods': r,
            'shopname': shop_msg['result'].shopname,
            'isnull': False
        }
        return {'result': result, 'err': None}

    def get_good_request(self, data):
        """
        获取商品申请记录
        :param:
        data(dict) {
            key: "id_num", value: str
        }
        :return:
        dict {
            key: "result", value: list
            key: "err", value: str
        }
        """
        # 获取shop_id
        shop_msg = ShopOperationObject.get_shop({"id_num": data['id_num'], "is_open": True})
        if shop_msg['err'] is not None:
            session.rollback()
            return {'result': None, 'err': shop_msg['err']}
        if shop_msg['result'] is None or shop_msg['result'].is_open is False:
            session.rollback()
            return {'result': None, "err": "没有正在开的店"}
        data['shop_id'] = shop_msg['result'].shop_id
        # 获取request
        request_msg = GoodRequestOperationObject.show_requests({'shop_id': data['shop_id'], 'is_admin': False})
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
            r = {
                'request_id': rm.request_id,
                'request_type': rm.request_type.value,
                'is_check': rm.is_check,
                'comment': rm.comment.value if rm.comment is not None else None,
                'request_date': rm.request_date,
                'info': rm.info,
                'good_id': rm.good_id,
                'shopname': shop_msg['result'].shopname,
                'goodname': good_msg['result'].goodname,
            }
            result.append(r)
        session.commit()
        return {'result': result, 'err': None}

    def get_shop_account(self, data):
        """
        商户获取商店账户
        :param:
        data(dict) {
            key: "id_num", value: str
        }
        :return:
        dict {
            key: 'result', value: float
            key: "err", value: str
        }
        """
        # 商户账户表
        msg = ShopOperationObject.get_shop({'id_num': data['id_num']})
        if msg['err'] is not None:
            session.rollback()
            return {'result': None, 'err': msg['err']}
        shopper_account_data = {
            'account_type': AccountType.shop,
            'shop_id': msg['result'].shop_id
        }
        msg = AccountOperationObject.get_balance(shopper_account_data)
        if msg['err'] is not None:
            session.rollback()
            return {'result': None, 'err': msg['err']}
        amount = str(msg['amount'])
        session.commit()
        return {'result': amount, 'err': None}

    def show_shop_bills(self, data):
        """
        展示商店账单
        params:
        data(dict) {
            key: "id_num", value: str
        }
        return:
        dict {
            key: "result", value: list
            key: "err", value: str
        }
        """
        account_msg = AccountOperationObject.get_account_id(
            {'account_type': AccountType.shop,
             'id_num': data['id_num']})
        if account_msg['err'] is not None:
            session.rollback()
            return {"results": None, "err": account_msg['err']}
        bill_msg = BillOperationObject.show_bills(
            {'account_id': account_msg['account_id']})
        if bill_msg['err'] is not None:
            session.rollback()
            return {"results": None, "err": bill_msg['err']}
        session.commit()
        return {"results": bill_msg, "err": None}
