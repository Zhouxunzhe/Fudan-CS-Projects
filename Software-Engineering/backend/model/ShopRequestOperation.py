from model.RequestOperation import RequestOperation
from model import session
from model.models import ShopRequests, Shops


class ShopRequestOperation(RequestOperation):
    def get_request(self, data):
        """
        添加请求
        params:
        data(dict) {
           key: "request_id", value: int
        }
        return:
        dict {
           key: "result", value: ShopRequests
           key: "err", value: str
        }
        """
        try:
            r: ShopRequests = session.query(ShopRequests). \
                filter(ShopRequests.request_id == data['request_id']).first()
            return {'result': r, "err": None}
        except Exception as e:
            print(e)
            return {'result': None, "err": str(e)}

    def add_request(self, data):
        """
        添加请求
        params: 
        data(dict) {
            key: "shop_id", value: int
            key: "request_type", value: RequestType
            key: "request_date", value: str
            [Optional]key: "info", value: str
        }
        return:
        dict {
            key: "result", value: ShopRequests
            key: "err", value: str    
        }
        """
        new_request = ShopRequests(data)
        try:
            session.add(new_request)
            session.flush()
            return {"result": new_request, "err": None}
        except Exception as e:
            print(e)
            return {"result": new_request, "err": str(e)}

    def check_request(self, data):
        """
        审批请求
        params: 
        data(dict) {
            key: "request_id", value: int
            key: "comment", value: CommentType
        }
        return:
        dict {
            key: "err", value: str    
        }
        """
        # TODO:将comment转换为str/int
        try:
            r = session.query(ShopRequests). \
                filter(ShopRequests.request_id == data['request_id']). \
                update({"is_check": True, "comment": data['comment']})
            return {"err": None}
        except Exception as e:
            print(e)
            return {"err": str(e)}

    def show_requests(self, data):
        """
        展示请求
        params: 
        data(dict) {
            key: "is_admin", value: Boolean
            [Optional]key: "id_num", value: int
        }
        return:
        dict {
            key: "results", value: list[ShopRequests]
            key: "err", value: str    
        }
        """
        try:
            # 管理员查询整张表
            if data['is_admin'] is True:
                r = session.query(ShopRequests).all()
                return {"results": r, "err": None}

            # 商户只能查到自己有关的项
            r = session.query(ShopRequests). \
                join(Shops, Shops.shop_id == ShopRequests.shop_id). \
                filter(Shops.id_num == data['id_num']).all()
            return {"results": r, "err": None}

        except Exception as e:
            print(e)
            return {"results": None, "err": str(e)}

    def get_current_request(self, data):
        """
        展示当前请求
        params:
        data(dict) {
            key: "id_num", value: int
        }
        return:
        dict {
            key: "results", value: list[ShopRequests]
            key: "err", value: str
        }
        """
        try:
            r = session.query(ShopRequests). \
                join(Shops, Shops.shop_id == ShopRequests.shop_id). \
                filter(Shops.id_num == data['id_num'], ShopRequests.is_check == False).all()
            return {"results": r, "err": None}

        except Exception as e:
            return {"results": None, "err": str(e)}
