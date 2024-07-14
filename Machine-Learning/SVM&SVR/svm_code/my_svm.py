import struct
import array
import numpy as np
import random
import matplotlib.pyplot as plt


def load_data(img_file, lbl_file):
    labels = []
    with open(lbl_file, 'rb') as file:
        magic, size = struct.unpack(">II", file.read(8))
        if magic != 2049:
            raise ValueError('Magic number mismatch, expected 2049,'
                             'got {}'.format(magic))

        label_data = array.array("B", file.read())
        for i in range(size):
            labels.append(label_data[i])
    with open(img_file, 'rb') as file:
        magic, size, rows, cols = struct.unpack(">IIII", file.read(16))
        if magic != 2051:
            raise ValueError('Magic number mismatch, expected 2051,'
                             'got {}'.format(magic))
        image_data = array.array("B", file.read())
        # 生成一个零矩阵
        images = np.zeros((size, rows * cols))

        for i in range(size):
            if i + 1 == size:
                print("%d numbers imported" % (i+1))
            images[i, :] = image_data[i * rows * cols: (i + 1) * rows * cols]
    return images, labels, size


def clip(alpha, L, H):
    # 修建alpha的值到L和H之间.
    if alpha < L:
        return L
    elif alpha > H:
        return H
    else:
        return alpha


def select_j(i, m):
    # 在m中随机选择除了i之外剩余的数
    l = list(range(m))
    seq = l[: i] + l[i+1:]
    return random.choice(seq)


def get_w(alphas, dataset, labels):
    # 通过已知数据点和拉格朗日乘子获得分割超平面参数w
    alphas, dataset, labels = np.array(alphas), np.array(dataset), np.array(labels)
    labels = labels.reshape(1, -1).T
    yx = labels * np.ones(784) * dataset
    w = np.dot(yx.T, alphas)
    return w.tolist()


def simple_smo(trainImages, trainLabels, C, max_iter):
    # get max of sum(alpha_i) - 1/2(sum(alpha_i * alpha_j * y_i * y_j * <x_i,x_j>))
    # subject to -> alpha_i >= 0,sum(alpha_i * y_i) = 0
    # 简化版SMO算法实现，未使用启发式方法对alpha对进行选择.
    # :param C: 软间隔常数, 0 <= alpha_i <= C
    # :param max_iter: 外层循环最大迭代次数
    trainImages = np.array(trainImages)
    m, n = trainImages.shape
    trainLabels = np.array(trainLabels)
    # 初始化参数
    alphas = np.zeros(m)
    b = 0
    it = 0

    def f(x):
        # "SVM分类器函数 y = w^Tx + b"
        # Kernel function vector.
        x = x.reshape(-1, 1)
        data = np.matrix(trainImages)
        ks = data*x
        # Predictive value.
        wx = np.matrix(alphas * trainLabels) * ks
        fx = wx + b
        return fx[0, 0]

    while it < max_iter:
        pair_changed = 0
        for i in range(m):
            a_i, x_i, y_i = alphas[i], trainImages[i], trainLabels[i]
            fx_i = f(x_i)
            E_i = fx_i - y_i
            j = select_j(i, m)
            a_j, x_j, y_j = alphas[j], trainImages[j], trainLabels[j]
            fx_j = f(x_j)
            E_j = fx_j - y_j
            K_ii, K_jj, K_ij = np.dot(x_i, x_i), np.dot(x_j, x_j), np.dot(x_i, x_j)
            eta = K_ii + K_jj - 2*K_ij
            if eta <= 0:
                print('WARNING  eta <= 0')
                continue
            # 获取更新的alpha对
            a_i_old, a_j_old = a_i, a_j
            a_j_new = a_j_old + y_j*(E_i - E_j)/eta
            # 对alpha进行修剪
            if y_i != y_j:
                L = max(0, a_j_old - a_i_old)
                H = min(C, C + a_j_old - a_i_old)
            else:
                L = max(0, a_i_old + a_j_old - C)
                H = min(C, a_j_old + a_i_old)
            a_j_new = clip(a_j_new, L, H)
            a_i_new = a_i_old + y_i*y_j*(a_j_old - a_j_new)
            if abs(a_j_new - a_j_old) < 0.0000001:
                # print('WARNING   alpha_j not moving enough')
                continue
            alphas[i], alphas[j] = a_i_new, a_j_new
            # 更新阈值b
            b_i = -E_i - y_i*K_ii*(a_i_new - a_i_old) - y_j*K_ij*(a_j_new - a_j_old) + b
            b_j = -E_j - y_i*K_ij*(a_i_new - a_i_old) - y_j*K_jj*(a_j_new - a_j_old) + b
            if 0 < a_i_new < C:
                b = b_i
            elif 0 < a_j_new < C:
                b = b_j
            else:
                b = (b_i + b_j)/2
            pair_changed += 1
            # print('INFO   iteration:{}  i:{}  pair_changed:{}'.format(it, i, pair_changed))
        if pair_changed == 0:
            it += 1
        else:
            it = 0
        # print('iteration number: {}'.format(it))
    return alphas, b


