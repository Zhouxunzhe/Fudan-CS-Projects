from model.models import ShoppingCarts
from model import session


class ShoppingCartOperation:
    """
    购物车操作
    """
    def get_count(self, data):
        """
        获取指定商品的数量
        :param data:
        data(dict) {
            key: "id_num", value: str
            key: "good_id", value: int
        }
        :return:
        dict {
            key: "count", value: int
            key: "err", value: str
        }
        """
        try:
            r: ShoppingCarts = session.query(ShoppingCarts). \
                filter(ShoppingCarts.good_id == data['good_id'],
                       ShoppingCarts.id_num == data['id_num']).first()
            return {'result': r.count, 'err': None}
        except Exception as e:
            print(e)
            return {'result': None, 'err': str(e)}

    def add_good(self, data):
        """
        向购物车中添加商品
        :param data:
        data(dict) {
            key: "id_num", value: str
            key: "good_id", value: int
            key: "count", value: int
        }
        :return:
        dict {
            key: "result", value: ShoppingCarts(class)
            key: "err", value: str
        }
        """
        try:
            r: ShoppingCarts = session.query(ShoppingCarts). \
                filter(ShoppingCarts.good_id == data['good_id'],
                       ShoppingCarts.id_num == data['id_num']).first()
        except Exception as e:
            return {'result': None, 'err': str(e)}
        if r is None:
            new_shoppingcart = ShoppingCarts(data)
            try:
                session.add(new_shoppingcart)
                session.flush()
                return {'result': new_shoppingcart, 'err': None}
            except Exception as e:
                print(e)
                return {'result': None, 'err': "商品已在购物车内，请去购物车查看"}
        else:
            data['count'] += r.count
            msg = self.modify_good(data)
            if msg['err'] is not None:
                return {'result': msg['result'], 'err': None}
            else:
                return {'result': None, 'err': msg['err']}

    def delete_good(self, data):
        """
        删除购物车中指定商品
        :param data:
        data(dict) {
            key: "id_num", value: str
            key: "good_id", value: int
        }
        :return:
        dict {
            key: "err", value: str
        }
        """
        try:
            r: ShoppingCarts = session.query(ShoppingCarts).\
                filter(ShoppingCarts.good_id == data['good_id'],
                       ShoppingCarts.id_num == data['id_num']).first()
            if r is None:
                return {"err": '没有该项购物车商品'}
            session.delete(r)
            return {'err': None}
        except Exception as e:
            print(e)
            return {'err': str(e)}

    def modify_good(self, data):
        """
        修改商品的数量
        :param data:
        data(dict) {
            key: "id_num", value: str
            key: "good_id", value: int
            key: "count", value: int
        }
        :return:
        dict {
            key: "result", value: ShoppingCarts
            key: "err", value: str
        }
        """
        try:
            r: ShoppingCarts = session.query(ShoppingCarts).\
                filter(ShoppingCarts.good_id == data['good_id'],
                       ShoppingCarts.id_num == data['id_num']).\
                update({'count': data['count']})
            return {'result': r, 'err': None}
        except Exception as e:
            print(e)
            return {'result': None, 'err': str(e)}

    def show_cart(self, data):
        """
        展示购物车
        :param data:
        data(dict) {
            key: "id_num", value: str
        }
        :return:
        dict {
            key: "result", value: list
            key: "err", value: str
        }
        """
        try:
            r = session.query(ShoppingCarts).\
                filter(ShoppingCarts.id_num == data['id_num']).all()
            return {'result': r, 'err': None}
        except Exception as e:
            print(e)
            return {'result': None, 'err': str(e)}
