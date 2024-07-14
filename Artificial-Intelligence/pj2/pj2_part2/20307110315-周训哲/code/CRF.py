import sklearn_crfsuite
from utils import get_divided_data, sent2features
from check import check
import pickle

if __name__ == '__main__':
    language = 'Chinese'
    train_path = language + '\\train.txt'
    validation_path = language + '\\validation.txt'
    train_states, train_observation, train_data = get_divided_data(train_path)
    validation_states, validation_observation, validation_data = get_divided_data(validation_path)

    # 定义标签序列
    train_states = [l + ['O'] * (len(s) - len(l)) for s, l in zip(train_observation, train_states)]
    validation_states = [l + ['O'] * (len(s) - len(l)) for s, l in zip(validation_observation, validation_states)]

    # 将训练数据和测试数据转换为特征向量序列
    x_train = [sent2features(s, language) for s in train_observation]
    y_train = train_states
    x_test = [sent2features(s, language) for s in validation_observation]
    y_test = validation_states

    # 定义CRF模型
    crf = sklearn_crfsuite.CRF(
        algorithm='lbfgs',
        c1=0.1,
        c2=0.1,
        max_iterations=100,
        all_possible_transitions=False
    )
    # 训练模型
    crf.fit(x_train, y_train)
    # 预测标签序列
    test_states = crf.predict(x_test)

    with open(language + '\\result.txt', 'w', encoding='UTF-8') as f:
        for observation, state in zip(validation_observation, test_states):
            for o, s in zip(observation, state):
                if o != '\n':
                    f.write(str(o) + ' ' + str(s) + '\n')
                else:
                    f.write('\n')
    f.close()

    check(language, validation_path, language + "\\result.txt")

    f = open(language + '_crf.pickle', 'wb')
    pickle.dump(crf, f)
    f.close()
