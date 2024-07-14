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
    models = []
    for i in range(model_size):
        models.append(torch.load("stacking\\base_model%s.pth" % i))
    meta_model = torch.load("stacking\\meta_model.pth")
    with torch.no_grad():
        error_count = 0.0
        outputs = []
        for cnn in models:
            output = cnn(x)
            outputs.append(output)
        stacked_outputs = torch.stack(outputs, dim=1)
        y_hat = meta_model(stacked_outputs)
        for i in range(len(x)):
            index = np.argmax(y_hat[i])
            if y[i][index] != 1:
                error_count += + 1.0
        accuracy = 1 - (error_count / len(x))
    print('accuracy: ', accuracy)
