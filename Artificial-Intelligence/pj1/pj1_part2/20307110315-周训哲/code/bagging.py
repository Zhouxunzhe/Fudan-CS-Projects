import numpy as np
import torch.optim as optim
from torchvision import transforms
from matplotlib import pyplot as plt
from torch.optim.lr_scheduler import ReduceLROnPlateau
from model import *
from load import load_data

path = 'train\\'
x, y = load_data(path)

# # 数据增强
# transform = transforms.Compose([
#     # 随机裁剪
#     transforms.RandomCrop(size=28),
#     # 水平翻转
#     transforms.RandomHorizontalFlip(),
#     # 随机旋转15°以内的角度
#     transforms.RandomRotation(15)
# ])
# x = transform(x)

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
epochs = 10
model_size = 5

if __name__ == '__main__':
    np.random.seed(0)
    models = []
    accuracy_list = []
    error_count = 0.0
    for i in range(model_size):
        cnn = MyCNN()
        criterion = nn.CrossEntropyLoss()
        # 正则化参数0.01
        optimizer = optim.Adam(cnn.parameters(), lr=learning_rate, weight_decay=0.01)
        scheduler = ReduceLROnPlateau(optimizer, mode='max', patience=3)
        # train
        acc_list = []
        permutation = np.random.permutation(x_train.shape[0])
        shuffled_x_train = x_train[permutation]
        shuffled_y_train = y_train[permutation]
        x_model = shuffled_x_train[0: int(2 / 3 * train_size)]
        y_model = shuffled_y_train[0: int(2 / 3 * train_size)]
        # x_model = x_train[i * int(train_size / model_size): (i + 1) * int(train_size / model_size)]
        # y_model = y_train[i * int(train_size / model_size): (i + 1) * int(train_size / model_size)]
        for epoch in range(epochs):
            cnn.train()
            permutation = np.random.permutation(x_model.shape[0])
            shuffled_x = x_model[permutation]
            shuffled_y = y_model[permutation]
            batches = int(len(shuffled_x) / batch_size) + 1
            for j in range(0, batches):
                x_ = shuffled_x[j * batch_size: (j + 1) * batch_size]
                y_ = shuffled_y[j * batch_size: (j + 1) * batch_size]
                y_hat = cnn(x_)
                optimizer.zero_grad()
                loss = criterion(y_hat, y_)
                loss.backward()
                optimizer.step()
            cnn.eval()
            with torch.no_grad():
                error_count = 0.0
                y_hat = cnn(x_test)
                for j in range(len(y_hat)):
                    index = np.argmax(y_hat[j])
                    if y_test[j][index] != 1:
                        error_count += + 1.0
                accuracy = 1 - (error_count / len(x_test))
                acc_list.append(accuracy)
        accuracy_list.append(acc_list)
        models.append(cnn)
    # test
    accuracy_list = np.array(accuracy_list)
    accuracy_list = accuracy_list.mean(axis=0)
    plt.figure()
    plt.scatter(range(epochs), accuracy_list, label='accuracy')
    plt.legend()
    plt.show()
    output_list = []
    for cnn in models:
        cnn.eval()
        with torch.no_grad():
            error_count = 0.0
            y_hat = cnn(x_test)
            output_list.append(y_hat)
    output_avg = torch.mean(torch.stack(output_list), dim=0)
    for i in range(len(output_avg)):
        index = np.argmax(output_avg[i])
        if y_test[i][index] != 1:
            error_count += + 1.0
    accuracy = 1 - (error_count / len(x_test))
    for i in range(len(models)):
        torch.save(models[i], "models\\model%s.pth" % i)
    print('accuracy: ', accuracy)
