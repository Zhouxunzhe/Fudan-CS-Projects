from model.models import Superusers
from model import session


class SuperUserOperation:
    def get_user(self, data):
        """
        获取管理员用户
        :param data:
        data(dict) {
            key: "username", value: str
        }
        :return:
        dict {
            key: "result", value: Superusers(class)
            key: "err", value: str
        }
        """
        try:
            r: Superusers = session.query(Superusers). \
                filter(Superusers.username == data['username']).first()
            if r is None:
                return {"result": None, 'err': "用户不存在"}
            return {"result": r, 'err': None}
        except Exception as e:
            print(e)
            return {"result": None, 'err': str(e)}
