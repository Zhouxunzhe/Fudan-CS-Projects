import struct
import array
import numpy as np
from numpy import tile
import operator


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


def classify(data, dataset, labels, k):
    dataset_size = dataset.shape[0]
    diff = tile(data, (dataset_size, 1)) - dataset
    # 欧几里得距离
    sqdiff = diff ** 2
    sqdistance = sqdiff.sum(axis=1)
    distance = sqdistance ** 0.5

    # 曼哈顿距离
    # absdiff = abs(diff)
    # distance = absdiff.sum(axis=1)
    sorteddist = distance.argsort()
    classCount = {}
    for i in range(k):
        targetlabel = labels[sorteddist[i]]
        classCount[targetlabel] = classCount.get(targetlabel, 0) + 1
    sortedClassCount = sorted(classCount.items(), key=operator.itemgetter(1), reverse=True)
    return sortedClassCount[0][0]


def kNN():
    TrainingImage, TrainingLabels, size = load_data('C:/Users/ZhouXunZhe/Desktop/train-images.idx3-ubyte',
                                                 'C:/Users/ZhouXunZhe/Desktop/train-labels.idx1-ubyte')
    TestImage, TestLables, size = load_data('C:/Users/ZhouXunZhe/Desktop/t10k-images.idx3-ubyte',
                                                      'C:/Users/ZhouXunZhe/Desktop/t10k-labels.idx1-ubyte')
    errorCount = 0.0
    for i in range(size):
        result = classify(TestImage[i, :], TrainingImage, TrainingLabels, 19)
        # print("the NO.%d classifier came back with: %d, the real answer is: %d, error count is: %d" % (
        # i, result, TestLables[i], errorCount))
        if (result != TestLables[i]): errorCount += + 1.0
    print("\nthe total number of errors is: %d" % errorCount)
    print("\nthe total error rate is: %f" % (errorCount / float(size)))


if __name__ == '__main__':
    kNN()