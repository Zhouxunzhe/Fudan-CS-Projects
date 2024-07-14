from Feature import *


def get_train_data(file_path):
    states = []
    observation = []
    with open(file_path, 'r', encoding='UTF-8') as f:
        lines = f.readlines()
        for i in range(len(lines)):
            if lines[i] == "\n":
                continue
            word, tag = lines[i].strip().split(" ")
            observation.append(word)
            states.append(tag)
    data = list(zip(observation, states))
    f.close()
    return states, observation, data


# 读取文件获取数据
def get_divided_data(file_path):
    data = []
    states = []
    observ = []
    data_list = []
    states_list = []
    observ_list = []
    with open(file_path, 'r', encoding='UTF-8') as f:
        for line in f:
            ls = line.strip('\n').split(' ')
            if line != '\n':
                d = (ls[0], ls[1])
                observ.append(ls[0])
                states.append(ls[1])
                data.append(d)
            if line == '\n':
                data.append(('\n', ''))
                observ.append('\n')
                states.append('')
                data_list.append(data)
                states_list.append(states)
                observ_list.append(observ)
                data = []
                states = []
                observ = []
    f.close()
    return states_list, observ_list, data_list


# 定义特征提取函数，用于将每个句子中的单词转换为特征向量序列
def sent2features(sent, language):
    if language == 'Chinese':
        return [feat2(sent, i) for i in range(len(sent))]
    elif language == 'English':
        return [feat1(sent, i) for i in range(len(sent))]


def load_data(file_path):
    states = []
    observation = []
    s = []
    o = []
    with open(file_path, 'r', encoding='UTF-8') as f:
        lines = f.readlines()
        for i in range(len(lines)):
            if lines[i] == "\n":
                states.append(s)
                observation.append(o)
                s = []
                o = []
                continue
            word, tag = lines[i].strip().split(" ")
            o.append(word)
            s.append(tag)
    f.close()
    return states, observation
