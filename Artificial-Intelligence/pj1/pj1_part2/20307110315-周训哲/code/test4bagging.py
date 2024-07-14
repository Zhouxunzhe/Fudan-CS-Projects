import numpy as np
import torch
from load import load_data

path = 'train\\'
x, y = load_data(path)

model_size = 5

if __name__ == '__main__':
    np.random.seed(0)
    error_count = 0.0
    output_list = []
    for i in range(model_size):
        cnn = torch.load("bagging10000\\model%s.pth" % i)
        cnn.eval()
        with torch.no_grad():
            error_count = 0.0
            y_hat = cnn(x)
            output_list.append(y_hat)
    output_avg = torch.mean(torch.stack(output_list), dim=0)
    for i in range(len(output_avg)):
        index = np.argmax(output_avg[i])
        if y[i][index] != 1:
            error_count += + 1.0
    accuracy = 1 - (error_count / len(x))
    print('accuracy: ', accuracy)
