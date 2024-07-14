"""
该文件用于创建和数据库对应的数据模型类
它们是程序和数据库沟通的桥梁
"""
from model import Base
from model.enum_type import RequestType, CommentType, AccountType, BillType, OrderType

from sqlalchemy import (
    PrimaryKeyConstraint,
    Column,
    ForeignKey,
    String,
    Boolean,
    Integer,
    DECIMAL,
    DATE,
    Enum,
    Text
)


class Users(Base):
    __tablename__ = "users"
    id_num = Column(String(18), primary_key=True, unique=True)
    username = Column(String(10), nullable=True, unique=True)
    phonenumber = Column(String(11), nullable=True, unique=True)
    email = Column(String(256), nullable=False, unique=True)
    password = Column(String(102), nullable=False)
    is_shop = Column(Boolean(), nullable=False)

    def __init__(self, data):
        self.id_num = data['id_num']
        self.username = data['username']
        self.phonenumber = data['phonenumber']
        self.email = data['email']
        self.password = data['password']
        self.is_shop = data['is_shop']

    def to_dict(self):
        names = ['id_num', 'username', 'phonenumber', 'email', 'password', 'is_shop']
        return dict((name, getattr(self, name)) for name in names if not name.startswith('__'))


class Superusers(Base):
    __tablename__ = "superusers"
    username = Column(String(10), primary_key=True, unique=True)
    password = Column(String(102), nullable=False)

    def __init__(self, data):
        self.username = data['username']
        self.password = data['password']

    def to_dict(self):
        names = ['username', 'password']
        return dict((name, getattr(self, name)) for name in names if not name.startswith('__'))


class Shops(Base):
    __tablename__ = "shops"
    shop_id = Column(Integer, primary_key=True, autoincrement=True)
    kind = Column(String(256), nullable=False)
    shopname = Column(String(12), nullable=False, unique=True)
    id_num = Column(String(18), ForeignKey('users.id_num', ondelete='RESTRICT'), nullable=False)
    intro = Column(String(18))
    address = Column(String(32), nullable=False)
    logo = Column(Text)
    register_capital = Column(DECIMAL(10, 2), nullable=False)
    register_date = Column(DATE(), nullable=False)
    is_open = Column(Boolean(), nullable=False, default=False)

    def __init__(self, data):
        self.kind = data['kind']
        self.shopname = data['shopname']
        self.id_num = data['id_num']
        self.intro = data['intro']
        self.address = data['address']
        self.register_capital = data['register_capital']
        self.register_date = data['register_date']
        if 'logo' in data:
            self.logo = data['logo']
        if 'is_open' in data:
            self.is_open = data['is_open']
        else:
            self.is_open = False

    def to_dict(self):
        names = ['shop_id', 'kind', 'shopname', 'id_num', 'intro', 'address', 'logo', 'register_capital',
                 'register_date', 'is_open']
        ret = dict((name, getattr(self, name)) for name in names if not name.startswith('__'))
        ret['register_date'] = str(ret['register_date'])
        ret['register_capital'] = str(ret['register_capital'])
        return ret


class ShopRequests(Base):
    __tablename__ = "shop_requests"
    request_id = Column(Integer, primary_key=True, autoincrement=True)
    request_type = Column(Enum(RequestType), nullable=False)
    shop_id = Column(Integer, ForeignKey('shops.shop_id', ondelete='RESTRICT'), nullable=False)
    is_check = Column(Boolean(), nullable=False, default=False)
    comment = Column(Enum(CommentType))
    request_date = Column(DATE(), nullable=False)
    info = Column(String(256))

    def __init__(self, data):
        if 'request_id' in data:
            self.request_id = data['request_id']
        self.request_type = data['request_type']
        self.shop_id = data['shop_id']
        if 'is_check' in data:
            self.is_check = data['is_check']
        else:
            self.is_check = False
        if 'comment' in data:
            self.comment = data['comment']
        self.request_date = data['request_date']
        if 'info' in data:
            self.info = data['info']

    def to_dict(self):
        names = ['request_id', 'request_type', 'shop_id', 'is_check', 'comment', 'request_date', 'info']
        ret = dict((name, getattr(self, name)) for name in names if not name.startswith('__'))
        ret['request_date'] = str(ret['request_date'])
        ret['request_type'] = ret['request_type'].value
        ret['comment'] = ret['comment'].value
        return ret


