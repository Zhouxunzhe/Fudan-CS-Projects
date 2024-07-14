from check import check
import pickle

if __name__ == '__main__':
    language = 'English'
    with open(language + '_mycrf.pickle', 'rb') as f:
        crf = pickle.load(f)

    test_path = "C:\\Users\\ZhouXunZhe\\Desktop\\" + language + '_test.txt'
    output_path = language + '\\my_crf_result.txt'

    crf.predict(test_path, output_path)
    check(language, test_path, language + "\\my_crf_result.txt")
