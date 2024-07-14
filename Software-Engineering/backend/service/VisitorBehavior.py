from model import (
    session,
    ShopOperationObject,
    GoodOperationObject
)
from model.models import *

class VisitorBehavior:
    def show_shops(self, data):
        msg = ShopOperationObject.get_shop_limit(data)
        if msg['err'] is not None:
            session.rollback()
            return msg
        session.commit()
        return msg

    def show_shop_home(self, data):
        msg = ShopOperationObject.get_shop(data)
        if msg['err'] is not None:
            session.rollback()
            return msg
        session.commit()
        return msg

    def show_hot_goods(self, data):
        msg = GoodOperationObject.get_good_limit(data)
        if msg['err'] is not None:
            session.rollback()
            return msg
        session.commit()
        return msg

    def show_preference(self, data):
        msg = GoodOperationObject.get_good_limit(data)
        if msg['err'] is not None:
            session.rollback()
            return msg
        session.commit()
        return msg

    def search(self ,data):
        pass
    
    def show_all_shops(self):
        msg = ShopOperationObject.show_shops()
        if msg['err'] is not None:
            session.rollback()
            return msg
        session.commit()
        return msg

    def search_shops(self, data):
        pass
    
    def show_good_home(self, data):
        msg = GoodOperationObject.get_good(data)
        if msg['err'] is not None:
            session.rollback()
            return msg
        result: Goods = msg['result']
        shop_msg = ShopOperationObject.get_shop({"shop_id": result.shop_id})
        if shop_msg['err'] is not None:
            session.rollback()
            return shop_msg
        result = result.to_dict()
        result['shop_name'] = shop_msg['result'].shopname
        session.commit()
        return {"result": result, "err": None}

    def show_all_goods(self):
        msg = GoodOperationObject.show_goods({})
        if msg['err'] is not None:
            session.rollback()
            return msg
        session.commit()
        return msg

    def search_goods(self, data):
        pass

    def get_category(self, data):
        pass