from sklearn.cluster import KMeans, DBSCAN
from sklearn.decomposition import PCA
from sklearn.decomposition import NMF
from sklearn.manifold import TSNE
from sklearn.metrics import silhouette_score
from sklearn.metrics import davies_bouldin_score, calinski_harabasz_score
import matplotlib.pyplot as plt
import pickle
import numpy as np
import json
from pyclustering import cluster


def plot_cluster(result, newData, numClass):  # 聚类数据可视化
    plt.figure(2)
    Lab = [[] for i in range(numClass)]
    index = 0
    for labi in result:
        Lab[labi].append(index)
        index += 1
    # color = ['oy', 'ob', 'og', 'cs', 'ms', 'bs', 'ks', 'ys', 'yv', 'mv', 'bv', 'kv', 'gv', 'y^', 'm^', 'b^', 'k^',
    #          'g^'] * 5
    colors = ['#F0F8FF', '#00FFFF', '#000000', '#FFE4C4', '#A52A2A', '#FF7F50', '#D2691E', '#DC143C', '#8B0000',
              '#FF8C00'] * 11
    for i in range(numClass):
        x1 = []
        y1 = []
        for ind1 in newData[Lab[i]]:
            # print ind1
            try:
                y1.append(ind1[1])
                x1.append(ind1[0])
            except:
                pass
        plt.scatter(x1, y1, c=colors[i], s=1)

    # 绘制初始中心点
    x1 = []
    y1 = []
    for ind1 in clf.cluster_centers_:
        try:
            y1.append(ind1[1])
            x1.append(ind1[0])
        except:
            pass
    plt.plot(x1, y1, "rv")  # 绘制中心
    plt.show()


def output_result(s):
    fp = open("cluster_99class_Direct_TSNE2_TI-IDF.jsonl", "w", encoding="utf-8")
    Ori_data_fp = open("ift_cluster_given_fudandm2023.jsonl", "r", encoding="utf-8")
    count = 0
    for line in Ori_data_fp:
        L_data = json.loads(line)
        L_data["label"] = str(s.labels_[count])
        json.dump(L_data, fp, ensure_ascii=False)
        fp.write('\n')
        count += 1
    fp.close()


def data_transform(data):
    newdata = np.zeros((33715, 2))
    newdata[:, 0] = np.mean(data[:, 0:384], axis=1)
    newdata[:, 1] = np.mean(data[:, 384:768], axis=1)
    return newdata


num_class = 70  # 选择簇数
dim = 20  # 降维数
N = 30  # 进行多少次循环

# fp = open("sent_vector_data.pkl","rb")                            #选择源数据，该数据需要进行降维
# data = pickle.load(fp)
fp = open("./Sent_vector_data/Direct_TSNE2_TI-IDF.pkl", "rb")  # 选择源数据，该数据已经进行了降维
Tnewdata = pickle.load(fp)

score1_list = {}  # 保存内部评价指标
score2_list = {}
score3_list = {}

# Tnewdata = data_transform(data)                                   #选择各种降维方法
# pca = PCA(n_components=dim)
# Tnewdata = pca.fit_transform(data)
# tsne = TSNE(n_components=2)
# Tnewdata = tsne.fit_transform(Tnewdata)
# Tnewdata = data[:,0:dim]
# Tnewdata = data

max = 0
max_index = 0
for i in range(N):
    # pca = PCA(n_components=dim - i)
    # Tnewdata = pca.fit_transform(data)
    clf = KMeans(n_clusters=num_class + i, init="k-means++", max_iter=10000)  # 选择聚类方法
    # clf = SpectralClustering(n_clusters=num_class + i)
    # clf = DBSCAN()
    s = clf.fit(Tnewdata)
    # output_result(s)
    s_pred = clf.predict(Tnewdata)
    score1 = silhouette_score(Tnewdata, s_pred)  # 轮廓系数计算
    score1_list[num_class + i] = score1
    if score1 > max:
        max = score1
        max_index = i + num_class
        # max_index = dim - i
    # score2 = calinski_harabasz_score(Tnewdata,s_pred)                         #方差比计算
    # score3 = davies_bouldin_score(Tnewdata,s_pred)                            #DB指数计算
    # score2_list[num_class + i] = score2
    # score3_list[num_class + i] = score3
    print("{}class 轮廓系数: {}\n".format(i + num_class, score1))
    # print("{}class 方差比: {}\n".format(i + num_class,score2))
    # print("{}class DB指数: {}\n".format(i + num_class,score3))

# result = list(s_pred)
# plot_cluster(result,Tnewdata,num_class)

print("{}class: {}".format(max, max_index))  # 输出最大轮廓系数以及其对应簇数


def save_score(score1_list, score2_list, score3_list):  # 保存score
    fp = open("./score_result/cluster_score_SC_TI-IDF.pkl", "wb")
    pickle.dump(score1_list, fp)
    fp.close()
    # fp = open("./score_result/cluster_score_CH.pkl","wb")
    # pickle.dump(score2_list,fp)
    # fp.close()
    # fp = open("./score_result/cluster_score_DB.pkl","wb")
    # pickle.dump(score3_list,fp)
    # fp.close()


save_score(score1_list, None, None)
