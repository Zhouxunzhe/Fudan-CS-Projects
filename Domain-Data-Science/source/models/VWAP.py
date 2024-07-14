from function import *


def calc_vwap(order_book):
    A = 0
    B = 0
    for i in range(5):
        price = float(order_book[i][0])
        volume = float(order_book[i][1])
        A += weight[i] * price * volume
        B += weight[i] * volume
    if B < 0.001:  # 0.001 for BTCUDST
        return 0
    return A / B