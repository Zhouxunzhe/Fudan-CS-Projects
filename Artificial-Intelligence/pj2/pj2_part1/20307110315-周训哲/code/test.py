from load import *
from check import check
import pickle

if __name__ == '__main__':
    language = 'English'
    f = open(language + '_hmm.pickle', 'rb')
    hmm = pickle.load(f)
    f.close()

    test_path = "C:\\Users\\ZhouXunZhe\\Desktop\\" + language + '_test.txt'
    _, test_observations = load_data(test_path)
    test_states = hmm.predict(test_observations)

    with open(language + '\\result.txt', 'w', encoding='UTF-8') as f:
        for observation, state in zip(test_observations, test_states):
            for o, s in zip(observation, state):
                f.write(str(o) + ' ' + str(s) + '\n')
            f.write('\n')
    f.close()

    check(language, test_path, language + "\\result.txt")
