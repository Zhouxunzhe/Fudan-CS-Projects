from function import *
import numpy as np
from matplotlib import pyplot as plt
import pandas as pd

from statsmodels.graphics.tsaplots import plot_acf
import statsmodels.api as sm
from statsmodels.tsa import stattools
import warnings
warnings.filterwarnings("ignore")


def price_arima():
    size = 1000
    pred_size = 200
    kline_data = np.array(get_klines_data(size)).astype('float64')
    data = np.delete(kline_data, [0, 6, 11], axis=1)
    train_data = data[:size-pred_size, :]
    test_data = data[size-pred_size:, :]

    closing_price = train_data[:, 3]
    test_ts = pd.DataFrame(test_data[:, 3])

    ts = pd.DataFrame(closing_price)
    # plot_acf(ts, use_vlines=True, lags=30)

    # 不进行差分
    # D_ts = ts
    # 进行一阶差分
    # D_ts = ts.diff().dropna()
    # 进行二阶差分
    D_ts = ts.diff().diff().dropna()
    # 绘制差分后时序图
    #     # D_ts.plot()

    # plot_acf(D_ts, use_vlines=True, lags=30)
    print('原始序列的ADF检验结果为：', stattools.adfuller(ts))
    print('一阶差分序列的ADF检验结果为：', stattools.adfuller(D_ts))

    LjungBox = stattools.q_stat(stattools.acf(ts)[1:12], len(ts))
    print('原始序列白噪声检验结果为：', LjungBox[1][-1])
    LjungBox = stattools.q_stat(stattools.acf(D_ts)[1:12], len(D_ts))
    print('一阶差分序列白噪声检验结果为：', LjungBox[1][-1])

    model = sm.tsa.ARIMA(ts, order=(2, 1, 2))
    result = model.fit()
    result.summary()

    predict = pd.DataFrame(result.forecast(pred_size))
    predict = predict.reset_index(drop=True)

















    kline_data = np.array(get_klines_data(size)).astype('float64')
    data = np.delete(kline_data, [0, 6, 11], axis=1)
    scaler = MinMaxScaler(feature_range=(0, 1))
    scaler_data = scaler.fit_transform(data)
    train_data = scaler_data[:size - pred_size - 32, :]
    test_data = scaler_data[size - pred_size - 32:, :]

    look_back = 30
    trainX, trainY = create_dataset(train_data, look_back)
    testX, testY = create_dataset(test_data, look_back)

    model = Sequential()
    model.add(LSTM(64, return_sequences=True, input_shape=(look_back, train_data.shape[1])))
    model.add(LSTM(64, return_sequences=True))
    model.add(LSTM(32))
    model.add(Dropout(0.1))
    model.add(Dense(10))
    model.add(Dense(1))
    model.compile(optimizer=optimizers.Adam(), loss='mae', metrics=['accuracy'])
    learning_rate_reduction = callbacks.ReduceLROnPlateau(monitor='val_loss', patience=3,
                                                          factor=0.7, min_lr=0.000000005)

    history = model.fit(trainX, trainY,
                        batch_size=20,
                        epochs=30,
                        validation_data=(testX, testY),
                        callbacks=[learning_rate_reduction])

    closing_price = model.predict(testX)
    model.evaluate(testX)
    scaler.fit_transform(data[size - pred_size - 32:, 3].reshape(-1, 1))
    closing_price = scaler.inverse_transform(closing_price.reshape(1, -1))
    predictY = scaler.inverse_transform(testY.reshape(1, -1))

    rms = np.sqrt(np.mean(np.power((predictY - closing_price), 2)))
    print(rms)
    plt.figure(figsize=(16, 8))
    dict_data = {
        'True': predictY[0][:-1],
        'Predict': closing_price[0][1:]
    }
    data_pd = pd.DataFrame(dict_data)
    plt.plot(data_pd[['True']], linewidth=3, alpha=0.8, label='True')
    plt.plot(data_pd[['Predict']], linewidth=1.2, label='Predict')
    plt.legend(loc='upper right')
    plt.show()





from function import *
from sklearn.preprocessing import MinMaxScaler
from keras import optimizers, callbacks
from keras.models import Sequential
from keras.layers import Dense, LSTM, Dropout
from matplotlib import pyplot as plt


def create_dataset(dataset, look_back):
    X, Y = [], []
    for i in range(len(dataset) - look_back):
        X.append(dataset[i:(i + look_back), :])
        Y.append(dataset[i + look_back, 3])
    return np.array(X), np.array(Y)


def price_lstm():
    size = 1500
    pred_size = 100
    kline_data = np.array(get_klines_data(size)).astype('float64')
    data = np.delete(kline_data, [0, 6, 11], axis=1)
    scaler = MinMaxScaler(feature_range=(0, 1))
    scaler_data = scaler.fit_transform(data)
    train_data = scaler_data[:size - pred_size - 32, :]
    test_data = scaler_data[size - pred_size - 32:, :]

    look_back = 1
    trainX, trainY = create_dataset(train_data, look_back)
    testX, testY = create_dataset(test_data, look_back)

    model = Sequential()
    model.add(LSTM(64, return_sequences=True, input_shape=(look_back, train_data.shape[1])))
    model.add(LSTM(64, return_sequences=True))
    model.add(LSTM(32))
    model.add(Dropout(0.1))
    model.add(Dense(10))
    model.add(Dense(1))
    model.compile(optimizer=optimizers.Adam(), loss='mae', metrics=['accuracy'])
    learning_rate_reduction = callbacks.ReduceLROnPlateau(monitor='val_loss', patience=3,
                                                          factor=0.7, min_lr=0.000000005)

    history = model.fit(trainX, trainY,
                        batch_size=20,
                        epochs=70,
                        validation_data=(testX, testY),
                        callbacks=[learning_rate_reduction])
    plt.plot(history.history['loss'], label='train loss')
    plt.show()

    closing_price = model.predict(testX)
    model.evaluate(testX)
    scaler.fit_transform(data[size - pred_size - 32:, 3].reshape(-1, 1))
    closing_price = scaler.inverse_transform(closing_price.reshape(1, -1))
    predictY = scaler.inverse_transform(testY.reshape(1, -1))

    rms = np.sqrt(np.mean(np.power((predictY - closing_price), 2)))
    print(rms)
    plt.figure(figsize=(16, 8))
    dict_data = {
        'True': predictY[0][:-1],
        'Predict': closing_price[0][1:]
    }
    data_pd = pd.DataFrame(dict_data)
    plt.plot(data_pd[['True']], linewidth=3, alpha=0.8, label='True')
    plt.plot(data_pd[['Predict']], linewidth=1.2, label='Predict')
    plt.legend(loc='upper right')
    plt.show()

