import matplotlib.pyplot as plt
import pickle


def visualize_score(score_list, title):  # 对score的可视化
    X = []
    Y = []
    for class_num, score in score_list.items():
        X.append(class_num)
        Y.append(score)
    plt.plot(X, Y)
    plt.title(title)
    plt.xlabel("class_num")
    # plt.xlabel("dim")
    plt.ylabel("score")
    plt.show()


def visualize_data(data):  # 对二维数据的可视化
    X = []
    Y = []
    for i in range(data.__len__()):
        X.append(data[i][0])
        Y.append(data[i][1])
    plt.scatter(X, Y, s=1)
    plt.show()


# fp = open("./Sent_vector_data/Direct_TSNE2_TI-IDF.pkl","rb")
fp = open("./score_result/cluster_score_SC_TI-IDF.pkl", "rb")
data = pickle.load(fp)
visualize_score(data, "SC_score")
# visualize_data(data)
