import akshare as ak


def select_futures():
    # 读取数据
    data = ak.futures_fees_info()
    # print(data.columns)

    # 计算因子分数
    data['成交量_score'] = data['成交量'].rank(pct=True, ascending=False)
    data['空盘量_score'] = data['空盘量'].rank(pct=True)

    # 组合因子
    data['composite_score'] = data[['成交量_score', '空盘量_score']].mean(axis=1)

    # 选股
    selected_futures = data.sort_values(by='composite_score', ascending=False).head(10)  # 选取得分最高的前10只股票

    # 输出选中的股票代码和名称
    print("selected_futures: ", selected_futures['合约'].values)

    return selected_futures['合约'].values


if __name__ == "__main__":
    select_futures()