from flask import current_app
import jwt
from jwt import exceptions
import time


def create_token(id_num, is_shop, superuser):
    headers = {
        "alg": "HS256",
        "typ": "JWT"
    }
    payload = {
        "id_num": id_num,
        "is_shop": is_shop,
        "superuser": superuser,
        "exp": int(time.time() + 3600)
    }
    token = jwt.encode(payload=payload, key=current_app.config['SECRET_KEY'],
                       algorithm='HS256', headers=headers)
    return str(token)


def parse_token(token):
    payload = None
    msg = None
    token = token[7:]
    try:
        payload = jwt.decode(jwt=token, key=current_app.config['SECRET_KEY'], verify=True, algorithms='HS256')
    except exceptions.ExpiredSignatureError:
        msg = 'token已失效'
    except jwt.DecodeError:
        msg = 'token认证失败'
    except jwt.InvalidTokenError:
        msg = '非法的token'
    return payload, msg
