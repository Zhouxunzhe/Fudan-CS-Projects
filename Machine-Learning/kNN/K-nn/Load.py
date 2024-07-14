import numpy as np
import struct
import array

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
            if (i + 1 == size):
                print("%d numbers imported" % (i+1))
            images[i, :] = image_data[i * rows * cols: (i + 1) * rows * cols]
    return images, labels, size