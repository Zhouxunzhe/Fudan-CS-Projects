import torch
import torch.nn as nn
import numpy as np
import pandas as pd
import math
import torch.nn.functional as function
from torch import optim
init_fund = 1000000


class Environment:
    def __init__(self, data):
        self.data = pd.DataFrame()
        self.data['close'] = data[:, 3]
        self.data['pct_change'] = \
            ((self.data['close'] - self.data['close'].shift(1)) / self.data['close'].shift(1))
        self.data = self.data[1:]
        self.day_epoch = 1
        self.buy_fee_rate = 0.0
        self.sell_fee_rate = 0.0
        self.order_size = 150000  # 每个订单购买的价钱
        self.balance = init_fund
        self.position = 0  # 持有股份数
        self.market_value = 0
        self.fund = self.balance
        self.day_profit = 0

    def reset(self):
        self.day_epoch = 1
        self.balance = init_fund
        self.position = 0  # 持有股份数
        self.market_value = 0
        self.fund = self.balance
        self.day_profit = 0
        state = list(self.data.iloc[self.day_epoch])

        return state

    def step(self, action):
        op = 0  # 0:do nothing, 1:buy, 2:sell
        current_price = self.data['close'][self.day_epoch]
        self.day_profit = self.position * current_price * self.data['pct_change'][self.day_epoch]
        if action == 0:
            # buy
            if self.balance > self.order_size:
                buy_order = math.floor(self.order_size / self.data['close'][self.day_epoch])
                self.position += buy_order
                trade_amount = buy_order * current_price
                buy_fee = buy_order * current_price * self.buy_fee_rate
                self.balance = self.balance - trade_amount - buy_fee
                op = 1
            else:
                # print("buy:not enough fund")
                pass
        elif action == 1:
            # sell
            if self.position * current_price > self.order_size:
                sell_order = math.ceil(self.order_size / self.data['close'][self.day_epoch])
                self.position -= sell_order
                sell_fee = sell_order * current_price * self.sell_fee_rate
                trade_amount = sell_order * current_price
                self.balance = self.balance + trade_amount - sell_fee
                op = 2
            else:
                # print("sell:not enough stock")
                pass
        else:
            # do nothing
            pass

        # 重新计算持仓状况
        self.market_value = self.position * current_price
        self.fund = self.market_value + self.balance
        self.day_epoch += 1
        state_ = list(self.data.iloc[self.day_epoch])
        reward = self.day_profit
        if self.day_epoch == len(self.data) - 1:
            done = True
        else:
            done = False

        return state_, reward, done, op


class DeepQNetwork(nn.Module):
    def __init__(self, lr, n_state, fc1_dims, fc2_dims, n_actions):
        super(DeepQNetwork, self).__init__()
        self.lr = lr
        self.n_state = n_state
        self.fc1_dims = fc1_dims
        self.fc2_dims = fc2_dims
        self.n_actions = n_actions

        self.fc1 = nn.Linear(self.n_state, self.fc1_dims)
        self.fc2 = nn.Linear(self.fc1_dims, self.fc2_dims)
        self.fc3 = nn.Linear(self.fc2_dims, self.n_actions)
        self.optimizer = optim.Adam(self.parameters(), lr=lr)
        self.loss = nn.MSELoss()

        self.device = torch.device('cuda:0' if torch.cuda.is_available() else 'cpu')
        self.to(self.device)

    def forward(self, state):
        state.to(self.device)
        x = self.fc1(state.to(torch.float32))
        x = function.leaky_relu(x)
        x = self.fc2(x)
        x = function.leaky_relu(x)
        actions = self.fc3(x)

        return actions


