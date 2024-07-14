from model.RequestOperation import RequestOperation
from model.models import GoodRequests, Goods
from model import session


class GoodRequestOperation(RequestOperation):
    def get_request(self, data):
        """
        获取请求
        params:
        data(dict) {
            key: "request_id", value: int
        }
        return:
        dict {
            key: "result", value: GoodRequests
            key: "err", value: str
        }
        """
        try:
            r: GoodRequests = session.query(GoodRequests). \
                filter(GoodRequests.request_id == data['request_id']).first()
            return {"result": r, "err": None}
        except Exception as e:
            print(e)
            return {"result": None, "err": str(e)}

    def add_request(self, data):
        """
        添加请求
        params: 
        data(dict) {
            key: "good_id", value: int
            key: "request_type", value: RequestType
            key: "request_date", value: str
            [Optional]key: "info", value: str
        }
        return:
        dict {
            key: "result", value: GoodRequests
            key: "err", value: str    
        }
        """
        new_request = GoodRequests(data)
        try:
            session.add(new_request)
            session.flush()
            return {"result": new_request, "err": None}
        except Exception as e:
            print(e)
            return {"result": None, "err": str(e)}

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
            key: "result", value: GoodRequests
            key: "err", value: str    
        }
        """
        try:
            r = session.query(GoodRequests). \
                filter(GoodRequests.request_id == data['request_id']). \
                update({"comment": data['comment'], "is_check": True})
            return {"result": r, "err": None}
        except Exception as e:
            print(e)
            return {"result": None, "err": str(e)}

    def show_requests(self, data):
        """
        展示请求
        params: 
        data(dict) {
            key: "is_admin", value: Boolean
            [Optional]key: "shop_id", value: int
        }
        return:
        dict {
            key: "results", value: list[GoodRequests]
            key: "err", value: str    
        }
        """
        try:
            # 管理员查询整张表
            if data['is_admin'] is True:
                r = session.query(GoodRequests).all()
                return {"results": r, "err": None}

            # 商户只能查到自己有关的项
            r = session.query(GoodRequests). \
                join(Goods, Goods.good_id == GoodRequests.good_id). \
                filter(Goods.shop_id == data['shop_id']).all()
            return {"results": r, "err": None}

        except Exception as e:
            return {"results": None, "err": str(e)}
