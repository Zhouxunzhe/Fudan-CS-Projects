from model.models import OrderGoods
from model import session

class OrderGoodsOperation:
    
    def add_good(self, data):
        """
        添加商品
        params:
        data(dict) {
            key: "good_id", value: int
            key: "order_id", value: int
            key: "count", value: int
        }
        return:
        dict {
           key: "result", value: OrderGoods
           key: "err", value: str
        }
        """
        new_good = OrderGoods(data)
        try:
            session.add(new_good)
            session.flush()
            return {"result": new_good, "err": None}
        except Exception as e:
            print(e)
            return {"result": None, "err": str(e)}
    
    def get_good(self, data):
        """
        获取商品
        params:
        data(dict) {
            key: "order_id", value: int
        }
        return:
        dict {
           key: "results", value: list[OrderGoods]
           key: "err", value: str
        }
        """
        try: 
            r = session.query(OrderGoods). \
                filter(OrderGoods.order_id == data['order_id']).all()
            return {'results': r, 'err': None}
        except Exception as e:
            print(e)
            return {'result': None, 'err': str(e)}