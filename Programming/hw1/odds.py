import random


def simulate_gambling(win_rate, payout_rate, bet_ratio, iterations):
    wins = 0
    losses = 0
    balance = 0

    for _ in range(iterations):
        if random.random() < win_rate:
            wins += 1
            balance += bet_ratio * payout_rate
        else:
            losses += 1
            balance -= bet_ratio

    return wins, losses, balance


win_rate = 0.4  # 胜率
payout_rate = 3  # 赔率
bet_ratio = 1  # 投入比率
iterations = 1000000  # 模拟次数

wins, losses, balance = simulate_gambling(win_rate, payout_rate, bet_ratio, iterations)
profit = balance - bet_ratio * iterations

print(f"Wins: {wins}, Losses: {losses}")
print(f"Final balance: {balance}")
print(f"Profit: {profit}")
