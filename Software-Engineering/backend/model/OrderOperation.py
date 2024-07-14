from model.models import Orders
from model.enum_type import OrderType
from model import session


class OrderOperation:
    """
    订单操作
    """

    def get_order(self, data):
        """
        获取订单信息
        data(dict) {
            [optional]key: "id_num", value: str
            [optional]key: "order_id", value: str
        }
        return:
        dict {
            key: "results", value: list[Orders] 
        or  key: "result", value: Orders
            key: "err", value: str
        }
        """
        try:
            if 'id_num' in data:
                r = session.query(Orders). \
                    filter(Orders.id_num == data['id_num']).all()
                return {"results": r, "err": None}
            elif 'order_id' in data:
                r = session.query(Orders). \
                    filter(Orders.order_id == data['order_id']).first()
                return {"result": r, "err": None}
            else:
                return {"results": None, "err": '参数错误'}
        except Exception as e:
            print(e)
            return {"results": None, "err": str(e)}

    def add_order(self, data):
        """
        添加订单
        params:
        data(dict) {
            key: "id_num", value: str
            key: "total", value: DECIMAL(10, 2)
            key: order_date, value: date
            [optional]key:note, value: str 
        }
        return:
        dict {
           key: "result", value: Orders
           key: "err", value: str
        }
        """
        data['order_type'] = OrderType.ready
        new_order = Orders(data)
        try:
            session.add(new_order)
            session.flush()
            return {"result": new_order, "err": None}
        except Exception as e:
            print(e)
            return {"result": None, "err": str(e)}

    def modify_order(self, data):
        """
        修改订单
        仅限将订单从待支付状态改为已支付或撤销状态
        params:
        data(dict) {
            key: order_id, value: int
            key: order_type, value: OrderType
        }
        return:
        dict {
           key: "result", value: Orders
           key: "err", value: str
        }
        """
        try:
            r = session.query(Orders). \
                filter(Orders.order_id == data['order_id']). \
                update({'order_type': data['order_type']})
            return {'result': r, 'err': None}
        except Exception as e:
            print(e)
            return {"err": str(e)}
