import numpy as np


class HMM:
    def __init__(self):
        self.tag2id = {}
        self.id2tag = {}
        self.word2id = {}
        self.id2word = {}
        self.M = 0
        self.N = 0
        self.pi = None
        self.A = None
        self.B = None

    def train(self, train_path):
        for line in open(train_path, "r", encoding='utf-8'):
            if line == "\n":
                continue
            items = line.split()
            word, tag = items[0], items[1].rstrip()

            if word not in self.word2id:
                self.word2id[word] = len(self.word2id)
                self.id2word[len(self.id2word)] = word

            if tag not in self.tag2id:
                self.tag2id[tag] = len(self.tag2id)
                self.id2tag[len(self.id2tag)] = tag

        self.M = len(self.word2id)
        self.N = len(self.tag2id)
        self.pi = np.zeros(self.N)
        self.A = np.zeros((self.N, self.N))
        self.B = np.zeros((self.N, self.M))
        pre_tag = ""
        for line in open(train_path, 'r', encoding='utf-8'):
            if line == "\n":
                pre_tag = ""
                continue
            items = line.split()
            wordId, tagId = self.word2id[items[0]], self.tag2id[items[1].rstrip()]
            if pre_tag == "":
                self.pi[tagId] += 1
                self.B[tagId][wordId] += 1
            else:
                self.B[tagId][wordId] += 1
                self.A[self.tag2id[pre_tag]][tagId] += 1
            pre_tag = items[1].strip()
        self.pi = self.pi / sum(self.pi)
        for i in range(self.N):
            self.A[i] /= sum(self.A[i])
            self.B[i] /= sum(self.B[i])

    def log(self, v):
        if v == 0:
            return np.log(v + 0.00001)
        return np.log(v)

    # Î¬ÌØ±ÈËã·¨
    def viterbi(self, x):
        for i in range(len(x)):
            word = x[i]
            if self.word2id.get(word) is None:
                x[i] = 2
            else:
                x[i] = self.word2id[word]
        T = len(x)
        dp = np.zeros((T, self.N))
        ptr = np.zeros((T, self.N), dtype=int)
        for j in range(self.N):
            dp[0][j] = self.log(self.pi[j]) + self.log(self.B[j][x[0]])
        for i in range(1, T):
            for j in range(self.N):
                dp[i][j] = -99999
                for k in range(self.N):
                    score = dp[i - 1][k] + self.log(self.A[k][j]) + self.log(self.B[j][x[i]])
                    if score > dp[i][j]:
                        dp[i][j] = score
                        ptr[i][j] = k
        path = [0] * T
        path[T - 1] = np.argmax(dp[T - 1])
        for i in range(T - 2, -1, -1):
            path[i] = ptr[i + 1][path[i + 1]]
        states = []
        for i in range(len(path)):
            states.append(self.id2tag[path[i]])
        return states

    def predict(self, test_path, output_path):
        fp = open(test_path, "r", encoding='utf-8')
        fp2 = open(output_path, 'w+', encoding='utf-8')
        words = []
        states = []
        line_count = 0
        for line in fp:
            if line == "\n":
                pred = self.viterbi(words)
                count = 0
                for i in range(len(states)):
                    if states[i] == pred[i]: count += 1
                    fp2.write(self.id2word[words[i]] + " " + pred[i] + '\n')
                fp2.write('\n')
                states.clear()
                words.clear()
                line_count += 1
                continue
            items = line.split()
            word, state = items[0], items[1].rstrip()
            words.append(word)
            states.append(state)
        if words.__len__() != 0:
            pred = self.viterbi(words)
            for i in range(len(states)):
                fp2.write(self.id2word[words[i]] + " " + pred[i] + '\n')
        fp.close()
        fp2.close()
