import torch.optim as optim
import numpy as np
from torch.optim.lr_scheduler import ReduceLROnPlateau
from matplotlib import pyplot as plt
from model import *
from load import load_data

path = 'train\\'
x, y = load_data(path)

data_size = len(x)
test_size = int(data_size / 10)
train_size = data_size - test_size

permutation = np.random.permutation(x.shape[0])
shuffled_x = x[permutation]
shuffled_y = y[permutation]
x_test = shuffled_x[0: test_size]
y_test = shuffled_y[0: test_size]
x_train = shuffled_x[test_size: data_size]
y_train = shuffled_y[test_size: data_size]

learning_rate = 0.001
batch_size = 40
epochs = 1000
model_size = 5

if __name__ == '__main__':
    np.random.seed(0)
    accuracy_list = []
    error_count = 0.0
    boosting = CNNBoosting(model_size)
    criterion = nn.CrossEntropyLoss()
    optimizer = optim.Adam(boosting.parameters(), lr=learning_rate, weight_decay=0.01)
    scheduler = ReduceLROnPlateau(optimizer, mode='max', patience=3)
    # train
    for epoch in range(epochs):
        boosting.train()
        # 训练集不能改变
        y_hat = boosting(x_train, y_train)
        optimizer.zero_grad()
        loss = criterion(y_hat, y_train)
        loss.backward()
        optimizer.step()
        boosting.eval()
        with torch.no_grad():
            error_count = 0.0
            y_hat = boosting(x_test, y_test)
            for j in range(len(y_hat)):
                index = np.argmax(y_hat[j])
                if y_test[j][index] != 1:
                    error_count += + 1.0
            accuracy = 1 - (error_count / len(x_test))
            accuracy_list.append(accuracy)
    # test
    accuracy_list = np.array(accuracy_list)
    plt.figure()
    plt.scatter(range(epochs), accuracy_list, label='accuracy')
    plt.legend()
    plt.show()
    boosting.eval()
    with torch.no_grad():
        error_count = 0.0
        y_hat = boosting(x_test, y_test)
        for i in range(len(y_hat)):
            index = np.argmax(y_hat[i])
            if y_test[i][index] != 1:
                error_count += + 1.0
        accuracy = 1 - (error_count / len(x_test))
    torch.save(boosting, "boosting.pth")
    print('accuracy: ', accuracy)
