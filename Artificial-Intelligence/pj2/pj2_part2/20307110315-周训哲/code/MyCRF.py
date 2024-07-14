import numpy as np
from MyFeature import *


class my_CRF:
    def __init__(self):
        self.tag2id = {}
        self.id2tag = {}
        self.word2id = {}
        self.id2word = {}
        self.M = 0
        self.N = 0
        self.trans = None
        self.features = None
        self.unigram = None
        self.bigram = None
        self.multiunigram = None
        self.multibigram = None

    def train(self, train_path, language):
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
        self.trans = np.zeros((self.N, self.N))
        self.unigram = np.zeros((self.N, self.M))
        self.bigram = np.zeros((self.N, self.N, self.M))
        self.multiunigram = np.zeros((self.N, self.M, self.M))
        self.multibigram = np.zeros((self.N, self.N, self.M, self.M))
        self.features = load_feature(language + '_template.txt')
        pre_tag = ""
        with open(train_path, 'r', encoding='utf-8') as f:
            lines = f.readlines()
            for line in lines:
                if line == "\n":
                    pre_tag = ""
                    continue
                items = line.split()
                wordId, tagId = self.word2id[items[0]], self.tag2id[items[1].rstrip()]
                if pre_tag != "":
                    self.trans[self.tag2id[pre_tag]][tagId] += 1
                pre_tag = items[1].strip()
            for feature in self.features:
                # gram_size = 1
                feat = feature.feat
                if feat == 1:
                    if feature.size == 1:
                        off = feature.off[0]
                        for i in range(max(0, -off), min(len(lines) - off, len(lines))):
                            if lines[i + off] != '\n' and lines[i] != '\n':
                                items_1 = lines[i].split()
                                wordId_1, tagId_1 = self.word2id[items_1[0]], self.tag2id[items_1[1].rstrip()]
                                items_2 = lines[i + off].split()
                                wordId_2, tagId_2 = self.word2id[items_2[0]], self.tag2id[items_2[1].rstrip()]
                                self.unigram[tagId_1][wordId_2] += 1
                    if feature.size == 2:
                        off1, off2 = feature.off[0], feature.off[1]
                        for i in range(max(0, -max(off1, off2)), min(len(lines) - max(off1, off2), len(lines))):
                            if lines[i + off1] != '\n' and lines[i] != '\n' and lines[i + off2] != '\n':
                                items_1 = lines[i].split()
                                wordId_1, tagId_1 = self.word2id[items_1[0]], self.tag2id[items_1[1].rstrip()]
                                items_2 = lines[i + off1].split()
                                wordId_2, tagId_2 = self.word2id[items_2[0]], self.tag2id[items_2[1].rstrip()]
                                items_3 = lines[i + off2].split()
                                wordId_3, tagId_3 = self.word2id[items_3[0]], self.tag2id[items_3[1].rstrip()]
                                self.multiunigram[tagId_1][wordId_2][wordId_3] += 1
                if feat == 2:
                    if feature.size == 1:
                        off = feature.off[0]
                        for i in range(max(0, -off), min(len(lines) - off, len(lines))):
                            if lines[i + off] != '\n' and lines[i] != '\n' and lines[i - 1] != '\n':
                                items_1 = lines[i - 1].split()
                                wordId_1, tagId_1 = self.word2id[items_1[0]], self.tag2id[items_1[1].rstrip()]
                                items_2 = lines[i].split()
                                wordId_2, tagId_2 = self.word2id[items_2[0]], self.tag2id[items_2[1].rstrip()]
                                items_3 = lines[i + off].split()
                                wordId_3, tagId_3 = self.word2id[items_3[0]], self.tag2id[items_3[1].rstrip()]
                                self.bigram[tagId_1][tagId_2][wordId_3] += 1
                    if feature.size == 2:
                        off1, off2 = feature.off[0], feature.off[1]
                        for i in range(max(0, -max(off1, off2)), min(len(lines) - max(off1, off2), len(lines))):
                            if lines[i + off1] != '\n' and lines[i] != '\n' and lines[i + off2] != '\n' and lines[i - 1] != '\n':
                                items_0 = lines[i - 1].split()
                                wordId_0, tagId_0 = self.word2id[items_0[0]], self.tag2id[items_0[1].rstrip()]
                                items_1 = lines[i].split()
                                wordId_1, tagId_1 = self.word2id[items_1[0]], self.tag2id[items_1[1].rstrip()]
                                items_2 = lines[i + off1].split()
                                wordId_2, tagId_2 = self.word2id[items_2[0]], self.tag2id[items_2[1].rstrip()]
                                items_3 = lines[i + off2].split()
                                wordId_3, tagId_3 = self.word2id[items_3[0]], self.tag2id[items_3[1].rstrip()]

                                self.multibigram[tagId_0][tagId_1][wordId_2][wordId_3] += 1

    def log(self, v):
        if v == 0:
            return np.log(v + 0.00001)
        return np.log(v)

    # 维特比算法
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
        for i in range(T):
            for j in range(self.N):
                dp[i][j] = -99999
                for k in range(self.N):
                    score = dp[i - 1][k]
                    for feature in self.features:
                        if feature.size == 1:
                            off = feature.off[0]
                            if 0 <= i + off < T:
                                score += self.unigram[j][x[i + off]] + self.bigram[k][j][x[i + off]]
                        if feature.size == 2:
                            off1, off2 = feature.off[0], feature.off[1]
                            if 0 <= i + off1 < T and 0 <= i + off2 < T:
                                score += self.multiunigram[j][x[i + off1]][x[i + off2]] + self.multibigram[k][j][x[i + off1]][x[i + off2]]
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
                    if states[i] == pred[i]:
                        count += 1
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
