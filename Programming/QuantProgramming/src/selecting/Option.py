import akshare as ak


def select_options():
    # 读取数据
    data = ak.option_cffex_sz50_spot_sina()
    # print(data.columns)

    # 计算因子分数
    data['看跌合约-持仓量_score'] = data['看跌合约-持仓量'].rank(pct=True, ascending=False)

    # 组合因子
    data['composite_score'] = data[['看跌合约-持仓量_score']].mean(axis=1)

    # 选股
    selected_futures = data.sort_values(by='composite_score', ascending=False).head(10)  # 选取得分最高的前10只股票

    # 输出选中的股票代码和名称
    print("selected_options: ", selected_futures[['看跌合约-标识']].values)

    return selected_futures['看跌合约-标识'].values


if __name__ == "__main__":
    select_options()