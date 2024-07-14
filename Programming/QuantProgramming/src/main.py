from investment import (StockInvestment, ETFInvestment, BondInvestment,
                        FuturesInvestment, FundInvestment, OptionsInvestment)
from selecting import (select_stocks, select_bonds, select_options,
                       select_funds, select_futures, select_etfs)

if __name__ == '__main__':
    bond_list = select_bonds()
    etf_list = select_etfs()
    fund_list = select_funds()
    future_list = select_futures()
    option_list = select_options()
    stock_list = select_stocks()

    num_investments = (len(bond_list) + len(etf_list) + len(fund_list) +
                       len(future_list) + len(option_list) + len(stock_list))

    total_strategy_return = 0
    total_market_return = 0
    for stock_name in stock_list:
        stock = StockInvestment("sh" + stock_name)  # 示例股票代码
        stock.fetch_data('20200101', '20201231', 'stocks', True)
        strategy_return, market_return = stock.backtest_strategy()
        total_strategy_return += strategy_return
        total_market_return += market_return

    for etf_name in etf_list:
        etf = ETFInvestment(etf_name, capital=100000)  # 示例ETF指数代码
        etf.fetch_data('2020-01-01', '2020-12-31', 'etfs', True)
        etf.calculate_technical_indicators()
        etf.generate_signals()
        strategy_return, market_return = etf.backtest_strategy()
        total_strategy_return += strategy_return
        total_market_return += market_return

    for fund_name in fund_list:
        fund = FundInvestment(fund_name, capital=100000)  # 示例基金代码
        fund.fetch_data('20200101', '20201231', 'funds', True)
        fund.calculate_technical_indicators()
        fund.generate_signals()
        strategy_return, market_return = fund.backtest_strategy()
        total_strategy_return += strategy_return
        total_market_return += market_return

    for future_name in future_list:
        future = FuturesInvestment(future_name)  # 示例期货代码
        future.fetch_data('20200101', '20201231', 'futures', True)
        future.calculate_technical_indicators()
        future.generate_signals()
        strategy_return, market_return = future.backtest_strategy()
        total_strategy_return += strategy_return
        total_market_return += market_return

    for option_name in option_list:
        option = OptionsInvestment(option_name)  # 示例期权代码
        option.fetch_data('2020-01-01', '2020-12-31', 'options', True)
        option.calculate_technical_indicators()
        option.generate_signals()
        strategy_return, market_return = option.backtest_strategy()
        total_strategy_return += strategy_return
        total_market_return += market_return

    for bond_name in bond_list:
        bond = BondInvestment(bond_name, capital=100000)  # Example bond symbol
        bond.fetch_data('20200101', '20201231', 'bonds', True)
        if bond.data is not None:
            bond.calculate_technical_indicators()
            bond.generate_signals()
            strategy_return, market_return = bond.backtest_strategy()
            total_strategy_return += strategy_return
            total_market_return += market_return

    print(f'策略最终回报: {total_strategy_return * 100 / num_investments:.2f}%')
    print(f'市场最终回报: {total_market_return * 100 / num_investments:.2f}%')
