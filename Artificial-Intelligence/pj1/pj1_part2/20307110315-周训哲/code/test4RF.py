import numpy as np
import torch
from load import load_data

path = 'D:\\test_data\\'
x, y = load_data(path)

model_size = 5

if __name__ == '__main__':
    np.random.seed(0)
    error_count = 0.0
    test_list = []
    forest = torch.load("bagging\\forest.pth")
    for i in range(model_size):
        cnn = torch.load("bagging\\cnn%s.pth" % i)
        cnn.eval()
        with torch.no_grad():
            error_count = 0.0
            y_hat = cnn(x)
            test_list.append(np.array(y_hat))
    predictions = torch.tensor(np.array(test_list)).permute(1, 0, 2)
    predictions = predictions.reshape(len(x), -1)
    results = forest.predict(np.array(predictions))
    for i in range(len(results)):
        index = np.argmax(results[i])
        if y[i][index] != 1:
            error_count += + 1.0
    accuracy = 1 - (error_count / len(x))
    print('accuracy: ', accuracy)