class Goods(Base):
    __tablename__ = "goods"
    good_id = Column(Integer, primary_key=True, autoincrement=True)
    shop_id = Column(Integer, ForeignKey('shops.shop_id', ondelete='RESTRICT'), nullable=False)
    goodname = Column(String(256), nullable=False)
    price = Column(DECIMAL(10, 2), nullable=False)
    goodamount = Column(Integer(), nullable=False, default=0)
    intro = Column(String(128), nullable=False)
    images = Column(Text)
    is_legal = Column(Boolean(), nullable=False, default=False)

    def __init__(self, data):
        if 'good_id' in data:
            self.good_id = data['good_id']
        self.shop_id = data['shop_id']
        self.goodname = data['goodname']
        self.price = data['price']
        if 'goodamount' in data:
            self.goodamount = data['goodamount']
        else:
            self.goodamount = 0
        self.intro = data['intro']
        if 'images' in data:
            self.images = data['images']
        if 'is_legal' in data:
            self.is_legal = data['is_legal']
        else:
            self.is_legal = False

    def to_dict(self):
        names = ['good_id', 'shop_id', 'goodname', 'price', 'goodamount', 'intro', 'images', 'is_legal']
        ret = dict((name, getattr(self, name)) for name in names if not name.startswith('__'))
        ret['price'] = float(ret['price'])
        ret['images'] = ret['images'].split(',')
        return ret


class TempGoods(Base):
    __tablename__ = "temp_goods"
    temp_good_id = Column(Integer, primary_key=True, autoincrement=True)
    good_id = Column(Integer, ForeignKey('goods.good_id', ondelete='RESTRICT'), nullable=False)
    shop_id = Column(Integer, ForeignKey('shops.shop_id', ondelete='RESTRICT'), nullable=False)
    goodname = Column(String(256), nullable=False)
    price = Column(DECIMAL(10, 2), nullable=False)
    goodamount = Column(Integer(), nullable=False, default=0)
    intro = Column(String(128), nullable=False)
    images = Column(Text)
    is_legal = Column(Boolean(), nullable=False, default=False)
    is_updated = Column(Boolean(), nullable=False, default=False)

    def __init__(self, data):
        if 'temp_good_id' in data:
            self.temp_good_id = data['temp_good_id']
        self.good_id = data['good_id']
        self.shop_id = data['shop_id']
        self.goodname = data['goodname']
        self.price = data['price']
        if 'goodamount' in data:
            self.goodamount = data['goodamount']
        else:
            self.goodamount = 0
        self.intro = data['intro']
        if 'images' in data:
            self.images = data['images']
        if 'is_legal' in data:
            self.is_legal = data['is_legal']
        else:
            self.is_legal = False

    def to_dict(self):
        names = ['good_id', 'shop_id', 'goodname', 'price', 'goodamount', 'intro', 'images', 'is_legal']
        ret = dict((name, getattr(self, name)) for name in names if not name.startswith('__'))
        ret['price'] = float(ret['price'])
        ret['images'] = ret['images'].split(',')
        return ret


class GoodRequests(Base):
    __tablename__ = "good_requests"
    request_id = Column(Integer, primary_key=True, autoincrement=True)
    request_type = Column(Enum(RequestType), nullable=False)
    good_id = Column(Integer, ForeignKey('goods.good_id', ondelete='RESTRICT'), nullable=False)
    is_check = Column(Boolean(), nullable=False, default=False)
    comment = Column(Enum(CommentType))
    request_date = Column(DATE(), nullable=False)
    info = Column(String(256))

    def __init__(self, data):
        if 'request_id' in data:
            self.request_id = data['request_id']
        self.request_type = data['request_type']
        self.good_id = data['good_id']
        if 'is_check' in data:
            self.is_check = data['is_check']
        else:
            self.is_check = False
        if 'comment' in data:
            self.comment = data['comment']
        self.request_date = data['request_date']
        if 'info' in data:
            self.info = data['info']

    def to_dict(self):
        names = ['request_id', 'request_type', 'good_id', 'is_check', 'comment', 'request_date', 'info']
        ret = dict((name, getattr(self, name)) for name in names if not name.startswith('__'))
        ret['request_date'] = str(ret['request_date'])
        ret['request_type'] = ret['request_type'].value
        ret['comment'] = ret['comment'].value
        return ret


