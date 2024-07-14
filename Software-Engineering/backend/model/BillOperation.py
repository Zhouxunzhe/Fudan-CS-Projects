from model.models import Bills
from model import session


class BillOperation:
    def add_bill(self, data):
        """
        生成账单
        params:
        data(dict) {
            key: "amount", value: decimal
            key: "sender_id", value: int
            key: "receiver_id", value: int
            key: "bill_date", value: date
            key: "bill_type", value: BillType
        }
        return:
        dict {
            key: "result", value: Bills
            key: "err", value: str
        }
        """
        new_bill = Bills(data)
        try:
            session.add(new_bill)
            session.flush()
            return {"result": new_bill, "err": None}
        except Exception as e:
            print(e)
            return {"result": None, "err": str(e)}

    def show_bills(self, data):
        """
        展示账单
        params:
        data(dict) {
            key: "account_id", value: int
        }
        return:
        dict {
            key: "result", value: list[Bills]
            key: "err", value: str
        }
        """
        try:
            r_sender = session.query(Bills). \
                filter(Bills.sender_id == data['account_id']).all()
            r_receiver = session.query(Bills). \
                filter(Bills.receiver_id == data['account_id']).all()
            r = r_sender + r_receiver
            return {"result": r, "err": None}
        except Exception as e:
            return {"result": None, "err": str(e)}
    