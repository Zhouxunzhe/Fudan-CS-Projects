from models import DQN, AC
import numpy as np
import matplotlib.pyplot as plt
from function import *


def dqn():
    data_size = 500
    kline_data = np.array(get_klines_data(data_size)).astype('float64')
    data = np.delete(kline_data, [0, 6, 11], axis=1)
    env = DQN.Environment(data)
    agent = DQN.Agent(gamma=0.01, epsilon=1.0, batch_size=64, n_actions=3, eps_end=0.01, n_state=2, lr=0.001)
    model_profits = []
    n_games = 10  # 训练局数

    for i in range(n_games):
        done = False
        state = env.reset()
        reward_pool, loss_list = [], []
        while not done:
            action = agent.choose_action(state)
            state_, reward, done, op = env.step(action)
            reward_pool.append(reward)
            agent.store_transition(state, action, reward, state_, done)
            agent.learn()
            loss_list.append(agent.loss)
            state = state_

        # plt.plot(range(len(loss_list)), loss_list, label="loss")
        # plt.legend(loc="upper right")
        # plt.show()
        # plt.plot(range(len(reward_pool)), reward_pool, label="reward")
        # plt.legend(loc="upper right")
        # plt.show()

        print('episode', i,
              'epsilon %.2f' % agent.epsilon,
              'profits %.2f' % sum(reward_pool))
        model_profits.append(sum(reward_pool))

    fund, buy, sell = [], [], []
    profit = 0
    done = False
    state = env.reset()
    reward_pool, loss_list = [], []
    while not done:
        action = agent.choose_action(state)
        state_, reward, done, op = env.step(action)
        reward_pool.append(reward)
        agent.store_transition(state, action, reward, state_, done)
        agent.learn()
        loss_list.append(agent.loss)
        state = state_

        if op == 1:
            buy.append(env.day_epoch)
        elif op == 2:
            sell.append(env.day_epoch)
        profit += reward
        fund.append(profit)

    print('episode', n_games,
          'epsilon %.2f' % agent.epsilon,
          'profits %.2f' % sum(reward_pool))
    model_profits.append(sum(reward_pool))

    # plt.plot(range(len(fund)), fund)
    # plt.show()
    plt.figure(figsize=(16, 8))
    plt.plot(env.data['close'].iloc[1:], linewidth=1.2)
    plt.plot(env.data['close'].iloc[1:], '^', markersize=10, label='buying signal', markevery=buy)
    plt.plot(env.data['close'].iloc[1:], 'v', markersize=10, label='selling signal', markevery=sell)
    plt.legend(loc='upper right')
    plt.show()


def actor_critic():
    size = 1000
    kline_data = np.array(get_klines_data(size)).astype('float64')
    data = np.delete(kline_data, [0, 6, 11], axis=1)
    env = AC.Environment(data)
    agent = AC.ActorCritic(n_states=5, n_hiddens=16, n_actions=3,
                           actor_lr=0.003, critic_lr=0.003, gamma=0.99)
    return_list = []
    n_games = 100  # 训练局数

    for i in range(n_games):
        state = env.reset()  # 环境重置
        done = False  # 任务完成的标记
        episode_return = 0  # 累计每回合的reward

        # 构造数据集，保存每个回合的状态数据
        transition_dict = {
            'states': [],
            'actions': [],
            'next_states': [],
            'rewards': [],
            'dones': [],
        }
        while not done:
            action = agent.take_action(state)  # 动作选择
            next_state, reward, done, op = env.step(action)  # 环境更新
            # 保存每个时刻的状态\动作\...
            transition_dict['states'].append(state)
            transition_dict['actions'].append(action)
            transition_dict['next_states'].append(next_state)
            transition_dict['rewards'].append(reward)
            transition_dict['dones'].append(done)
            # 更新状态
            state = next_state
            # 累计回合奖励
            episode_return += reward
        # 保存每个回合的return
        return_list.append(episode_return)
        # 模型训练
        agent.update(transition_dict)

        # 打印回合信息
        print(f'iter:{i}, return:{np.mean(return_list[-10:])}')

    plt.plot(return_list)
    plt.title('return')
    plt.show()
