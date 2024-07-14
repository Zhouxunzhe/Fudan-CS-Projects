import numpy as np
import akshare as ak


def select_stocks():
    # 读取数据
    data = ak.stock_sh_a_spot_em()
    data = data[~data['名称'].str.contains('ST')]

    # 定义选股因子
    factors = {
        'pe_ratio': '市盈率-动态',
        'pb_ratio': '市净率',
        # 'roe': '资产收益率',
    }

    # 计算因子分数
    data['pe_ratio_score'] = data['市盈率-动态'].rank(pct=True, ascending=False)
    data['pb_ratio_score'] = data['市净率'].rank(pct=True, ascending=False)

    # 组合因子
    data['composite_score'] = data[['pe_ratio_score', 'pb_ratio_score']].mean(axis=1)

    # 选股
    selected_stocks = data.sort_values(by='composite_score', ascending=False).head(10)  # 选取得分最高的前10只股票

    # 输出选中的股票代码和名称
    print("selected_stocks: ", selected_stocks[['代码', '名称']].values)

    return selected_stocks['代码'].values


if __name__ == "__main__":
    select_stocks()
