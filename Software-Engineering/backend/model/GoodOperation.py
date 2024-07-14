from model.models import Goods, TempGoods
from model import session


class GoodOperation:
    """
    商品操作
    """
    def get_good(self, data):
        """
        添加商品
        :param data:
        data(dict) {
            key: "good_id", value: int
        }
        :return:
        dict {
            key: "result", value: Goods(class)
            key: "err", value: str
        }
        """
        try:
            r: Goods = session.query(Goods).filter(Goods.good_id == data['good_id']).first()
            return {'result': r, 'err': None}
        except Exception as e:
            print(e)
            return {'result': None, 'err': str(e)}

    def get_good_limit(self, data):
        """
        获取一定数量的商品
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
            results = session.query(Goods).filter(Goods.is_legal == True).limit(data['limit']).all()
            return {'result': results, 'err': None}
        except Exception as e:
            print(e)
            return {'result': None, 'err': str(e)}

    def add_good(self, data):
        """
        添加商品
        :param data:
        data(dict) {
            key: "shop_id", value: int
            key: "goodname", value: str
            key: "price", value: float
            key: "goodamount", value: int
            key: "intro", value: str
            key: "images", value: str
        }
        :return:
        dict {
            key: "result", value: Goods(class)
            key: "err", value: str
        }
        """
        new_good = Goods(data)
        try:
            session.add(new_good)
            session.flush()
            return {'result': new_good, 'err': None}
        except Exception as e:
            print(e)
            return {'result': None, 'err': str(e)}

    def delete_good(self, data):
        """
        删除商品
        :param data:
        data(dict) {
            key: "good_id", value: int
        }
        :return:
        dict {
            key: "err", value: str
        }
        """
        try:
            # 并不直接删除商品,而是标记失效
            session.query(Goods).filter(Goods.good_id == data['good_id']).\
                update({'is_legal': False})
            return {'err': None}
        except Exception as e:
            print(e)
            return {'err': str(e)}

    def modify_good(self, data):
        """
        修改商品信息
        :param data:
        data(dict) {
            key: "good_id", value: int
            [option]key: "is_legal", value: bool
            [option]key: "update", value: bool
        }
        :return:
        dict {
            key: "result", value: Goods(class)
            key: "err", value: str
        }
        """
        try:
            if 'is_legal' in data:
                session.query(Goods).filter(Goods.good_id == data['good_id']).update({'is_legal': data['is_legal']})

            if 'update' in data and data['update']:
                # 检索未被更新的临时商品
                temp_good: TempGoods = session.query(TempGoods).filter(
                    TempGoods.good_id == data['good_id'], TempGoods.is_updated == False).first()
                current_good: Goods = session.query(Goods).filter(Goods.good_id == data['good_id']).first()

                if temp_good.goodname != current_good.goodname:
                    session.query(Goods).filter(Goods.good_id == data['good_id']).update({'goodname': temp_good.goodname})
                if temp_good.intro != current_good.intro:
                    session.query(Goods).filter(Goods.good_id == data['good_id']).update({'intro': temp_good.intro})
                if temp_good.price != current_good.price:
                    session.query(Goods).filter(Goods.good_id == data['good_id']).update({'price': temp_good.price})
                if temp_good.images != current_good.images:
                    session.query(Goods).filter(Goods.good_id == data['good_id']).update({'images': temp_good.images})

                # 将已更新的临时商品的状态更新
                session.query(TempGoods).filter(TempGoods.good_id == data['good_id'], TempGoods.is_updated == False)\
                    .update({'is_updated': True})
            return {'err': None}
        except Exception as e:
            print(e)
            return {'err': str(e)}

    def modify_temp_good(self, data):
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
            r: Goods = session.query(Goods).filter(Goods.good_id == data['good_id']).first()
        except Exception as e:
            print(e)
            return {'result': None, 'err': str(e)}
        try:
            new_good = {
                'good_id': r.good_id,
                'shop_id': r.shop_id,
                'goodname': data['goodname'] if 'goodname' in data else r.goodname,
                'price': data['price'] if 'price' in data else r.price,
                'goodamount': r.goodamount,
                'intro': data['intro'] if 'intro' in data else r.intro,
                'images': data['images'] if 'images' in data else r.images,
                'is_legal': r.is_legal
            }
            new_temp_good = TempGoods(new_good)
            return {'err': None}
        except Exception as e:
            print(e)
            return {'err': str(e)}

    def show_goods(self, data):
        """
        展示商品:
        分类讨论:如果data包含shop_id则展示该商店所有商品
        如果不包含shop_id则展示所有商品
        :param data:
        data(dict) {
            [option]key: "shop_id", value: int
        }
        :return:
        dict {
            key: "result", value array
            key: "err", value: str
        }
        """
        try:
            if 'shop_id' in data:
                results = session.query(Goods).\
                    filter(Goods.shop_id == data['shop_id'], Goods.is_legal == True).all()
                return {'result': results, 'err': None}
            else:
                results = session.query(Goods).filter().all()
                return {'result': results, 'err': None}
        except Exception as e:
            print(e)
            return {'result': None, 'err': str(e)}

    def delete_shop_good(self, data):
        """
        删除商店的内置函数
        :param data:
        data(dict) {
            key: "shop_id", value: int
        }
        :return:
        dict {
            key: "err", value: str
        }
        """
        try:
            session.query(Goods).filter(Goods.shop_id == data['shop_id']). \
                update({'is_legal': False})
            return {'err': None}
        except Exception as e:
            print(e)
            return {'err': str(e)}
