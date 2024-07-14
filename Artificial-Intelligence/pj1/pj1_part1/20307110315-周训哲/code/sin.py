import numpy as np
import matplotlib.pyplot as plt
from model import BP

x = np.arange(-np.pi, np.pi, 0.01)
y = np.sin(x)
permutation = np.random.permutation(x.shape[0])
shuffled_x = x[permutation]
shuffled_y = y[permutation]
train_size = int(x.shape[0] / 10)
x_test = shuffled_x[0: train_size]
y_test = shuffled_y[0: train_size]
x_train = shuffled_x[train_size: x.shape[0]]
y_train = shuffled_y[train_size: x.shape[0]]

epochs = 100
batch_size = 80
batches = int(len(x) / batch_size) + 1
if __name__ == '__main__':
    np.random.seed(0)
    bp = BP(input_dim=1, hidden_layer=[100, 100], output_dim=1,
            batch_size=batch_size, batches=batches, learning_rate=0.001,
            task_type='regression', l2_lambda=0.0, dropout=0.7)
    bp.train('sin', x_train.reshape(-1, 1), y_train.reshape(-1, 1), epochs,
             x_test.reshape(-1, 1), y_test.reshape(-1, 1))
    y_hat = bp.predict(x_test.reshape(-1, 1))
    y_hat = y_hat.reshape(1, y_hat.shape[0])[0]
    loss = np.square(np.subtract(y_hat, y_test)).mean()
    print('mean square error: ', loss)

    plt.scatter(x_test, y_test, label='True')
    plt.scatter(x_test, y_hat, label='Predicted')
    plt.text(0.5, -0.3, 'MSE error = %f ' % loss, size=10, ha='center')
    plt.legend()
    plt.show()