class Accounts(Base):
    __tablename__ = "accounts"
    account_id = Column(Integer, primary_key=True, autoincrement=True)
    id_num = Column(String(18), ForeignKey('users.id_num', ondelete='RESTRICT'), nullable=False, unique=True)
    shop_id = Column(Integer, ForeignKey('shops.shop_id', ondelete='RESTRICT'), nullable=False, unique=True)
    account_type = Column(Enum(AccountType), nullable=False)
    amount = Column(DECIMAL(10, 2), nullable=False, default=0)

    def __init__(self, data):
        if 'account_id' in data:
            self.account_id = data['account_id']
        if 'id_num' in data:
            self.id_num = data['id_num']
        if 'shop_id' in data:
            self.shop_id = data['shop_id']
        self.account_type = data['account_type']
        if 'amount' in data:
            self.amount = data['amount']
        else:
            self.amount = 0

    def to_dict(self):
        names = ['account_id', 'id_num', 'shop_id', 'account_type', 'amount']
        ret = dict((name, getattr(self, name)) for name in names if not name.startswith('__'))
        ret['account_type'] = ret['account_type'].value
        ret['amount'] = str(ret['amount'])
        return ret


class Bills(Base):
    __tablename__ = "bills"
    biil_id = Column(Integer, primary_key=True, autoincrement=True)
    amount = Column(DECIMAL(10, 2), nullable=False)
    sender_id = Column(Integer, ForeignKey('accounts.account_id', ondelete='RESTRICT'), nullable=False)
    receiver_id = Column(Integer, ForeignKey('accounts.account_id', ondelete='RESTRICT'), nullable=False)
    bill_date = Column(DATE(), nullable=False)
    bill_type = Column(Enum(BillType), nullable=False)

    def __init__(self, data):
        if 'bill_id' in data:
            self.biil_id = data['bill_id']
        self.amount = data['amount']
        self.sender_id = data['sender_id']
        self.receiver_id = data['receiver_id']
        self.bill_date = data['bill_date']
        self.bill_type = data['bill_type']

    def to_dict(self):
        names = ['bill_id', 'amount', 'sender_id', 'receiver_id']
        ret = dict((name, getattr(self, name)) for name in names if not name.startswith('__'))
        ret['amount'] = str(ret['amount'])
        return ret


class ShoppingCarts(Base):
    __tablename__ = "shoppingcarts"
    id_num = Column(String(18), ForeignKey('users.id_num', ondelete='RESTRICT'), nullable=False)
    good_id = Column(Integer, ForeignKey('goods.good_id', ondelete='RESTRICT'), nullable=False)
    count = Column(Integer, nullable=False, default=0)
    __table_args__ = (
        PrimaryKeyConstraint('id_num', 'good_id'),
        {},
    )

    def __init__(self, data):
        self.id_num = data['id_num']
        self.good_id = data['good_id']
        if 'count' in data:
            self.count = data['count']
        else:
            self.count = 0

    def to_dict(self):
        names = ['id_num', 'good_id', 'count']
        return dict((name, getattr(self, name)) for name in names if not name.startswith('__'))


class Images(Base):
    __tablename__ = 'images'
    image_id = Column(Integer, primary_key=True, autoincrement=True)

    def __init__(self, data):
        if 'image_id' in data:
            self.image_id = data['image_id']

    def to_dict(self):
        return {'image_id', self.image_id}


class Orders(Base):
    __tablename__ = 'orders'
    order_id = Column(Integer, primary_key=True, autoincrement=True)
    id_num = Column(String(18), ForeignKey('users.id_num', ondelete='RESTRICT'), nullable=False)
    total = Column(DECIMAL(10, 2), nullable=False)
    order_date = Column(DATE(), nullable=False)
    note = Column(Text)
    order_type = Column(Enum(OrderType, nullable=1))

    def __init__(self, data):
        self.id_num = data['id_num']
        self.total = data['total']
        self.order_date = data['order_date']
        if 'note' in data:
            self.note = data['note']
        self.order_type = data['order_type']

    def to_dict(self):
        names = ['order_id', 'id_num', 'total', 'order_date', 'note', 'order_type']
        ret = dict((name, getattr(self, name)) for name in names if not name.startswith('__'))
        ret['total'] = float(ret['total'])
        ret['date'] = str(ret['order_date'])
        ret['status'] = ret['order_type'].value
        ret.pop('order_type')
        return ret


class OrderGoods(Base):
    __tablename__ = 'order_goods'
    good_id = Column(Integer, ForeignKey('goods.good_id', ondelete='RESTRICT'), nullable=False)
    order_id = Column(Integer, ForeignKey('orders.order_id', ondelete='RESTRICT'), nullable=False)
    count = Column(Integer, nullable=False)
    __table_args__ = (
        PrimaryKeyConstraint('good_id', 'order_id'),
        {},
    )

    def __init__(self, data):
        self.good_id = data['good_id']
        self.order_id = data['order_id']
        self.count = data['count']

    def to_dict(self):
        names = ['good_id', 'order_id', 'count']
        ret = dict((name, getattr(self, name)) for name in names if not name.startswith('__'))
        return ret
