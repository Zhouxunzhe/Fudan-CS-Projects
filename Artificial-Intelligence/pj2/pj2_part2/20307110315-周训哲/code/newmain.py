from newCRF import CRF
from utils import *
from check import check
import pickle

if __name__ == '__main__':
    language = 'Chinese'
    train_path = language + '\\train.txt'
    valid_path = language + '\\validation.txt'
    train_states, train_observations = load_data(train_path)
    valid_states, valid_observations = load_data(valid_path)
    train_data = list(zip(train_observations, train_states))

    observations = []
    states = []
    for sentence, tags in train_data:
        for word in sentence:
            if word not in observations:
                observations.append(word)
        for tag in tags:
            if tag not in states:
                states.append(tag)

    crf = CRF(states, observations)
    crf.fit(train_data)

    test_states = crf.predict(valid_observations)

    with open(language + '\\result.txt', 'w', encoding='UTF-8') as f:
        for observation, state in zip(valid_observations, test_states):
            for o, s in zip(observation, state):
                f.write(str(o) + ' ' + str(s) + '\n')
            f.write('\n')

    check(language, valid_path, language + "\\my_crf_result.txt")

    f = open(language + '_mycrf.pickle', 'wb')
    pickle.dump(crf, f)
    f.close()
