import random

def simulate_investment(n, days=3650, initial_capital=1_000_000, win_rate=0.45, win_loss_ratio=3):
    capital = initial_capital
    daily_profit = []

    for day in range(days):
        daily_result = 0
        for _ in range(n):
            if random.random() < win_rate:
                daily_result += win_loss_ratio  # 盈利
            else:
                daily_result -= 1  # 亏损
        daily_profit.append(daily_result)
        capital += daily_result
    
    return capital, daily_profit

# 示例：每天交易5次，模拟10年
final_capital, daily_profit = simulate_investment(n=5)
print(f"10年后的本金：{final_capital}元")
