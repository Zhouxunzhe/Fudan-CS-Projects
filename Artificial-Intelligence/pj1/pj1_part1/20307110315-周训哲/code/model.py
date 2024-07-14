import numpy as np
from matplotlib import pyplot as plt


class sigmoid:
    def forward(self, x):
        return 1 / (1 + np.exp(-x))
    def backward(self, x):
        return x * (1 - x)
class tanh:
    def forward(self, x):
        return np.tanh(x)
    def backward(self, x):
        return 1 - np.tanh(x) ** 2
class relu:  # leaky relu
    def forward(self, x):
        return np.maximum(0.01 * x, x)
    def backward(self, x):
        return np.where(x > 0, 1, 0.01)
class softmax:
    def forward(self, x):
        exp_x = np.exp(x - np.max(x, axis=1, keepdims=True))
        sum_exp_x = np.sum(exp_x, axis=1, keepdims=True)
        return exp_x / sum_exp_x
    def backward(self, x):
        return 1
class none_function:
    def forward(self, x):
        return x
    def backward(self, x):
        return 1
class loss:
    def mean_square(self, y, y_hat):
        return y_hat - y
    def corss_entropy(self, y, y_hat):
        return (softmax.forward(self, y_hat) - y) / y.shape[0]


class BP:
    def __init__(self, input_dim, hidden_layer, output_dim, batch_size, batches,
                 learning_rate, task_type, l2_lambda, dropout):
        if task_type not in ('regression', 'classification'):
            raise ValueError('Now only \'regression\' model and \'classification\' model are supported!')
        self.input_dim = input_dim
        self.hidden_layer = hidden_layer
        self.output_dim = output_dim
        self.batch_size = batch_size
        self.batches = batches
        self.learning_rate = learning_rate
        self.shrink_lr = False
        self.l2_lambda = l2_lambda
        self.dropout = dropout
        self.activation_function = sigmoid
        if task_type == 'regression':
            self.classification_function = none_function
        elif task_type == 'classification':
            self.classification_function = softmax
        self.loss_function = loss
        self.w = {}
        self.b = {}
        self.layers = len(hidden_layer)
        self.w[0] = np.random.randn(input_dim, hidden_layer[0])
        for i in range(1, self.layers):
            self.w[i] = np.random.randn(hidden_layer[i-1], hidden_layer[i])
        self.w[self.layers] = np.random.randn(hidden_layer[self.layers-1], output_dim)
        for i in range(self.layers):
            self.b[i] = np.zeros((1, hidden_layer[i]))
        self.b[self.layers] = np.zeros((1, output_dim))

    def forward(self, x):
        self.z = {}
        self.a = {}
        self.z[0] = np.dot(x, self.w[0]) + self.b[0]
        self.a[0] = self.activation_function.forward(self, self.z[0])
        for i in range(1, self.layers):
            self.z[i] = np.dot(self.a[i-1], self.w[i]) + self.b[i]
            self.a[i] = self.activation_function.forward(self, self.z[i])
        self.z[self.layers] = np.dot(self.a[self.layers-1], self.w[self.layers]) + self.b[self.layers]
        self.a[self.layers] = self.classification_function.forward(self, self.z[self.layers])
        return self.a[self.layers]

    def backward(self, x, y, y_hat):
        delta = {}
        dw = {}
        db = {}
        delta[0] = loss.mean_square(self, y, y_hat) * self.classification_function.backward(self, y_hat)
        dw[0] = np.dot(self.a[self.layers - 1].T, delta[0])
        db[0] = np.sum(delta[0], axis=0, keepdims=True)
        for i in range(self.layers-1, 0, -1):
            delta[self.layers-i] = np.dot(delta[self.layers-i-1], self.w[i+1].T) * \
                                   self.activation_function.backward(self, self.a[i])
            dw[self.layers-i] = np.dot(self.a[i-1].T, delta[self.layers-i])
            db[self.layers-i] = np.sum(delta[self.layers-i], axis=0, keepdims=True)
        delta[self.layers] = np.dot(delta[self.layers-1], self.w[1].T) * \
                             self.activation_function.backward(self, self.a[0])
        dw[self.layers] = np.dot(x.T, delta[self.layers])
        db[self.layers] = np.sum(delta[self.layers], axis=0)
        # # 缩小学习率
        # if (dw[0].max() < 1.0 or dw[0].min() > -1.0) and not self.shrink_lr:
        #     self.shrink_lr = True
        #     self.learning_rate /= 10
        # 正则化
        for i in range(self.layers+1):
            dw[i] += self.l2_lambda * self.w[self.layers - i]
            self.w[i] -= self.learning_rate * dw[self.layers-i]
            self.b[i] -= self.learning_rate * db[self.layers-i]

    def train(self, type, x, y, epochs, x_test=None, y_test=None):
        accuracy_list = []
        loss_list = []
        for i in range(epochs):
            if i % 1000 == 0:
                print('epoch: ', i)
            permutation = np.random.permutation(x.shape[0])
            shuffled_x = x[permutation]
            shuffled_y = y[permutation]
            for j in range(0, self.batches):
                x_train = shuffled_x[j * self.batch_size: (j + 1) * self.batch_size]
                y_train = shuffled_y[j * self.batch_size: (j + 1) * self.batch_size]
                y_hat = self.forward(x_train)
                self.backward(x_train, y_train, y_hat)
            # 验证测试集准确率
            if type == 'sin':
                y_hat = self.predict(x_test.reshape(-1, 1))
                test_loss = np.square(np.subtract(y_hat, y_test)).mean()
                loss_list.append(test_loss)
            if type == 'hanzi':
                y_valid = self.predict(x_test)
                error_count = 0.0
                for k in range(len(y_valid)):
                    index = np.argmax(y_valid[k])
                    if y_test[k][index] != 1:
                        error_count += + 1.0
                accuracy = 1 - (error_count / len(y_valid))
                accuracy_list.append(accuracy)
        if type == 'sin':
            plt.figure()
            plt.scatter(range(epochs), loss_list, label='loss')
            plt.legend()
            plt.show()
        if type == 'hanzi':
            plt.figure()
            plt.scatter(range(epochs), accuracy_list, label='accuracy')
            plt.legend()
            plt.show()


    def predict(self, x):
        y_hat = self.forward(x)
        return y_hat
