import numpy as np
import pickle
from sklearn.decomposition import PCA
from sklearn.manifold import TSNE
import matplotlib.pyplot as plt

dim = 2

# fp = open("./Original_vector_data/sent_vector_data2.pkl","rb")
# data = pickle.load(fp)

fp = open("./Original_vector_data/TI-IDF_vector.pkl", "rb")  # 选择源数据
data = pickle.load(fp)

# fp = open("./Sent_vector_data/PCA4_TSNE2.pkl","rb")
# newdata = pickle.load(fp)

# pca = PCA(n_components=2)                                                 #选择降维方法
# newdata = pca.fit_transform(data)
tsne = TSNE(n_components=2)
newdata = tsne.fit_transform(data)
fp2 = open("./Sent_vector_data/Direct_TSNE2_TI-IDF.pkl", "wb")  # 保存降维结果
pickle.dump(newdata, fp2)

# for i in range(17):                                                       #PCA-TSNE降维，先降维至i维，最终降维至2维保存多个降维结果
#     print("{}/{}".format(i,17))
#     pca = PCA(n_components= dim + i + 1)
#     newdata = pca.fit_transform(data)
#     tsne = TSNE(n_components=2)
#     newdata = tsne.fit_transform(newdata)
#     filename = "./Sent_vector_data/PCA" + str(dim + i + 1 ) +  "_TSNE2.pkl"
#     save_fp = open(filename,"wb")
#     pickle.dump(newdata,save_fp)
#     save_fp.close()

# for i in range(3,20):                                                     #降维后的二维图片保存
#     plt.clf()
#     filename = "./Sent_vector_data/PCA" + str(i) + "_TSNE2.pkl"
#     fp = open(filename,"rb")
#     data = pickle.load(fp)
#     plt.scatter(data.T[0],data.T[1],s=1)
#     savename = "./Data_Pic/PCA" + str(i) + "_TSNE2.png"
#     plt.savefig(savename)
#     fp.close()
