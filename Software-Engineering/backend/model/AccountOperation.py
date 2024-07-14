from model.models import Accounts
from model.enum_type import AccountType
from model import session


class AccountOperation:
    def create_account(self, data):
        """
        创建账户
        params: 
        data(dict) {
            key: "account_type", value: AccountType
            [Optional]key: "id_num", value: str
            [Optional]key: "shop_id", value: int
        }
        return:
        dict {
            key: "result", value: Accounts
            key: "err", value: str    
        }
        """
        msg = self.__get_account(data)
        if msg['err'] is None:
            return {"result": None, "err": "账户已存在"}
        new_account = Accounts(data)
        try:
            session.add(new_account)
            session.flush()
            return {"result": new_account, "err": None}
        except Exception as e:
            print(e)
            return {"result": None, "err": str(e)}

    def get_balance(self, data):
        """
        获取账户余额
        params: 
        data(dict) {
            key: "account_type", value: AccountType
            [Optional]key: "id_num", value: str
            [Optional]key: "shop_id", value: int
        }
        return: 
        dict {
            key: "amount", value: DECIMAL(10, 2)
            key: "err", value: str    
        }
        """
        msg = self.__get_account(data)
        if msg['err'] is not None:
            return {"amount": None, "err": msg["err"]}
        return {"amount": msg['account'].amount, "err": None}

    def get_account_id(self, data):
        """
        获取账户id
        params:
        data(dict) {
            key: "account_type", value: AccountType
            [Optional]key: "id_num", value: str
            [Optional]key: "shop_id", value: int
        }
        return:
        dict {
            key: "account_id", value: int
            key: "err", value: str
        }
        """
        msg = self.__get_account(data)
        if msg['err'] is not None:
            return {"account_id": None, "err": msg["err"]}
        return {"account_id": msg['account'].account_id, "err": None}

    def increase_balance(self, data):
        """
        增加余额
        params: 
        data(dict) {
            key: "account_type", value: AccountType
            [Optional]key: "id_num", value: str
            [Optional]key: "shop_id", value: int
            key: "amount", value: DECIMAL(10, 2)
        } 
        return: 
        dict {
            key: "result", value: Accounts
            key: "err", value: str    
        }
        """
        try:
            r = self.__direct_modify_balance(data)
            r = self.__get_account(data)
            return {"result": r['account'], "err": None}
        except Exception as e:
            print(e)
            return {"result": None, "err": str(e)}

    def decrease_balance(self, data):
        """
        减少余额
        params: 
        data(dict) {
            key: "account_type", value: AccountType
            [Optional]key: "id_num", value: str
            [Optional]key: "shop_id", value: int
            key: "amount", value: DECIMAL(10, 2)
        } 
        return: 
        dict {
            key: "result", value: Accounts
            key: "err", value: str    
        }
        """
        r = self.__get_account(data)
        if r['account'].amount < data['amount']:
            return {"err": "账户余额不足"}

        data['amount'] = -data['amount']
        try:
            r = self.__direct_modify_balance(data)
            r = self.__get_account(data)
            return {"result": r['account'], "err": None}
        except Exception as e:
            print(e)
            return {"result": None, "err": str(e)}

    def __get_account(self, data):
        """
        从数据库中获取账户信息
        """
        r: Accounts
        try:
            if data['account_type'] == AccountType.user:
                r = session.query(Accounts). \
                    filter(Accounts.account_type == AccountType.user, Accounts.id_num == data['id_num']).first()
                if r is None:
                    return {"account": None, "err": "用户不存在"}
            elif data['account_type'] == AccountType.shop:
                r = session.query(Accounts). \
                    filter(Accounts.account_type == AccountType.shop, Accounts.shop_id == data['shop_id']).first()
                if r is None:
                    return {"account": None, "err": "店铺不存在"}
            elif data['account_type'] == AccountType.middle:
                r = session.query(Accounts). \
                    filter(Accounts.account_type == AccountType.middle).first()
            elif data['account_type'] == AccountType.mall:
                r = session.query(Accounts). \
                    filter(Accounts.account_type == AccountType.mall).first()
            else:
                return {"account": None, "err": "账户类型错误"}
        except Exception as e:
            print(e)
            return {"account": None, "err": str(e)}
        return {"account": r, "err": None}

    def __direct_modify_balance(self, data):
        """
        不经验证地直接改变余额
        """
        try:
            r: Accounts = None
            if data['account_type'] == AccountType.user:
                r = session.query(Accounts). \
                    filter(Accounts.account_type == AccountType.user, Accounts.id_num == data['id_num']). \
                    update({Accounts.amount: Accounts.amount + data['amount']})
            elif data['account_type'] == AccountType.shop:
                r = session.query(Accounts). \
                    filter(Accounts.account_type == AccountType.shop, Accounts.shop_id == data['shop_id']). \
                    update({Accounts.amount: Accounts.amount + data['amount']})
            elif data['account_type'] == AccountType.middle:
                r = session.query(Accounts). \
                    filter(Accounts.account_type == AccountType.middle). \
                    update({Accounts.amount: Accounts.amount + data['amount']})
            elif data['account_type'] == AccountType.mall:
                r = session.query(Accounts). \
                    filter(Accounts.account_type == AccountType.mall). \
                    update({Accounts.amount: Accounts.amount + data['amount']})
            return r
        except Exception as e:
            raise e
