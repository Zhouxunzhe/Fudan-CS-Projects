from time import time
import numpy as np
import matplotlib.pyplot as plt
from sklearn import datasets
from sklearn.manifold import TSNE

"""获取数据"""


def get_data():
    digits = datasets.load_digits(n_class=10)  # 取前六种数字图片，0-5
    data = digits.data  # data.shape=[1083,64]，表示1084张图片，每个图片8*8但是将图片表示为一个行向量
    label = digits.target  # 表示取出的1083个图片对应的数字
    n_samples, n_features = data.shape  # 图片数1083和每张图片的维度64
    return data, label, n_samples, n_features


"""显示数据"""


def plot_embedding(result, label, title):  # 传入1083个2维数据，1083个标签，图表标题
    x_min, x_max = np.min(result, 0), np.max(result, 0)  # 分别求出每一列最小值和最大值
    data = (result - x_min) / (x_max - x_min)  # 将数据进行正则化，分母为数据的总长度，因此分子一定小于分母，生成的矩阵元素都是0-1区间内的
    plt.figure()  # 创建一个画布
    for i in range(data.shape[0]):  # 遍历所有的数据，共1083个
        plt.text(data[i, 0], data[i, 1], str(label[i]),
                 color=plt.cm.Set1(label[i] / 10.),
                 fontdict={'weight': 'bold', 'size': 9})  # 将该点的标签设置颜色显示到对应位置
    plt.title(title)  # 设置标题
    plt.show()


"""主函数"""


def main():
    data, label, n_samples, n_features = get_data()  # data种保存[1083,64]的向量
    tsne = TSNE(n_components=2, init='pca',
                random_state=0)  # n_components将64维降到该维度，默认2；init设置embedding初始化方式，可选pca和random，pca要稳定些
    t0 = time()  # 记录开始时间
    result = tsne.fit_transform(data)  # 进行降维，[1083,64]-->[1083,2]
    plot_embedding(result, label, 't-SNE embedding of the digits (time %.2fs)' % (time() - t0))  # 显示数据


if __name__ == '__main__':
    main()