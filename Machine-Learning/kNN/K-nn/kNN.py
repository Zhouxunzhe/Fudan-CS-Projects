from numpy import *
import numpy as np
import operator
import Load
import cv2

def kNN(input, dataset, labels, k):
    dataset_size = dataset.shape[0]
    diff = tile(input, (dataset_size, 1)) - dataset
    sqdiff = diff ** 2
    sqdistance = sqdiff.sum(axis=1)
    distance = sqdistance ** 0.5
    sorteddist = distance.argsort()
    classCount = {}
    for i in range(k):
        nnlabel = labels[sorteddist[i]]
        classCount[nnlabel] = classCount.get(nnlabel, 0) + 1
    sortedClassCount = sorted(classCount.items(), key=operator.itemgetter(1), reverse=True)
    # print sortedClassCount
    return sortedClassCount[0][0]


def classify():
    TrainingImage, TrainingLabels, size = Load.load_data('C:/Users/ZhouXunZhe/Desktop/train-images.idx3-ubyte',
                                                 'C:/Users/ZhouXunZhe/Desktop/train-labels.idx1-ubyte')
    TestImage, TestLables, size = Load.load_data('C:/Users/ZhouXunZhe/Desktop/t10k-images.idx3-ubyte',
                                                      'C:/Users/ZhouXunZhe/Desktop/t10k-labels.idx1-ubyte')
    errorCount = 0.0
    for i in range(size):
        result = kNN(TestImage[i, :], TrainingImage, TrainingLabels, 3)
        print("the NO.%d classifier came back with: %d, the real answer is: %d, error count is: %d" % (
        i, result, TestLables[i], errorCount))
        if (result != TestLables[i]): errorCount += + 1.0
    print("\nthe total number of errors is: %d" % errorCount)
    print("\nthe total error rate is: %f" % (errorCount / float(size)))