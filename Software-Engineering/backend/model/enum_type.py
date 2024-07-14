import enum


class RequestType(enum.Enum):
    open = 1
    close = 2
    modify = 3


class CommentType(enum.Enum):
    approve = 1
    refuse = 2


class AccountType(enum.Enum):
    user = 1
    shop = 2
    middle = 3
    mall = 4


class BillType(enum.Enum):
    buy = 1
    open = 2
    recharge = 3
    open_success = 4
    open_fail = 5


class OrderType(enum.Enum):
    ready = 1
    already = 2
    cancel = 3
