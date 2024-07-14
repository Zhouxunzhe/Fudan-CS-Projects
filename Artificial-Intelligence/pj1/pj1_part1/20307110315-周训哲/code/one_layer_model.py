import numpy as np
import matplotlib.pyplot as plt

x = np.arange(-np.pi, np.pi, 0.01)
y = np.sin(x)


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


class BP:
    def __init__(self, input_dim, hidden_dim, output_dim, batch_size, batches):
        self.input_dim = input_dim
        self.hidden_dim = hidden_dim
        self.output_dim = output_dim
        self.batch_size = batch_size
        self.batches = batches
        self.activation_function = relu
        self.w1 = np.random.randn(input_dim, hidden_dim)
        self.w2 = np.random.randn(hidden_dim, output_dim)
        self.b1 = np.zeros((1, hidden_dim))
        self.b2 = np.zeros((1, output_dim))
        self.learning_rate = 0.01

    def forward(self, x):
        self.z1 = np.dot(x, self.w1) + self.b1
        self.x1 = self.activation_function.forward(self, self.z1)
        self.z2 = np.dot(self.x1, self.w2) + self.b2
        self.x2 = self.activation_function.forward(self, self.z2)
        return self.x2

    def backward(self, x, y):
        delta2 = (self.x2 - y) * self.activation_function.backward(self, self.x2)
        dw2 = np.dot(self.x1.T, delta2)
        db2 = np.sum(delta2, axis=0, keepdims=True)
        delta1 = np.dot(delta2, self.w2.T) * self.activation_function.backward(self, self.x1)
        dw1 = np.dot(x.T, delta1)
        db1 = np.sum(delta1, axis=0)
        self.w1 -= self.learning_rate * dw1
        self.b1 -= self.learning_rate * db1
        self.w2 -= self.learning_rate * dw2
        self.b2 -= self.learning_rate * db2

    def train(self, x, y, epochs):
        for i in range(epochs):
            permutation = np.random.permutation(x.shape[0])
            shuffled_x = x[permutation]
            shuffled_y = y[permutation]
            for j in range(0, self.batches):
                x_train = shuffled_x[j * self.batch_size: (j + 1) * self.batch_size]
                y_train = shuffled_y[j * self.batch_size: (j + 1) * self.batch_size]
                self.forward(x_train)
                self.backward(x_train, y_train)

    def predict(self, x):
        y_hat = self.forward(x)
        return y_hat


epochs = 10000
batches = 100
if __name__ == '__main__':
    np.random.seed(0)
    bp = BP(input_dim=1, hidden_dim=200, output_dim=1, batch_size=100, batches=batches)
    bp.train(x.reshape(-1, 1), y.reshape(-1, 1), epochs)
    y_hat = bp.predict(x.reshape(-1, 1))
    y_hat = y_hat.reshape(1, y_hat.shape[0])[0]
    loss = np.square(np.subtract(y_hat, y)).mean()
    print('mean square error: ', loss)

    plt.plot(x, y, label='True')
    plt.plot(x, y_hat, label='Predicted')
    plt.text(0.5, -0.3, 'MSE error = %f ' % loss, size=10, ha='center')
    plt.legend()
    plt.show()