import pymysql
from db.options import *


def connect_db():
    db = pymysql.connect(host=host, user=user, password=password, db=db_name, cursorclass=pymysql.cursors.DictCursor)
    return db
