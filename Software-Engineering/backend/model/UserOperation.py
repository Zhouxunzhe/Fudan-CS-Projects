from model.models import Users
from model import session


class UserOperation:
    def create_user(self, data):
        """
        创建用户
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
        new_user = Users(data)
        try:
            session.add(new_user)
            session.flush()
            return {'err': None}
        except Exception as e:
            print(e)
            return {'err': "该用户名或手机号或身份证号或邮箱已被注册"}

    def get_user_with_username(self, data):
        """
        根据用户名查询用户信息
        params: 
        data(dict) {
            key: "username", value: str
        }
        return:
        dict {
            key: "result", value: Users
            key: "err", value: str    
        }
        """
        try:
            r: Users = session.query(Users). \
                filter(Users.username == data['username']).first()
            if r is None:
                return {"result": r, 'err': "用户不存在"}
            return {"result": r, "err": None}
        except Exception as e:
            print(e)
            return {"result": None, "err": "奇怪的事情发生了"}

    # TODO 应该改进获取方法减少冗余
    def get_user_with_id(self, data):
        """
        根据用户名查询用户信息
        params:
        data(dict) {
            key: "id_num", value: str
        }
        return:
        dict {
            key: "result", value: Users
            key: "err", value: str
        }
        """
        try:
            r: Users = session.query(Users). \
                filter(Users.id_num == data['id_num']).first()
            if r is None:
                return {"result": r, 'err': "用户不存在"}
            return {"result": r, "err": None}
        except Exception as e:
            print(e)
            return {"result": None, "err": "奇怪的事情发生了"}

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
            key: "result", value: Users
            key: "err", value: str    
        }
        """
        try:
            r: Users = session.query(Users). \
                filter(Users.id_num == data.pop('id_num')). \
                update(data)
            return {"result": r, "err": None}
        except Exception as e:
            print(e)
            return {"result": None, "err": "该用户名或手机号或身份证号或邮箱已被占用"}
