import numpy as np
import torch
from load import load_data

path = 'train\\'
x, y = load_data(path)

model_size = 5

if __name__ == '__main__':
    np.random.seed(0)
    error_count = 0.0
    boosting = torch.load("boosting.pth")
    boosting.eval()
    with torch.no_grad():
        y_hat = boosting(x, y)
        for i in range(len(y_hat)):
            index = np.argmax(y_hat[i])
            if y[i][index] != 1:
                error_count += + 1.0
        accuracy = 1 - (error_count / len(x))
    torch.save(boosting, "boosting.pth")
    print('accuracy: ', accuracy)
