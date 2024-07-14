import pickle
from utils import sent2features

if __name__ == '__main__':
    f = open('Chinese_crf.pickle', 'rb')
    crf = pickle.load(f)
    f.close()

    stri = "谢卫东先生：1966年12月出生，汉族，硕士研究生、高级工程师、国家注册造价工程师。"
    # stri = "现任安徽新华传媒股份有限公司副董事长。"
    # stri = "周训哲任复旦大学计算机足球队队长一职。"
    # stri = "周训哲是复旦大学本科生里面最帅的人。"
    pred_tag = crf.predict([sent2features(stri, language="Chinese")])[0]
    cws = []
    for i in range(len(pred_tag) - 1):
        cws.extend(stri[i])
        current_state = pred_tag[i]
        next_state = pred_tag[i + 1]
        if current_state[0] == 'E' or current_state[0] == 'S' or next_state[0] == 'B':
            cws.append('/')
    cws.append(stri[len(stri)-1])
    print(stri)
    print(''.join(cws))
