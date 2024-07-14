from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker, scoped_session
from sqlalchemy.ext.declarative import declarative_base

# 连接引擎
engine = create_engine(
    "mysql+pymysql://soft_engineer:123456@127.0.0.1/Web_Project?charset=utf8mb4",
    max_overflow=-1,
    pool_size=5,
    pool_timeout=10,
    pool_recycle=1,
    echo=True
)

# 基类
Base = declarative_base()

# 创建session
session = scoped_session(sessionmaker(bind=engine))

from .AccountOperation import AccountOperation
from .BillOperation import BillOperation
from .ShopOperation import ShopOperation
from .GoodOperation import GoodOperation
from .GoodRequestOperation import GoodRequestOperation
from .RequestOperation import RequestOperation
from .ShopRequestOperation import ShopRequestOperation
from .ShoppingCartOperation import ShoppingCartOperation
from .UserOperation import UserOperation
from .SuperUserOperation import SuperUserOperation
from .ImageOperation import ImageOperation
from .OrderOperation import OrderOperation
from .OrderGoodOperation import OrderGoodsOperation

AccountOperationObject = AccountOperation()
BillOperationObject = BillOperation()
GoodOperationObject = GoodOperation()
GoodRequestOperationObject = GoodRequestOperation()
ShopOperationObject = ShopOperation()
ShoppingCartOperationObject = ShoppingCartOperation()
ShopRequestOperationObject = ShopRequestOperation()
UserOperationObject = UserOperation()
SuperUserOperationObject = SuperUserOperation()
ImageOperationObject = ImageOperation()
OrderOperationObject = OrderOperation()
OrderGoodOperationObject = OrderGoodsOperation()
