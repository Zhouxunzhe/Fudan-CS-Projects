import pickle
from utils import *
from check import check

if __name__ == '__main__':
    language = 'Chinese'
    f = open(language + '_crf.pickle', 'rb')
    crf = pickle.load(f)
    f.close()

    test_path = "C:\\Users\\ZhouXunZhe\\Desktop\\" + language + '_test.txt'
    test_states, test_observation, test_data = get_divided_data(test_path)

    x_test = [sent2features(s, language) for s in test_observation]
    pred_states = crf.predict(x_test)

    with open(language + '\\result.txt', 'w', encoding='UTF-8') as f:
        for observation, state in zip(test_observation, pred_states):
            for o, s in zip(observation, state):
                if o != '\n':
                    f.write(str(o) + ' ' + str(s) + '\n')
                else:
                    f.write('\n')
    f.close()

    check(language, test_path, language + "\\result.txt")
