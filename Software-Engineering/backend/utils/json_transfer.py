"""
该文件方法用于将数据库属性名转化为前端接口需要的名字
"""

def good_transfer(good_data):
    good_data['id'] = good_data['good_id']
    good_data['stock'] = good_data['goodamount']
    good_data.pop('good_id')
    good_data.pop('goodamount')