import numpy as np
import torch.optim as optim
from torch.optim.lr_scheduler import ReduceLROnPlateau
from sklearn.ensemble import RandomForestClassifier
from model import *
from load import load_data

file_path = 'train\\'
x, y = load_data(file_path)

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
    models = []
    output_list = []
    error_count = 0.0
    criterion = nn.CrossEntropyLoss()
    for i in range(model_size):
        models.append(MyCNN())

    for epoch in range(epochs):
        for cnn in models:
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
    # train RandomForest
    forest = RandomForestClassifier(n_estimators=10)
    for cnn in models:
        cnn.eval()
        with torch.no_grad():
            error_count = 0.0
            y_hat = cnn(x_train)
            output_list.append(np.array(y_hat))
    predictions = torch.tensor(np.array(output_list)).permute(1, 0, 2)
    predictions = predictions.reshape(len(x_train), -1)
    forest.fit(np.array(predictions), np.array(y_train))

    # test
    test_list = []
    for cnn in models:
        cnn.eval()
        with torch.no_grad():
            error_count = 0.0
            y_hat = cnn(x_test)
            test_list.append(np.array(y_hat))
    predictions = torch.tensor(np.array(test_list)).permute(1, 0, 2)
    predictions = predictions.reshape(len(x_test), -1)
    results = forest.predict(np.array(predictions))
    for i in range(len(results)):
        index = np.argmax(results[i])
        if y_test[i][index] != 1:
            error_count += + 1.0
    accuracy = 1 - (error_count / len(x_test))
    for i in range(len(models)):
        torch.save(models[i], "bagging\\cnn%s.pth" % i)
    torch.save(forest, "bagging\\forest.pth")
    print('accuracy: ', accuracy)
