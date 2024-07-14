import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.svm import SVR
from sklearn.metrics import mean_squared_error
from sklearn.decomposition import PCA
plt.style.use('ggplot')


names = ['CRIM', 'ZN', 'INDUS', 'CHAS', 'NOX', 'RM', 'AGE', 'DIS', 'RAD', 'TAX', 'PIRATIO', 'B', 'LSTAT', 'MEDV']
data = pd.read_csv("C:/Users/ZhouXunZhe/Desktop/housing.csv", names=names, delim_whitespace=True)


# 正则化处理
def rescale(x):
    mean = x.mean()
    std = x.std()
    scaled_x = [(i - mean) / std for i in x]
    return pd.Series(scaled_x)


data_std = pd.DataFrame(columns=data.columns)
for i in data.columns:
    data_std[i] = rescale(data[i])

# PCA降维处理
pca = PCA(n_components=13)
x = data_std.drop('MEDV', axis=1)
x_pca = pca.fit_transform(x)
df_std_pca = pd.DataFrame(x_pca, columns=['PCA1','PCA2','PCA3','PCA4','PCA5','PCA6','PCA7','PCA8','PCA9','PCA10','PCA11','PCA12','PCA13'])
df_std_pca['MEDV'] = data_std['MEDV']


x = df_std_pca.drop('MEDV', axis=1)
y = df_std_pca['MEDV']
x_train, x_test, y_train, y_test = train_test_split(x, y, test_size=0.2)

linear_svr_model = SVR(kernel="poly", C=100, gamma=0.02)
linear_svr_model.fit(x_train, y_train.ravel())
linear_svr_pre = linear_svr_model.predict(x_test)
print('svm_poly:')
print('SCORE:{:.4f}'.format(linear_svr_model.score(x_test, y_test)))
print('RMSE:{:.4f}'.format(np.sqrt(mean_squared_error(y_test, linear_svr_pre))))

plt.figure()
plt.title('poly_pca')
plt.scatter(y_test, linear_svr_pre, label='y')
plt.plot([y_test.min(), y_test.max()], [y_test.min(), y_test.max()], 'k--', lw=4, label='predicted')
# 设置坐标轴标题
plt.xlabel('scaled features')
plt.ylabel('scaled features distribute')
plt.show()
