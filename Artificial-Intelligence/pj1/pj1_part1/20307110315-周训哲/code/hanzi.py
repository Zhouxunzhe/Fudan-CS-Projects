import numpy as np
import cv2
import os
from model import BP

images = []
labels = []

for label in range(1, 13):
    folder = os.listdir('train\\' + str(label))
    for file in folder:
        path = os.path.join('train\\' + str(label), file)
        img = cv2.imread(path, cv2.IMREAD_GRAYSCALE).flatten()
        lbl = np.zeros(12)
        lbl[label-1] = 1
        images.append(img)
        labels.append(lbl)

x = np.array(np.vstack(images))
scale_factor = 1 / 255
x = np.multiply(x, scale_factor)
y = np.array(np.vstack(labels))
data_size = len(images)
train_size = int(data_size / 10)
epochs = 500  # 8000
batch_size = 40
batches = int(train_size / batch_size) + 1

permutation = np.random.permutation(x.shape[0])
shuffled_x = x[permutation]
shuffled_y = y[permutation]
x_test = shuffled_x[0: train_size]
y_test = shuffled_y[0: train_size]
x_train = shuffled_x[train_size: data_size]
y_train = shuffled_y[train_size: data_size]

if __name__ == '__main__':
    np.random.seed(0)
    bp = BP(input_dim=784, hidden_layer=[500, 300, 300, 100], output_dim=12,
            batch_size=batch_size, batches=batches, learning_rate=0.001,
            task_type='classification', l2_lambda=0.01, dropout=0.7)
    bp.train('hanzi', x_train, y_train, epochs, x_test, y_test)
    y_hat = bp.predict(x_test)
    error_count = 0.0
    for i in range(len(y_hat)):
        index = np.argmax(y_hat[i])
        if y_test[i][index] != 1:
            error_count += + 1.0
    accuracy = 1 - (error_count / len(y_hat))
    print('accuracy: ', accuracy)
