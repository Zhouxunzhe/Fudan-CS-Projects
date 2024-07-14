from datetime import datetime


def date_transfer(utc_time, date_type=1):
    """
    将UTC标准时间转换为本地时间
    :param utc_time:
    :param date_type:
    :return: local_time
    """
    # 将字符串解析为datetime对象
    utc_time = datetime.fromisoformat(utc_time.replace('Z', '+00:00'))
    # 将UTC时间转换为本地时间
    local_time = utc_time.astimezone()
    # 根据type的值选择输出格式
    if date_type == 1:
        result_str = local_time.strftime('%Y-%m-%d')
    else:
        result_str = local_time.strftime('%Y-%m-%d %H:%M:%S')
    return result_str
