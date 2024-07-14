from HMM import HMM
from check import check
import pickle

if __name__ == '__main__':
    language = 'English'
    f = open(language + '_hmm.pickle', 'rb')
    hmm = pickle.load(f)
    f.close()

    test_path = "C:\\Users\\ZhouXunZhe\\Desktop\\" + language + '_test.txt'
    output_path = language + '\\result.txt'

    hmm.predict(test_path, output_path)
    check(language, test_path, language + "\\result.txt")
