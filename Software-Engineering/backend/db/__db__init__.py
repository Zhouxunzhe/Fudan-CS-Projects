from options import *
from werkzeug.security import generate_password_hash
import pymysql

db = pymysql.connect(host=host, user=user, password=password, db=db_name, cursorclass=pymysql.cursors.DictCursor)
cursor = db.cursor()

# account_id: admin: 1, middle: 2, mall: 3
sql1 = "insert into superusers(username, password) values (%s, %s);"
sql2 = "insert into accounts(account_type) values (%s);"
sql3 = "insert into accounts(id_num, account_type) values (%s, %s);"
sql4 = "insert into users(id_num, username, phonenumber, email, password, is_shop) values (%s, %s, %s, %s, %s, %s)"
params1 = ('admin', generate_password_hash(admin_password))
try:
    cursor.execute(sql1, params1)
    cursor.execute(sql4, ("-1", "", "", "", "", False))
    cursor.execute(sql3, ('-1', 'user'))
    cursor.execute(sql2, 'middle')
    cursor.execute(sql2, 'mall')
except Exception as e:
    db.rollback()
    print(e)

db.commit()
db.close()
