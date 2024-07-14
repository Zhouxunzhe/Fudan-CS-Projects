from model.models import Shops, Goods
from model import session


class ShopOperation:
    """
    商店操作
    """
    def get_shop(self, data):
        """
        获取商店
        :param data:
        data(dict) {
            [option]key: "id_num", value: str
            [option]key: "shop_id", value: int
            [option]key: "is_open", value: boolean
        }
        :return:
        dict {
            key: "result", value: Shops(class)
            key: "err", value: str
        }
        """
        try:
            if "id_num" in data:
                if "is_open" in data:
                    r: Shops = session.query(Shops). \
                        filter(Shops.id_num == data['id_num'], Shops.is_open == data['is_open']).first()
                else:
                    r: Shops = session.query(Shops). \
                        filter(Shops.id_num == data['id_num']).first()
                return {'result': r, 'err': None}
            if "shop_id" in data:
                r: Shops = session.query(Shops). \
                    filter(Shops.shop_id == data['shop_id']).first()
                return {'result': r, 'err': None}
            else:
                return {'result': None, 'err': "没有信息用于获取商店"}
        except Exception as e:
            print(e)
            return {'result': None, 'err': str(e)}

    def add_shop(self, data):
        """
        添加商店
        :param data:
        data(dict) {
            key: "kind", value: str
            key: "shopname", value: str
            key: "id_num", value: str
            key: "intro", value: str
            key: "address", value: str
            key: "logo", value: str
            key: "register_capital", value: float
            key: "register_date", value: str
        }
        :return:
        dict {
            key: "result", value: Shops(class)
            key: "err", value: str
        }
        """
        new_shop = Shops(data)
        try:
            session.add(new_shop)
            session.flush()
            return {'result': new_shop, 'err': None}
        except Exception as e:
            print(e)
            return {'result': None, 'err': '重复开店'}
            # return {'result': None, 'err': str(e)}

    def delete_shop(self, data):
        """
        删除商店
        :param data:
        data(dict) {
            key: "shop_id", value: int
        }
        :return:
        dict {
            key: "err", value: str
        }
        """
        # 删除商店的约束判断
        # msg = self.__check_delete_shop__(data)
        # if msg is not None:
        #     return {'err': msg['err']}
        try:
            # 删除只是更改商店的状态
            session.query(Shops).filter(Shops.shop_id == data['shop_id']).\
                update({'is_open': False})
            return {'err': None}
        except Exception as e:
            print(e)
            return {'err': str(e)}

    def __check_delete_shop__(self, data):
        """
        检测商店删除条件的内置函数
        :param data:
        data(dict) {
            key: "shop_id", value: int
        }
        :return:
        dict {
            key: "err", value: str
        }
        """
        # TODO:删除商店约束判断
        # try:
        #     r: Orders = session.query(Orders).\
        #         filter(Orders.state == 'unfinished', Orders.shop_id == data['shop_id']).all()
        #     if r is not None:
        #         return {'err': "存在未完成的订单"}
        # except Exception as e:
        #     return {'err': str(e)}
        pass

    def modify_shop(self, data):
        """
        修改商品信息
        :param data:
        data(dict) {
            key: "good_id", value: int
            [option]key: "goodname", value: str
            [option]key: "price", value: float
            [option]key: "intro", value: str
            [option]key: "image", value: str
        }
        :return:
        dict {
            key: "result", value: Goods(class)
            key: "err", value: str
        }
        """
        try:
            if 'kind' in data:
                session.query(Shops).filter(Shops.shop_id == data['shop_id']).update({'kind': data['kind']})
            if 'shopname' in data:
                session.query(Shops).filter(Shops.shop_id == data['shop_id']).update({'shopname': data['shopname']})
            if 'intro' in data:
                session.query(Shops).filter(Shops.shop_id == data['shop_id']).update({'intro': data['intro']})
            if 'address' in data:
                session.query(Shops).filter(Shops.shop_id == data['shop_id']).update({'address': data['address']})
            if 'logo' in data:
                session.query(Shops).filter(Shops.shop_id == data['shop_id']).update({'logo': data['logo']})
            if 'is_open' in data:
                session.query(Shops).filter(Shops.shop_id == data['shop_id']).update({'is_open': data['is_open']})
            return {'err': None}
        except Exception as e:
            print(e)
            return {'err': str(e)}

    def show_shops(self):
        """
        展示商店
        :return:
        dict {
            key: "result", value: array
            key: "err", value: str
        }
        """
        try:
            results = session.query(Shops).filter(Shops.is_open == True).all()
            return {'result': results, 'err': None}
        except Exception as e:
            print(e)
            return {'result': None, 'err': str(e)}

    def get_shop_limit(self, data):
        """
        获取一定数量的商店
        :param data:
        data(dict) {
            key: "limit", value: int
        }
        :return:
        dict {
            key: "result", value: array
            key: "err", value: str
        }
        """
        try:
            results = session.query(Shops).filter(Shops.is_open == True).limit(data['limit']).all()
            return {'result': results, 'err': None}
        except Exception as e:
            print(e)
            return {'result': None, 'err': str(e)}

    def show_shop_home(self, data):
        """
        展示商店主页
        :param data:
        data(dict) {
            key: "shop_id", value: int
        }
        :return:
        dict {
            key: "result", value: array
            key: "err", value: str
        }
        """
        try:
            r: Shops = session.query(Shops).filter(Shops.shop_id == data['shop_id']).first()
            if r is None:
                return {'result': r, 'err': '商店不存在'}
            return {'result': r, 'err': None}
        except Exception as e:
            print(e)
            return {'result': None, 'err': str(e)}
