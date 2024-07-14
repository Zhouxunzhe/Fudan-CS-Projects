from HMM import HMM
from check import check
import pickle

if __name__ == '__main__':
    language = 'Chinese'
    train_path = language + '\\train.txt'
    valid_path = language + '\\validation.txt'
    output_path = language + '\\result.txt'

    hmm = HMM()
    hmm.train(train_path)
    hmm.predict(valid_path, output_path)
    check(language, valid_path, language + "\\result.txt")

    f = open(language + '_hmm.pickle', 'wb')
    pickle.dump(hmm, f)
    f.close()
