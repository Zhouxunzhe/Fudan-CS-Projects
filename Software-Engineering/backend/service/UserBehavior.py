from werkzeug.security import generate_password_hash, check_password_hash
from utils.token import create_token
from model.models import Users, Orders, OrderGoods, Goods
from model.enum_type import AccountType, BillType, OrderType
from model import (
    session,
    BillOperationObject,
    UserOperationObject,
    AccountOperationObject,
    ShoppingCartOperationObject,
    GoodOperationObject,
    OrderOperationObject,
    BillOperationObject,
    OrderGoodOperationObject,
    ShopOperationObject
)
import decimal

class UserBehavior:
    """
    用户操作
    """

    def register(self, data):
        """
        用户注册
        params: 
        data(dict) {
            key: "id_num", value: str
            key: "username", value: str
            key: "password", value: str
            key: "phonenumber", value: str
            key: "email", value: str
            key: "is_shop", value: Boolean
        }
        return:
        dict {
            key: "err", value: str    
        }
        """
        # 创建用户
        data["password"] = generate_password_hash(data["password"])
        r = UserOperationObject.create_user(data)
        if r['err'] is not None:
            session.rollback()
            return {"err": r['err']}

        # 绑定账户
        r = AccountOperationObject.create_account({"account_type": AccountType.user, "id_num": data['id_num']})
        if r['err'] is not None:
            session.rollback()
            return {"err": r['err']}

        session.commit()
        return {"err": None}

    def login(self, data):
        """
        用户登录
        param data(dict) {
            key: "username", value: str
            key: "password", value: str
        }
        return: dict {
            key: "result", value: dict(data)
            key: "err", value: str    
        }
        """
        r = UserOperationObject.get_user_with_username(data)
        if r['err'] is not None:
            session.rollback()
            return {'result': None, "err": r['err']}
        session.commit()
        result: Users = r['result']
        if check_password_hash(result.password, data["password"]) is False:
            return {'result': None, "err": "用户名或密码错误"}
        results = {
            'username': result.username,
            'phonenumber': result.phonenumber,
            'id_num': result.id_num,
            'email': result.email,
            'token': create_token(result.id_num, result.is_shop, False),
            'is_shop': result.is_shop
        }
        return {"result": results, "err": None}

    def modify_userinfo(self, data):
        """
        修改用户信息
        params: 
        data(dict) {
            key: "id_num", value: str
            [Optional]key: "username", value: str
            [Optional]key: "password", value: str
            [Optional]key: "phonenumber", value: str
            [Optional]key: "email", value: str
            [Optional]key: "is_shop", value: Boolean
        }
        return:
        dict {
            key: "err", value: str    
        }
        """
        if 'password' in data:
            data["password"] = generate_password_hash(data["password"])
        r = UserOperationObject.modify_userinfo(data)
        if r['err'] is not None:
            session.rollback()
            return {"err": r['err']}
        session.commit()
        return {"err": None}

    def recharge(self, data):
        """
        用户充值
        params: 
        data(dict) {
            key: "id_num", value: str
            key: "amount", value: Decimal(10, 2)
            key: "bill_date", value: date
        }
        return:
        dict {
            key: "err", value: str
        }
        """
        r = AccountOperationObject. \
            increase_balance({"account_type": AccountType.user,
                              "id_num": data['id_num'],
                              "amount": data['amount']})
        if r['err'] is not None:
            session.rollback()
            return {"err": r['err']}
        r = BillOperationObject. \
            add_bill({"amount": data['amount'],
                      "sender_id": int(r['result'].account_id),
                      "receiver_id": int(r['result'].account_id),
                      "bill_date": data['bill_date'],
                      "bill_type": BillType.recharge,
                      })
        if r['err'] is not None:
            session.rollback()
            return {"err": r['err']}
        session.commit()
        return {"err": None}

    def show_account(self, data):
        """
        用户充值
        params: 
        data(dict) {
            key: "id_num", value: str
        }
        return: 
        dict {
            key: "result", value: {
                key: "amount", value: DECIMAL(10, 2)
                key: "username", value: str
                key: "phonenumber", value: str
                key: "email", value: str
            }
            key: "err", value: str    
        }
        """
        account_msg = AccountOperationObject. \
            get_balance({"account_type": AccountType.user,
                         "id_num": data['id_num']})
        if account_msg['err'] is not None:
            session.rollback()
            return {'result': None, 'err': account_msg['err']}
        user_msg = UserOperationObject.get_user_with_id({'id_num': data['id_num']})
        if user_msg['err'] is not None:
            session.rollback()
            return {'result': None, 'err': user_msg['err']}
        session.commit()
        result = {
            'amount': account_msg['amount'],
            'username': user_msg['result'].username,
            'phonenumber': user_msg['result'].phonenumber,
            'email': user_msg['result'].email
        }
        return {'result': result, 'err': None}

    def add_to_cart(self, data):
        """
        商品加入购物车
        params: 
        data(dict) {
            key: "id_num", value: str
            key: "good_id", value: int
            key: "count", value: int
        }
        return: 
        dict {
            key: "err", value: str    
        }
        """
        r = ShoppingCartOperationObject.add_good(data)
        if r['err'] is not None:
            session.rollback()
            return {"err": r['err']}
        session.commit()
        return {"err": None}

    def delete_from_cart(self, data):
        """
        从购物车删除商品
        params: 
        data(dict) {
            key: "id_num", value: str
            key: "goods", value: list(包含good_id的数组)
        }
        return: 
        dict {
            key: "err", value: str    
        }
        """
        for good_id in data['goods']:
            good_data = {
                'id_num': data['id_num'],
                'good_id': good_id
            }
            r = ShoppingCartOperationObject.delete_good(good_data)
            if r['err'] is not None:
                session.rollback()
                return {"err": r['err']}
        session.commit()
        return {"err": None}

    def show_bills(self, data):
        """
        展示账单
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
            {'account_type': AccountType.user,
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

    def show_cart(self, data):
        """
        展示购物车
        params:
        data(dict) {
            key: "id_num", value: str
        }
        return:
        dict {
            key: "result", value: list
            {
                key: "image", value: str
                key: "goodname", value: str
                key: "price", value: float
                key: "intro", value: str
                key: "good_id", value: int
                key: "count", value: int
                key: "goodamount", value: int
                key: "is_legal", value: bool
            }
            key: "err", value: str
        }
        """
        carts_msg = ShoppingCartOperationObject.show_cart(data)
        if carts_msg['err'] is not None:
            session.rollback()
            return {'result': None, 'err': carts_msg['err']}
        result = []
        for cm in carts_msg['result']:
            # 获取goodname
            data['good_id'] = cm.good_id
            good_msg = GoodOperationObject.get_good(data)
            if good_msg['err'] is not None:
                session.rollback()
                return {'result': None, 'err': good_msg['err']}
            r = good_msg['result'].to_dict()
            r['good_id'] = cm.good_id
            r['count'] = cm.count
            shop_msg = ShopOperationObject.get_shop({"shop_id": r['shop_id']})
            if shop_msg['err'] is not None:
                session.rollback()
                return {'result': None, 'err': shop_msg['err']}
            r['shop_name'] = shop_msg['result'].shopname

            result.append(r)
        session.commit()
        return {'result': {'goods': result, 'isnull': len(result) == 0, 'total': len(result)}, 'err': None}

    def update_cart(self, data):
        """
        修改购物车中商品
        params:
        data(dict) {
            key: "id_num", value: str
            key: "good_id", value: int
            key: "count", value: int
        }
        return:
        dict {
            key: "err", value: str
        }
        """
        r = ShoppingCartOperationObject.modify_good(data)
        if r['err'] is not None:
            session.rollback()
            return {"err": r['err']}
        session.commit()
        return {"err": None}

    def submit_order(self, data):
        """
        提交订单
        params:
        data(dict) {
            key: "id_num", value: string
            key: "goods", value: list[{"good_id", count}]
            key: "total_price", value: decimal
            key: "request_date", value: date
        }
        return:
        dict {
           key: "order_id", value: int
           key: "err", value: str
        }
        """
        # 1.新增订单
        # 2.新增订单商品
        r = OrderOperationObject.add_order({"id_num": data['id_num'], \
            "total": data['total_price'], "order_date": data['request_date']})
        if r['err'] is not None:
            session.rollback()
            return {"err": r["err"]}
        
        # 增加商品
        order_id = r['result'].order_id
        for good in data["goods"]:
            if int(good['count']) <= 0:
                session.rollback()
                return {"err": '商品数量小于0'}
            rr = OrderGoodOperationObject.add_good({"good_id": good["good_id"], \
                "order_id": order_id, "count": good["count"]})
            if rr['err'] is not None:
                session.rollback()
                return {"err": rr["err"]}
        session.commit()
        return {"order_id": order_id, "err": None}

    def cancel_order(self, data):
        """
        取消订单
        params:
        data(dict) {
            key: "order_id", value: int
        }
        return:
        dict {
           key: "err", value: str
        }
        """
        r = OrderOperationObject.modify_order({"order_id": data['order_id'], "order_type": OrderType.cancel})
        if r['err'] is not None:
            session.rollback()
            return {"err": r['err']}
        session.commit()
        return {"err": None}

    def pay_order(self, data):
        """
        支付订单
        params:
        data(dict) {
            key: "order_id", value: int
            key: "id_num", value: string
            key: "bill_date", value: date
        }
        return:
        dict {
           key: "err", value: str
        }
        """
        # 支付账单流程
        # 1.转帐到商户
        # 2.在账单流水中添加一项
        # 3.将订单状态改为已支付
        
        # 获取订单信息
        r = OrderOperationObject.get_order({'order_id': data['order_id']})
        if r['err'] is not None:
            session.rollback()
            return {'err': r['err']}
        order :Orders = r['result']
        
        # 从用户账户扣钱
        r = AccountOperationObject.decrease_balance({'account_type': AccountType.user,\
            'id_num': data['id_num'], 'amount': order.total})
        if r['err'] is not None:
            session.rollback()
            return {'err': "账户余额不足"}
        
        # 获取用户账户
        r = AccountOperationObject.get_account_id({"account_type": AccountType.user,\
            'id_num': data['id_num']})
        if r['err'] is not None:
            session.rollback()
            return {"err": r['err']}
        user_account_id = r['account_id']

        # 获取所有商品信息，并把钱一笔笔转入商户账户
        r = OrderGoodOperationObject.get_good({'order_id': data['order_id']})
        if r['err'] is not None:
            session.rollback()
            return {"err": r['err']}
        goods: list[OrderGoods] = r['results']
        for good in goods:
            # 获取商品单价
            rr = GoodOperationObject.get_good({"good_id": good.good_id})
            if rr['err'] is not None:
                session.rollback()
                return {"err": rr['err']}
            good_info: Goods = rr['result']
            # 将钱打入商户
            total = decimal.Decimal(good.count * good_info.price)
            rr = AccountOperationObject.increase_balance({'account_type': AccountType.shop,\
                'shop_id': good_info.shop_id, "amount": total})
            if rr['err'] is not None:
                session.rollback()
                return {"err": rr['err']}
            # 获取商户账户
            rr = AccountOperationObject.get_account_id({"account_type": AccountType.shop,\
            'shop_id': good_info.shop_id})
            if rr['err'] is not None:
                session.rollback()
                return {'err': rr['err']}
            # 在账单中添加流水
            rr = BillOperationObject.add_bill({'amount': total,\
                "sender_id": user_account_id, "receiver_id": rr["account_id"], \
                "bill_date": data['bill_date'], "bill_type": BillType.buy})
            if rr['err'] is not None:
                session.rollback()
                return {'err': rr['err']}

        #修改订单状态
        r = OrderOperationObject.modify_order({"order_id": data['order_id'], "order_type": OrderType.already})
        if r['err'] is not None:
            session.rollback()
            return {"err": r['err']}


        session.commit()
        return {"err": None}

    def show_order(self, data):
        """
        查看订单
        params:
        data(dict) {
            key: "id_num", value: string
        }
        return:
        dict {
           key: "results", value: list[...]
           key: "err", value: str
        }
        """
        # 获取订单
        r = OrderOperationObject.get_order({"id_num": data['id_num']})
        if r['err'] is not None:
            session.rollback()
            return {"err": r['err']}
        ret = []
        orders :list[Orders] = r["results"]
        for order in orders:
            ret.append(order.to_dict())
        return {"err": None, "results": ret}

    def get_order_by_id(self, data):
        r = OrderOperationObject.get_order({"order_id": data["order_id"]})
        if r['err'] is not None:
            session.rollback()
            return {"err": r['err']}
        order = r['result']
        # 获取订单商品
        rr = OrderGoodOperationObject.get_good({"order_id": order.order_id})
        if rr['err'] is not None:
            session.rollback()
            return {"err": r['err']}
        
        # 获取商品详细信息
        ordergoods: list[OrderGoods] = rr['results']
        goods = []
        for ordergood in ordergoods:
            rrr = GoodOperationObject.get_good({"good_id": ordergood.good_id})
            if rrr['err'] is not None:
                session.rollback()
                return {"err": r['err']}
            good: Goods = rrr['result']
            shop_msg = ShopOperationObject.get_shop({"shop_id": good.shop_id})
            if shop_msg["err"] is not None:
                session.rollback()
                return {"result": None, "err": shop_msg['err']}
            goods.append({"goodname": good.goodname, \
                "shopname": shop_msg['result'].shopname,
                "count": ordergood.count, \
                "price": float(good.price), \
                "image": good.images.split(',')[0]})

        ret = {"id": order.order_id, \
            "goods": goods, \
            "total": float(order.total), \
            "date": order.order_date, \
            "status": order.order_type.value}

        session.commit()
        return {"result": ret, "err": None}