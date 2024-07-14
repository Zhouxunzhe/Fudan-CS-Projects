import numpy as np
import torch.optim as optim
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
epochs = 100
meta_epochs = 10
model_size = 5

if __name__ == '__main__':
    np.random.seed(0)
    accuracy_list = []
    error_count = 0.0
    models = [MyCNN(), MyCNN(), MyCNN(), MyCNN(), MyCNN()]
    criterion = nn.CrossEntropyLoss()
    # train basemodel
    for epoch in range(epochs):
        permutation = np.random.permutation(x_train.shape[0])
        shuffled_x_train = x_train[permutation]
        shuffled_y_train = y_train[permutation]
        for cnn in models:
            cnn.train()
            optimizer = optim.Adam(cnn.parameters(), lr=learning_rate, weight_decay=0.01)
            permutation = np.random.permutation(shuffled_x_train.shape[0])
            shuffled_x = shuffled_x_train[permutation]
            shuffled_y = shuffled_y_train[permutation]
            batches = int(len(shuffled_x) / batch_size) + 1
            for j in range(0, batches):
                x_ = shuffled_x[j * batch_size: (j + 1) * batch_size]
                y_ = shuffled_y[j * batch_size: (j + 1) * batch_size]
                y_hat = cnn(x_)
                optimizer.zero_grad()
                loss = criterion(y_hat, y_)
                loss.backward()
                optimizer.step()
    # train metamodel
    meta_model = MetaModel()
    meta_criterion = nn.CrossEntropyLoss()
    meta_optimizer = optim.SGD(meta_model.parameters(), lr=0.01, momentum=0.9)
    for epoch in range(meta_epochs):
        outputs = []
        permutation = np.random.permutation(x_train.shape[0])
        shuffled_x_train = x_train[permutation]
        shuffled_y_train = y_train[permutation]
        for cnn in models:
            cnn.eval()
            output = cnn(shuffled_x_train)
            outputs.append(output)
        stacked_outputs = torch.stack(outputs, dim=1)
        meta_optimizer.zero_grad()
        meta_output = meta_model(stacked_outputs)
        meta_loss = meta_criterion(meta_output, y_train)
        meta_loss.backward()
        meta_optimizer.step()
    # test
    with torch.no_grad():
        error_count = 0.0
        outputs = []
        for cnn in models:
            output = cnn(x_test)
            outputs.append(output)
        stacked_outputs = torch.stack(outputs, dim=1)
        y_hat = meta_model(stacked_outputs)
        for i in range(test_size):
            index = np.argmax(y_hat[i])
            if y_test[i][index] != 1:
                error_count += + 1.0
        accuracy = 1 - (error_count / test_size)
    for i in range(len(models)):
        torch.save(models[i], "stacking\\base_model%s.pth" % i)
    torch.save(meta_model, "stacking\\meta_model.pth")
    print('accuracy: ', accuracy)
