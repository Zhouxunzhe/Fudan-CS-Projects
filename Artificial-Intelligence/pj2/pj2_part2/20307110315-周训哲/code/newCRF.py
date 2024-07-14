import numpy as np
from MyFeature import load_feature


class CRF:
    def __init__(self, states, observations):
        self.states = states
        self.observations = observations
        self.n_states = len(states)
        self.n_observations = len(observations)
        self.trans = np.zeros((self.n_states, self.n_states))
        self.unigram = np.zeros((self.n_states, self.n_observations))
        self.bigram = np.zeros((self.n_states, self.n_states, self.n_observations))
        self.features = load_feature('Chinese_template.txt')
        """
        class feature{
            gram_size: 1 or 2
            off: [off1, off2]
            unigram_prob
            bigram_prob
        }
        """

    def fit(self, train_data):
        for data in train_data:
            for i in range(len(data[0]) - 1):
                state_i = data[1][i]
                state_j = data[1][i + 1]
                j = self.states.index(state_i)
                k = self.states.index(state_j)
                self.trans[j, k] += 1
            # 计算n-gram出现次数
            words = data[0]
            tags = data[1]
            for feature in self.features:
                # gram_size = 1
                feat = feature.feat
                if feat == 1:
                    off = feature.off
                    for i in range(max(0, -off), min(len(words) - off, len(words))):
                        self.unigram[self.states.index(tags[i])][self.observations.index(words[i+off])] += 1
                if feat == 2:
                    off = feature.off
                    for i in range(max(0, -off), min(len(words) - off, len(words))):
                        self.bigram[self.states.index(tags[i-1])][self.states.index(tags[i])][
                            self.observations.index(words[i+off])] += 1

    def predict(self, observations):
        test_states = []
        for observation in observations:
            # Viterbi 算法
            n_steps = len(observation)
            dp = np.zeros((n_steps, self.n_states))
            psi = np.zeros((n_steps, self.n_states), dtype=np.int)
            for i in range(n_steps):
                for j in range(self.n_states):
                    score = dp[i - 1] + self.trans[:, j]
                    psi[i, j] = np.argmax(score)
                    for feature in self.features:
                        off = feature.off
                        if 0 <= i+off < len(observation) and observation[i+off] in self.observations:
                            dp[i, j] = score[psi[i, j]] + \
                                       self.unigram[j, self.observations.index(observation[i+off])] + \
                                       self.bigram[psi[i, j], j, self.observations.index(observation[i+off])]
            path = np.zeros(n_steps, dtype=np.int)
            path[n_steps - 1] = np.argmax(dp[n_steps - 1])
            for i in range(n_steps - 2, -1, -1):
                path[i] = psi[i + 1, path[i + 1]]
            test_states.append([self.states[i] for i in path])

        return test_states