def SVM():
    # 采用"一对其余"构造n个支持向量机
    TrainingImage, TrainingLabels, size = load_data('C:/Users/ZhouXunZhe/Desktop/train-images.idx3-ubyte',
                                                 'C:/Users/ZhouXunZhe/Desktop/train-labels.idx1-ubyte')
    TestImage, TestLabels, size = load_data('C:/Users/ZhouXunZhe/Desktop/t10k-images.idx3-ubyte',
                                                      'C:/Users/ZhouXunZhe/Desktop/t10k-labels.idx1-ubyte')
    errorCount = 0.0

    for i in range(size):
        if TrainingLabels[i] != 1.0:
            TrainingLabels[i] = -1
        if TestLabels[i] != 3.0:
            TestLabels[i] = -1
    dataset = TrainingImage[:1000]
    labels = TrainingLabels[:1000]
    # 使用简化版SMO算法优化SVM
    alphas, b = simple_smo(dataset, labels, 0.6, 40)

    w = get_w(alphas, dataset, labels)
    for i in range(size):
        # print("the NO.%d classifier came back with: %d, the real answer is: %d, error count is: %d" % (
        # i, result, TestLables[i], errorCount))
        test = TestImage[i].reshape(1, -1)
        result = test * w + b
        if result[0][0] * TestLabels[i] < 0:
            errorCount += + 1.0
    print("\nthe total number of errors is: %d" % errorCount)
    print("\nthe total error rate is: %f" % (errorCount / float(size)))

    # # 分类数据点
    # classified_pts = {'+1': [], '-1': []}
    # for point, label in zip(dataset, labels):
    #     if label == 1.0:
    #         classified_pts['+1'].append(point)
    #     else:
    #         classified_pts['-1'].append(point)
    # fig = plt.figure()
    # ax = fig.add_subplot(111)
    # # 绘制数据点
    # for label, pts in classified_pts.items():
    #     pts = np.array(pts)
    #     ax.scatter(pts[:, 0], pts[:, 1], label=label)
    # # 绘制分割线
    # w = get_w(alphas, dataset, labels)
    # x1 = max(dataset, key=lambda x: x[0])
    # x2 = min(dataset, key=lambda x: x[0])
    # a1, a2 = w
    # y1, y2 = (-b - np.array(a1).T * x1) / np.array(a2), (-b - np.array(a1).T * x2) / np.array(a2)
    # ax.plot([x1, x2], [y1, y2])
    # # 绘制支持向量
    # for i, alpha in enumerate(alphas):
    #     if abs(alpha) > 1e-3:
    #         x, y = dataset[i]
    #         ax.scatter([x], [y], s=150, c='none', alpha=0.7,
    #                    linewidth=1.5, edgecolor='#AB3319')
    # plt.show()


if __name__ == '__main__':
    SVM()