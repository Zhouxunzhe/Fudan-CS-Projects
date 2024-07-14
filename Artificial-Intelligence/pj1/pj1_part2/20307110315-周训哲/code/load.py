import numpy as np
import cv2
import os
import torch


def load_data(file_path):
    images = []
    labels = []
    for label in range(1, 13):
        folder = os.listdir(file_path + str(label))
        for file in folder:
            path = os.path.join(file_path + str(label), file)
            img = cv2.imread(path, cv2.IMREAD_GRAYSCALE)
            lbl = np.zeros(12)
            lbl[label-1] = 1
            images.append(img)
            labels.append(lbl)
    x = np.array(images)
    scale_factor = 1 / 255
    x = np.multiply(x, scale_factor)
    y = np.array(np.vstack(labels))
    x = torch.tensor(x).float()
    y = torch.tensor(y).float()

    return x, y
