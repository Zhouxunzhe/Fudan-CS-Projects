import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.svm import SVR
from sklearn.metrics import mean_squared_error
import warnings
warnings.filterwarnings("ignore")
plt.style.use('ggplot')


names = ['CRIM', 'ZN', 'INDUS', 'CHAS', 'NOX', 'RM', 'AGE', 'DIS', 'RAD', 'TAX', 'PIRATIO', 'B', 'LSTAT', 'MEDV']
data = pd.read_csv("C:/Users/ZhouXunZhe/Desktop/housing.csv", names=names, delim_whitespace=True)


x, y = data[data.columns.delete(-1)], data['MEDV']
x_train, x_test, y_train, y_test = train_test_split(x, y, test_size=0.2, random_state=888)

linear_svr_model = SVR(kernel="linear", C=100, gamma=0.02)
linear_svr_model.fit(x_train, y_train.ravel())
linear_svr_pre = linear_svr_model.predict(x_test)
print('svm_linear:')
print('SCORE:{:.4f}'.format(linear_svr_model.score(x_test, y_test)))
print('RMSE:{:.4f}'.format(np.sqrt(mean_squared_error(y_test, linear_svr_pre))))

plt.figure()
plt.title('linear')
plt.scatter(y_test, linear_svr_pre, label='y')
plt.plot([y_test.min(), y_test.max()], [y_test.min(), y_test.max()], 'k--', lw=4, label='predicted')
# 设置坐标轴标题
plt.xlabel('scaled features')
plt.ylabel('scaled features distribute')
plt.show()
