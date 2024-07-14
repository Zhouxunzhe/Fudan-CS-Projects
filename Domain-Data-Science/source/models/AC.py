import torch
import torch.nn as nn
import pandas as pd
import math
import torch.nn.functional as F


class Environment:
    def __init__(self, data):
        self.data = pd.DataFrame()
        self.data['open'] = data[:, 0]
        self.data['high'] = data[:, 1]
        self.data['low'] = data[:, 2]
        self.data['close'] = data[:, 3]
        self.data['pct_change'] = \
            ((self.data['close'] - self.data['close'].shift(1)) / self.data['close'].shift(1))
        self.data = self.data[1:]
        self.barpos = 1
        self.buy_fee_rate = 0.0
        self.sell_fee_rate = 0.0
        self.order_size = 100000  # 每个订单购买的价钱
        self.balance = 10000000
        self.position = 0  # 持有股份数
        self.market_value = 0
        self.fund = self.balance
        self.day_profit = 0

    def reset(self):
        self.barpos = 1
        self.balance = 10000000
        self.position = 0  # 持有股份数
        self.market_value = 0
        self.fund = self.balance
        self.day_profit = 0

        observation = list(self.data.iloc[self.barpos])
        return observation

    def step(self, action):
        op = 0  # 0:do nothing, 1:buy, 2:sell
        current_price = self.data['close'][self.barpos]
        self.day_profit = self.position * current_price * self.data['pct_change'][self.barpos]
        if action == 0:
            if self.balance > self.order_size:
                buy_order = math.floor(self.order_size / self.data['close'][self.barpos])
                self.position += buy_order
                trade_amount = buy_order * current_price
                buy_fee = buy_order * current_price * self.buy_fee_rate
                self.balance = self.balance - trade_amount - buy_fee
                # print("buy:success")
                op = 1
            else:
                # print("buy:not enough fund")
                pass
        elif action == 1:
            if self.position * current_price > self.order_size:
                sell_order = math.ceil(self.order_size / self.data['close'][self.barpos])
                self.position -= sell_order
                sell_fee = sell_order * current_price * self.sell_fee_rate
                trade_amount = sell_order * current_price
                self.balance = self.balance + trade_amount - sell_fee
                op = 2
                # print("sell:success")
            else:
                # print("sell:not enough stock")
                pass
        else:
            pass

        # 重新计算持仓状况
        self.market_value = self.position * current_price
        self.fund = self.market_value + self.balance
        self.barpos += 1

        observation_ = list(self.data.iloc[self.barpos])
        reward = self.day_profit
        if self.barpos == len(self.data) - 1:
            done = True
        else:
            done = False

        return observation_, reward, done, op


# Actor
class PolicyNet(nn.Module):
    def __init__(self, n_states, n_hiddens, n_actions):
        super(PolicyNet, self).__init__()
        self.fc1 = nn.Linear(n_states, n_hiddens)
        self.fc2 = nn.Linear(n_hiddens, n_actions)

    # 前向传播
    def forward(self, x):
        x = self.fc1(x)  # [b,n_states]-->[b,n_hiddens]
        x = F.relu(x)
        x = self.fc2(x)  # [b,n_hiddens]-->[b,n_actions]
        # 每个状态对应的动作的概率
        x = F.softmax(x, dim=1)  # [b,n_actions]-->[b,n_actions]
        return x


# Critic
class ValueNet(nn.Module):
    def __init__(self, n_states, n_hiddens):
        super(ValueNet, self).__init__()
        self.fc1 = nn.Linear(n_states, n_hiddens)
        self.fc2 = nn.Linear(n_hiddens, 1)

    # 前向传播
    def forward(self, x):
        x = self.fc1(x)  # [b,n_states]-->[b,n_hiddens]
        x = F.relu(x)
        x = self.fc2(x)  # [b,n_hiddens]-->[b,1]
        return x


# Actor-Critic
class ActorCritic:
    def __init__(self, n_states, n_hiddens, n_actions,
                 actor_lr, critic_lr, gamma):
        # 属性分配
        self.gamma = gamma

        # 实例化策略网络
        self.actor = PolicyNet(n_states, n_hiddens, n_actions)
        # 实例化价值网络
        self.critic = ValueNet(n_states, n_hiddens)
        # 策略网络的优化器
        self.actor_optimizer = torch.optim.Adam(self.actor.parameters(), lr=actor_lr)
        # 价值网络的优化器
        self.critic_optimizer = torch.optim.Adam(self.critic.parameters(), lr=critic_lr)

    # 动作选择
    def take_action(self, state):
        # 维度变换numpy[n_states]-->[1,n_sates]-->tensor
        state = torch.tensor([state], dtype=torch.float32)
        # 动作价值函数，当前状态下各个动作的概率
        probs = self.actor(state)
        # 创建以probs为标准类型的数据分布
        action_dist = torch.distributions.Categorical(probs)
        # 随机选择一个动作 tensor-->int
        action = action_dist.sample().item()
        return action

    # 模型更新
    def update(self, transition_dict):
        # 训练集
        states = torch.tensor(transition_dict['states'], dtype=torch.float)
        actions = torch.tensor(transition_dict['actions']).view(-1, 1)
        rewards = torch.tensor(transition_dict['rewards'], dtype=torch.float).view(-1, 1)
        next_states = torch.tensor(transition_dict['next_states'], dtype=torch.float)
        dones = torch.tensor(transition_dict['dones'], dtype=torch.float).view(-1, 1)

        # 预测的当前时刻的state_value
        td_value = self.critic(states)
        # 目标的当前时刻的state_value
        td_target = rewards + self.gamma * self.critic(next_states) * (1 - dones)
        # 时序差分的误差计算，目标的state_value与预测的state_value之差
        td_delta = td_target - td_value

        # 对每个状态对应的动作价值用log函数
        log_probs = torch.log(self.actor(states).gather(1, actions))
        # 策略梯度损失
        actor_loss = torch.mean(-log_probs * td_delta.detach())
        # 值函数损失，预测值和目标值之间
        critic_loss = torch.mean(F.mse_loss(self.critic(states), td_target.detach()))

        # 优化器梯度清0
        self.actor_optimizer.zero_grad()  # 策略梯度网络的优化器
        self.critic_optimizer.zero_grad()  # 价值网络的优化器
        # 反向传播
        actor_loss.backward()
        critic_loss.backward()
        # 参数更新
        self.actor_optimizer.step()
        self.critic_optimizer.step()
