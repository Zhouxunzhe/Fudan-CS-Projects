import pickle
import json
import numpy as np
import warnings

np.warnings = warnings
from sklearn.decomposition import PCA
from pyclustering.cluster import cluster_visualizer
from pyclustering.cluster.center_initializer import kmeans_plusplus_initializer
from pyclustering.cluster.fcm import fcm
from pyclustering.cluster.birch import birch
from pyclustering.cluster.kmeans import kmeans
from pyclustering.cluster.cure import cure
from pyclustering.cluster.rock import rock
from pyclustering.samples.definitions import FAMOUS_SAMPLES
from pyclustering.utils import read_sample
from sklearn.metrics import silhouette_score
import matplotlib.pyplot as plt


def plot_cluster(result, newData, numClass):  # 聚类数据可视化
    plt.figure(2)
    Lab = [[] for i in range(numClass)]
    index = 0
    for labi in result:
        Lab[labi].append(index)
        index += 1
    # color = ['oy', 'ob', 'og', 'cs', 'ms', 'bs', 'ks', 'ys', 'yv', 'mv', 'bv', 'kv', 'gv', 'y^', 'm^', 'b^', 'k^',
    #          'g^'] * 5
    colors = ['#9400D3', '#00FFFF', '#000000', '#FFE4C4', '#A52A2A', '#FF7F50', '#D2691E', '#DC143C', '#8B0000',
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
    # x1 = []
    # y1 = []
    # for ind1 in clf.cluster_centers_:
    #     try:
    #         y1.append(ind1[1])
    #         x1.append(ind1[0])
    #     except:
    #         pass
    # plt.plot(x1, y1, "rv") #绘制中心
    plt.show()


def output_result(s, filename):
    fp = open(filename, "w", encoding="utf-8")
    Ori_data_fp = open("ift_cluster_given_fudandm2023.jsonl", "r", encoding="utf-8")
    count = 0
    for line in Ori_data_fp:
        L_data = json.loads(line)
        L_data["label"] = str(s[count])
        json.dump(L_data, fp, ensure_ascii=False)
        fp.write('\n')
        count += 1
    fp.close()


num_class = 99
dim = 20
N = 1
# fp = open("sent_vector_data.pkl","rb")                                #选取源数据，该数据需降维
# data = pickle.load(fp)  # 33715 *768

# fp = open("./Sent_vector_data/Direct_TSNE2.pkl","rb")                 #选取源数据，该数据已经降维
fp = open("./Sent_vector_data/Direct_TSNE2_TI-IDF.pkl", "rb")
# fp = open("./Sent_vector_data/Direct_TSNE3_RBWE.pkl","rb")
Tnewdata = pickle.load(fp)

# pca = PCA(n_components=dim)                                           #降维算法
# Tnewdata = pca.fit_transform(data)        


score_list = {}
max = 0
max_index = 0
for k in range(N):
    # center = kmeans_plusplus_initializer(Tnewdata,num_class + k).initialize() #kmeans类算法选取初始质点
    # fcm_ins = fcm(Tnewdata,center)                                            #选取聚类算法
    # fcm_ins = kmeans(Tnewdata,center)
    # fcm_ins = clique(Tnewdata,10,3)
    # fcm_ins = birch(Tnewdata,70,diameter=2.0)
    fcm_ins = cure(Tnewdata, num_class)
    # fcm_ins = rock(Tnewdata,1.0,70)
    fcm_ins.process()
    clusters = fcm_ins.get_clusters()
    label = np.zeros(33715)
    label = label.astype(np.int32)  # 统一规格化标签
    for i in range(clusters.__len__()):
        for j in range(clusters[i].__len__()):
            label[clusters[i][j]] = i
    score = silhouette_score(Tnewdata, label)  # 计算轮廓系数
    score_list[num_class + k] = score
    print("{}: {}".format(num_class + k, score))
    if score > max:  # 多轮迭代时保存最佳轮廓系数对应数据
        filename = "./Cluster_Result/cluster_" + str(num_class + k) + "class_cure_TSNE_TI-IDF.jsonl"
        output_result(label, filename)
        # max = score
        # max_index = num_class + k
# print("max {}:{}".format(max_index,max))                                      #输出最佳轮廓系数及其簇数
# fp = open("./score_cure_list.pkl","wb")                                       #保存轮廓系数
# pickle.dump(score_list,fp)
# visualizer = cluster_visualizer()                                             #类自带可视化，但是速度较慢
# visualizer.append_clusters(clusters,Tnewdata)
# visualizer.show()
plot_cluster(label, Tnewdata, num_class)  # 自实现可视化
print("Done")
