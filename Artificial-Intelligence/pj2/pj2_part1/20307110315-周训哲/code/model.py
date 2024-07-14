import numpy as np


class HMM:
    def __init__(self, states, observations):
        self.states = states
        self.observations = observations
        self.n_states = len(states)
        self.n_observations = len(observations)
        self.initial_prob = np.zeros(self.n_states)
        self.transition_prob = np.zeros((self.n_states, self.n_states))
        self.emission_prob = np.zeros((self.n_states, self.n_observations))
        self.min = 1e-100

    def fit(self, train_data):
        # 计算初始概率
        for data in train_data:
            for i in range(len(data[0])):
                state = data[1][i]
                j = self.states.index(state)
                self.initial_prob[j] += 1
            # 计算状态转移概率
            for i in range(len(data[0]) - 1):
                state_i = data[1][i]
                state_j = data[1][i + 1]
                j = self.states.index(state_i)
                k = self.states.index(state_j)
                self.transition_prob[j, k] += 1
            # 计算发射概率
            for i in range(len(data[0])):
                state = data[1][i]
                observation = data[0][i]
                j = self.states.index(state)
                k = self.observations.index(observation)
                self.emission_prob[j, k] += 1

        self.initial_prob[self.initial_prob == 0] = self.min
        self.initial_prob = np.log(self.initial_prob) - np.log(np.sum(self.initial_prob))
        self.transition_prob[self.transition_prob == 0] = self.min
        self.transition_prob = np.log(self.transition_prob) - np.log(np.sum(self.transition_prob, axis=1, keepdims=True))
        self.emission_prob[self.emission_prob == 0] = self.min
        self.emission_prob = np.log(self.emission_prob) - np.log(np.sum(self.emission_prob, axis=1, keepdims=True))

    def predict(self, observations):
        test_states = []
        for observation in observations:
            # Viterbi 算法
            n_steps = len(observation)
            dp = np.zeros((n_steps, self.n_states))
            psi = np.zeros((n_steps, self.n_states), dtype=np.int)
            if observation[0] in self.observations:
                dp[0] = self.initial_prob + self.emission_prob[:, self.observations.index(observation[0])]
            else:
                dp[0] = np.log(self.min)
            for i in range(1, n_steps):
                for j in range(self.n_states):
                    score = dp[i - 1] + self.transition_prob[:, j]
                    psi[i, j] = np.argmax(score)
                    if observation[i] in self.observations:
                        dp[i, j] = score[psi[i, j]] + self.emission_prob[j, self.observations.index(observation[i])]
                    else:
                        dp[i, j] = np.log(self.min)
            path = np.zeros(n_steps, dtype=np.int)
            path[n_steps - 1] = np.argmax(dp[n_steps - 1])
            for i in range(n_steps - 2, -1, -1):
                path[i] = psi[i + 1, path[i + 1]]
            test_states.append([self.states[i] for i in path])

        return test_states
