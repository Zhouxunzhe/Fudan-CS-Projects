import re

"""
该文件方法用于进行前端的数据验证
"""

# TODO 对类型等属性进行检验


def is_valid_idcard(idcard) -> bool:
    if idcard[-1] == 'x':
        idcard[-1] = 'X'
    IDCARD_REGEX = '[1-9][0-9]{14}([0-9]{2}[0-9X])?'

    if not re.match(IDCARD_REGEX, idcard):
        return False

    items = [int(item) for item in idcard[:-1]]

    # 加权因子表
    factors = (7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2)

    # 计算17位数字各位数字与对应的加权因子的乘积
    copulas = sum([a * b for a, b in zip(factors, items)])

    # 校验码表
    ckcodes = ('1', '0', 'X', '9', '8', '7', '6', '5', '4', '3', '2')

    return ckcodes[copulas % 11].upper() == idcard[-1].upper()


def check_id(id) -> bool:
    return is_valid_idcard(id)


def check_phonenumber(phonenumber) -> bool:
    if not phonenumber:
        return False
    return re.match(r'^1[0-9]{10}$', str(phonenumber)) is not None


def check_username(username) -> bool:
    if not username:
        return False
    return re.match(r'[A-Za-z0-9_]{3,10}$', username) is not None


def check_email(email) -> bool:
    if not email:
        return False
    return re.match(r'[a-zA-Z0-9]+@[a-zA-Z0-9]+\.[a-zA-Z0-9]+$', email) is not None


def check_password(password) -> bool:
    if not password:
        return False
    if len(password) < 6 or len(password) > 32:
        return False
    cnt1 = 0
    cnt2 = 0
    cnt3 = 0
    for c in password:
        if ('a' <= c and c <= 'z') or ('A' <= c and c <= 'Z'):
            cnt1 = 1
        elif '0' <= c and c <= '9':
            cnt2 = 1
        elif c == '-' and c == '_':
            cnt3 = 1
        else:
            return False
    return cnt1 + cnt2 + cnt3 >= 2


def check_isshop(is_shop):
    if (is_shop != 1) and (is_shop != 0):
        return False
    return True


# following functions would be really used to check
def check_login(data) -> bool:
    if 'username' not in data or check_username(data['username']) is False:
        return False
    if 'password' not in data or check_password(data['password']) is False:
        return False
    return True


def check_register(data) -> bool:
    if 'id_num' not in data or check_id(data['id_num']) is False:
        return False
    if 'username' not in data or check_username(data['username']) is False:
        return False
    if 'phonenumber' not in data or check_phonenumber(data['phonenumber']) is False:
        return False
    if 'email' not in data or check_email(data['email']) is False:
        return False
    if 'password' not in data or check_password(data['password']) is False:
        return False
    if 'is_shop' not in data or check_isshop(data['is_shop']) is False:
        return False
    return True

def check_limit(data) -> bool:
    if 'limit' not in data or data['limit'] < 0:
        return False
    return True

def check_shop_id(data) -> bool:
    if 'shop_id' not in data or data['shop_id'] < 0:
        return False
    return True

def check_good_id(data) -> bool:
    if 'good_id' not in data or data['good_id'] < 0:
        return False
    return True

def check_shopname(shopname) -> bool:
    if not shopname:
        return False
    return re.match(r'[\u4e00-\u9fa5a-zA-Z0-9]{1,12}$', shopname) is not None


def check_kind(kind) -> bool:
    if not kind:
        return False
    return re.match(r'[\u4e00-\u9fa5a-zA-Z]+([，, ][\u4e00-\u9fa5a-zA-Z]+)*$', kind) is not None


def check_intro(intro) -> bool:
    return re.match(r'(.|\n){0,128}$', intro) is not None


def check_address(address) -> bool:
    if not address:
        return False
    # TODO:限定输入地址的格式
    # city,province,country
    return re.match(r'([\u4e00-\u9fa5a-zA-Z]+[,，][\u4e00-\u9fa5a-zA-Z]+[,，][\u4e00-\u9fa5a-zA-Z]+){1,32}$',
                    address) is not None


def check_capital(register_capital) -> bool:
    if not register_capital:
        return False
    if register_capital[0] == '0':
        return False
    return re.match(r'^\d{4,}\.\d{0,2}$', register_capital) is not None\
        or re.match(r'^\d{4,}$', register_capital) is not None


def check_date(date) -> bool:
    if not date:
        return False
    return re.match(r'\d{4}-\d{1,2}-\d{1,2}$', date) is not None


def check_openshop(data) -> bool:
    if 'shopname' not in data or check_shopname(data['shopname']) is False:
        return False
    if 'kind' not in data or check_kind(data['kind']) is False:
        return False
    if 'intro' not in data or check_intro(data['intro']) is False:
        return False
    if 'address' not in data or check_address(data['address']) is False:
        return False
    if 'register_capital' not in data or check_capital(data['register_capital']) is False:
        return False
    if 'register_date' not in data or check_date(data['register_date'][0:9]) is False:
        return False
    return True


def check_price(price):
    if not price:
        return False
    return re.match(r'\d+.\d{0,2}$', price) is not None \
        or re.match(r'^\d+$', price) is not None

def check_recharge(data):
    if 'amount' not in data or check_price(data['amount']) is False:
        return False
    return True

def check_image(image):
    if not image:
        return False
    # TODO 对图片格式的判断
    return True


def check_amount(amount):
    if not amount:
        return False
    return re.match(r'\d{0,10}$', amount) is not None


def check_add_good(data):
    if 'goodname' not in data or check_shopname(data['goodname']) is False:
        return False
    if 'intro' not in data or check_intro(data['intro']) is False:
        return False
    if 'price' not in data or check_price(data['price']) is False:
        return False
    if 'request_date' not in data or check_date(data['request_date']) is False:
        return False
    if 'goodamount' not in data or check_amount(str(data['goodamount'])) is False:
        return False
    return True


def check_modify_good(data):
    if 'goodname' in data and data['goodname'] != '' and check_shopname(data['goodname']) is False:
        return False
    if 'intro' in data and data['intro'] != '' and check_intro(data['intro']) is False:
        return False
    if 'price' in data and data['price'] != '' and check_price(data['price']) is False:
        return False
    if 'images' in data and data['images'] != '' and check_image(data['images']) is False:
        return False
    if 'request_data' in data and data['request_date'] != '' and check_date(data['request_date']) is False:
        return False
    if 'goodamount' in data and data['register_capital'] != '' and check_capital(data['register_capital']) is False:
        return False
    return True


def check_modify_userinfo(data):
    if 'username' in data and data['username'] == "":
        data.pop('username')
    if 'email' in data and data['email'] == "":
        data.pop('email')
    if 'phonenumber' in data and data['phonenumber'] == "":
        data.pop('phonenumber')
    if 'password' in data and data['password'] == "":
        data.pop('password')
    if 'username' in data and check_username(data['username']) is False:
        return False
    if 'email' in data and check_email(data['email']) is False:
        return False
    if 'phonenumber' in data and check_phonenumber(data['phonenumber']) is False:
        return False
    if 'password' in data and check_password(data['password']) is False:
        return False
    return True
