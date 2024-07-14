import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.svm import SVR
from sklearn.metrics import mean_squared_error
plt.style.use('ggplot')


names = ['CRIM', 'ZN', 'INDUS', 'CHAS', 'NOX', 'RM', 'AGE', 'DIS', 'RAD', 'TAX', 'PIRATIO', 'B', 'LSTAT', 'MEDV']
data = pd.read_csv("C:/Users/ZhouXunZhe/Desktop/housing.csv", names=names, delim_whitespace=True)

x, y = data[data.columns.delete(-1)], data['MEDV']
x_train, x_test, y_train, y_test = train_test_split(x, y, test_size=0.2, random_state=888)

ss_x = StandardScaler()
x_train = ss_x.fit_transform(x_train)
x_test = ss_x.transform(x_test)
ss_y = StandardScaler()
y_train = ss_y.fit_transform(y_train.values.reshape(-1, 1))
y_test = ss_y.transform(y_test.values.reshape(-1, 1))

linear_svr_model = SVR(kernel="rbf", C=100, gamma=0.02)
linear_svr_model.fit(x_train, y_train.ravel())
linear_svr_pre = linear_svr_model.predict(x_test)
print('svm_rbf:')
print('SCORE:{:.4f}'.format(linear_svr_model.score(x_test, y_test)))
print('RMSE:{:.4f}'.format(np.sqrt(mean_squared_error(y_test, linear_svr_pre))))
# 处理之后的数据进行可视化
plt.figure()
plt.plot(x_train.min(axis=0), 'v', label='train set min')
plt.plot(x_train.max(axis=0), '^', label='train set max')
plt.plot(x_test.min(axis=0), 'v', label='test set min')
plt.plot(x_test.max(axis=0), '^', label='test set max')
plt.yscale('log')
# 设置图标注位置，为最佳
plt.legend(loc='best')

plt.figure()
plt.scatter(y_test, linear_svr_pre, label='y')
plt.plot([y_test.min(), y_test.max()], [y_test.min(), y_test.max()], 'k--', lw=4, label='predicted')
# 设置坐标轴标题
plt.xlabel('scaled features')
plt.ylabel('scaled features distribute')

plt.show()
