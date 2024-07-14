import os
import akshare as ak


def save_stock(stock_list=None):
    # 假设这里用的是中国A股市场的股票代码
    stock_list = ["sh000001", "sh000002", "sh000004", "sh000006", "sh000007", "sh000008", "sh000009", "sh000010"]  # 示例股票代码
    for stock_code in stock_list:
        stock_data = ak.stock_zh_index_daily_em(symbol=stock_code, adjust="qfq")
        stock_data.to_csv(f"stocks/{stock_code}.csv", index=False)


def save_future(future_list=None):
    # 示例期货代码
    future_list = ["CU0", "AL0", "ZN0", "PB0", "NI0", "SN0", "AU0", "AG0", "RB0", "WR0"]
    for future_code in future_list:
        future_data = ak.futures_zh_spot(symbol=future_code, market="SHFE")
        future_data.to_csv(f"commodities/{future_code}.csv", index=False)


def save_option(option_list=None):
    # 示例期权数据（此处以50ETF期权为例）
    option_list = ["10000872", "10000873", "10000874", "10000875", "10000876", "10000877", "10000878", "10000879",
                   "10000880", "10000881"]
    for option_code in option_list:
        option_data = ak.option_sse_daily_sina(symbol=option_code)
        option_data.to_csv(f"derivatives/{option_code}.csv", index=False)


def save_etf(etf_list=None):
    # 示例ETF代码
    etf_list = ["510050", "510500", "510300", "510330", "510880", "510900", "511010", "511220", "512100", "512880"]
    for etf_code in etf_list:
        etf_data = ak.fund_etf_hist_sina(symbol=etf_code)
        etf_data.to_csv(f"etfs/{etf_code}.csv", index=False)


def save_fund(fund_list=None):
    # 示例基金代码
    fund_list = ["000001", "000002", "000003", "000004", "000005", "000006", "000007", "000008", "000009", "000010"]
    for fund_code in fund_list:
        fund_data = ak.fund(fund=fund_code, indicator="单位净值走势")
        fund_data.to_csv(f"funds/{fund_code}.csv", index=False)


def save_bond(bond_list=None):
    # 示例债券代码
    bond_list = ["019501", "019502", "019503", "019504", "019505", "019506", "019507", "019508", "019509", "019510"]
    for bond_code in bond_list:
        bond_data = ak.bond_zh_hs_daily(symbol=bond_code)
        bond_data.to_csv(f"bonds/{bond_code}.csv", index=False)
