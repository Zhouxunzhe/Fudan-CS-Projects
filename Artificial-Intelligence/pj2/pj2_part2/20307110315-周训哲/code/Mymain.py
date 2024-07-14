from MyCRF import my_CRF
from check import check
import pickle

if __name__ == '__main__':
    language = 'English'
    train_path = language + '\\train.txt'
    valid_path = language + '\\validation.txt'
    output_path = language + '\\my_crf_result.txt'

    crf = my_CRF()
    crf.train(train_path, language)
    crf.predict(valid_path, output_path)
    check(language, valid_path, language + "\\my_crf_result.txt")

    with open(language + '_mycrf.pickle', 'wb') as f:
        pickle.dump(crf, f)
