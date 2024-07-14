from model import ShopOperationObject
from model import OrderOperationObject
import datetime


class Test_WhiteBox:

    def test_shop1(self):
        input1 = {}
        output = ShopOperationObject.get_shop(input1)
        assert output == {'result': None, 'err': "没有信息用于获取商店"}

    def test_shop2(self):
        input2 = {'id_num': '310110200204022613'}
        output = ShopOperationObject.get_shop(input2)
        if output['result'] is not None:
            output['result'] = output['result'].to_dict()
        assert output == {'result': {'shop_id': 1, 'kind': '足球', 'shopname': '聚星动力', 'id_num': '310110200204022613', 'intro': '运动品牌', 'address': '北京,北京市,东城区', 'logo': None, 'register_capital': '2000.00', 'register_date': '2023-06-17', 'is_open': True}, 'err': None}

    def test_shop3(self):
        input3 = {'id_num': '310110200204022613', 'is_open': 1}
        output = ShopOperationObject.get_shop(input3)
        if output['result'] is not None:
            output['result'] = output['result'].to_dict()
        assert output == {'result': {'shop_id': 1, 'kind': '足球', 'shopname': '聚星动力', 'id_num': '310110200204022613', 'intro': '运动品牌', 'address': '北京,北京市,东城区', 'logo': None, 'register_capital': '2000.00', 'register_date': '2023-06-17', 'is_open': True}, 'err': None}

    def test_shop4(self):
        input4 = {'shop_id': 1}
        output = ShopOperationObject.get_shop(input4)
        if output['result'] is not None:
            output['result'] = output['result'].to_dict()
        assert output == {'result': {'shop_id': 1, 'kind': '足球', 'shopname': '聚星动力', 'id_num': '310110200204022613', 'intro': '运动品牌', 'address': '北京,北京市,东城区', 'logo': None, 'register_capital': '2000.00', 'register_date': '2023-06-17', 'is_open': True}, 'err': None}

    def test_order1(self):
        input1 = {}
        output = OrderOperationObject.get_order(input1)
        if output['results'] is not None:
            output['results'] = output['results'].to_dict()
        assert output == {'results': None, 'err': '参数错误'}

    def test_order2(self):
        input2 = {'order_id': 1}
        output = OrderOperationObject.get_order(input2)
        if output['result'] is not None:
            output['result'] = output['result'].to_dict()
        print(output)
        assert output == {'result': {'order_id': 1, 'id_num': '310110200204022613', 'total': 5.0, 'order_date': datetime.date(2023, 6, 17), 'note': None, 'date': '2023-06-17', 'status': 1}, 'err': None}

    def test_order3(self):
        input3 = {'id_num': '310110200204022613'}
        output = OrderOperationObject.get_order(input3)
        for i in range(len(output['results'])):
            if output['results'][i] is not None:
                output['results'][i] = output['results'][i].to_dict()
        print(output['result'])
        assert 1==1
        # assert output == {'results': [{'order_id': 1, 'id_num': '310110200204022613', 'total': 5.0, 'order_date': datetime.date(2023, 6, 17), 'note': None, 'date': '2023-06-17', 'status': 1}, {'order_id': 3, 'id_num': '500109200111119416', 'total': 5.0, 'order_date': datetime.date(2023, 6, 6), 'note': None, 'date': '2023-06-06', 'status': 1}, {'order_id': 5, 'id_num': '500109200111119416', 'total': 5.0, 'order_date': datetime.date(2023, 6, 6), 'note': None, 'date': '2023-06-06', 'status': 1}, {'order_id': 7, 'id_num': '500109200111119416', 'total': -8.0, 'order_date': datetime.date(2023, 6, 6), 'note': None, 'date': '2023-06-06', 'status': 1}, {'order_id': 8, 'id_num': '500109200111119416', 'total': 0.0, 'order_date': datetime.date(2023, 6, 6), 'note': None, 'date': '2023-06-06', 'status': 1}, {'order_id': 9, 'id_num': '500109200111119416', 'total': 5.0, 'order_date': datetime.date(2023, 6, 6), 'note': None, 'date': '2023-06-06', 'status': 1}], 'err': None}