class Agent:
    # gamma的折扣率它必须介于0和1之间。越大，折扣越小。这意味着学习，agent 更关心长期奖励。另一方面，gamma越小，折扣越大。这意味着我们的 agent 更关心短期奖励（最近的奶酪）。
    # epsilon探索率ϵ。即策略是以1−ϵ的概率选择当前最大价值的动作，以ϵ的概率随机选择新动作。
    def __init__(self, gamma, epsilon, lr, n_state, batch_size, n_actions=3,
                 max_mem_size=100000, eps_end=0.01, eps_dec=0.995):
        self.gamma = gamma
        self.epsilon = epsilon
        self.eps_min = eps_end
        self.eps_dec = eps_dec
        self.lr = lr
        self.n_actions = n_actions
        self.action_space = [i for i in range(n_actions)]
        self.mem_size = max_mem_size
        self.batch_size = batch_size
        self.mem_cntr = 0

        self.Q_eval = DeepQNetwork(self.lr, n_state=n_state, n_actions=self.n_actions,
                                   fc1_dims=256, fc2_dims=256)
        self.loss = None
        self.state_memory = np.zeros((self.mem_size, n_state), dtype=np.float32)
        self.new_state_memory = np.zeros((self.mem_size, n_state), dtype=np.float32)
        self.action_memory = np.zeros(self.mem_size, dtype=np.int32)
        self.reward_memory = np.zeros(self.mem_size, dtype=np.float32)
        self.terminal_memory = np.zeros(self.mem_size, dtype=np.bool)

    def save_model(self):
        torch.save(self.Q_eval, 'DNN_Params')

    def load_model(self):
        self.Q_eval = torch.load('DNN_Params')

    # 存储记忆
    def store_transition(self, state, action, reward, state_, done):
        index = self.mem_cntr % self.mem_size
        self.state_memory[index] = state
        self.new_state_memory[index] = state_
        self.reward_memory[index] = reward
        self.action_memory[index] = action
        self.terminal_memory[index] = done

        self.mem_cntr += 1

    def choose_action(self, state):
        if np.random.random() > self.epsilon:
            # 随机0-1，即1-epsilon的概率执行以下操作,最大价值操作
            state = torch.tensor(state).to(self.Q_eval.device)
            # 放到神经网络模型里面得到action的Q值vector
            actions = self.Q_eval.forward(state)
            action = torch.argmax(actions).item()
        else:
            # epsilon概率执行随机动作
            action = np.random.choice(self.action_space)
        return action

    # 从记忆中抽取batch进行学习
    def learn(self):
        # memory counter小于一个batch大小的时候直接return
        if self.mem_cntr < self.batch_size:
            return

        self.Q_eval.optimizer.zero_grad()
        max_mem = min(self.mem_cntr, self.mem_size)
        batch = np.random.choice(max_mem, self.batch_size, replace=False)
        batch_index = np.arange(self.batch_size, dtype=np.int32)

        state_batch = torch.tensor(self.state_memory[batch]).to(self.Q_eval.device)
        new_state_batch = torch.tensor(self.new_state_memory[batch]).to(self.Q_eval.device)
        reward_batch = torch.tensor(self.reward_memory[batch]).to(self.Q_eval.device)
        terminal_batch = torch.tensor(self.terminal_memory[batch]).to(self.Q_eval.device)  # 存储是否结束的bool型变量
        action_batch = self.action_memory[batch]

        # 第batch_index行，取action_batch列,对state_batch中的每一组输入，输出action对应的Q值,batch size行，1列的Q值
        q_eval = self.Q_eval.forward(state_batch)[batch_index, action_batch]
        q_next = self.Q_eval.forward(new_state_batch)
        q_next[terminal_batch] = 0.0  # 如果是最终状态，则将q值置为0

        q_target = reward_batch + self.gamma * torch.max(q_next, dim=1)[0]

        loss = self.Q_eval.loss(q_target, q_eval).to(self.Q_eval.device)
        loss.backward()
        self.loss = loss.item()
        self.Q_eval.optimizer.step()

        self.epsilon = self.epsilon * self.eps_dec if self.epsilon > self.eps_min \
            else self.eps_min
