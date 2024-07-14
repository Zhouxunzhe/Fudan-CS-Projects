import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import tensorflow._api.v2.compat.v1 as tf
import warnings
tf.disable_v2_behavior()
tf.get_logger().setLevel('ERROR')
tf.get_logger().setLevel('WARNING')
warnings.filterwarnings("ignore")

# 设置
# 学习率
Learning_Rate = 1e-4
# 训练轮数
Training_Iterations = 2500
# dropout保留百分比
Dropout = 0.6
# 批处理数量
Batch_Size = 100
# 测试数据集数量
Test_Size = 10000


def convert(imgf, labelf, outf, n):
    # rb，以二进制只读方式从文件开头打开
    # w，从文件开头开始写入
    f = open(imgf, "rb")
    o = open(outf, "w")
    l = open(labelf, "rb")

    # 读入指定字节数
    f.read(16)
    l.read(8)

    # 创建一个列表
    images = []

    for i in range(n):
        # ord()返回字符对应的ASC码
        image = [ord(l.read(1))]  # 添加标签

        for j in range(28 * 28):
            image.append(ord(f.read(1)))  # 添加图像

        images.append(image)  # 存入列表

    # 写入输出文件
    # 写入列名，label,pixel0,pixel1...piexl783
    o.write("label,")
    pixel = []
    for p in range(28 * 28):
        name = "pixel" + str(p)
        pixel.append(name)

    o.write(",".join(pixel))
    o.write("\n")
    # 写入图像数据
    for image in images:
        o.write(",".join(str(pix) for pix in image) + "\n")
    f.close()
    o.close()
    l.close()


# 导入文件
try:
    data = pd.read_csv(r'C:\Users\ZhouXunZhe\Desktop\机器学习\MNIST\mnist_train.csv')
    print("data loaded!")
except:
    # 生成train.csv
    convert(r"C:\Users\ZhouXunZhe\Desktop\机器学习\MNIST\train-images.idx3-ubyte",
            r"C:\Users\ZhouXunZhe\Desktop\机器学习\MNIST\train-labels.idx1-ubyte",
            r"C:\Users\ZhouXunZhe\Desktop\机器学习\MNIST\mnist_train.csv", 60000)
    # 生成test.csv
    convert(r"C:\Users\ZhouXunZhe\Desktop\机器学习\MNIST\t10k-images.idx3-ubyte",
            r"C:\Users\ZhouXunZhe\Desktop\机器学习\MNIST\t10k-labels.idx1-ubyte",
            r"C:\Users\ZhouXunZhe\Desktop\机器学习\MNIST\mnist_test.csv", 10000)
    print("Convert Finished!")

    data = pd.read_csv(r'C:\Users\ZhouXunZhe\Desktop\机器学习\MNIST\mnist_train.csv')


# 图像的灰度值在0~255之间，差异性过大，先进行归一化处理
images = data.iloc[:,1:].values
images = images.astype(np.float)

images = np.multiply(images, 1.0 / 255.0)

# 此时图像是一个784的长条，需要转换成28x28的，先算出来高和宽的值
image_size = images.shape[1]
image_width = image_height = np.ceil(np.sqrt(image_size)).astype(np.uint8)


# 读取标签数据和类型数量
labels_flat = data['label'].values.ravel()
labels_count = np.unique(labels_flat).shape[0]


# 对标签进行one-hot coding
# 0 => [1,0,0,0,0,0,0,0,0,0]
# 1 => [0,1,0,0,0,0,0,0,0,0]
# ...
# 9 => [0,0,0,0,0,0,0,0,0,1]
def dense_to_one_hot(labels_dense, num_classes):
    num_labels = labels_dense.shape[0]  # 样本数量
    index_offset = np.arange(num_labels) * num_classes  # [0,10,20,...,599990]
    labels_one_hot = np.zeros((num_labels, num_classes))
    labels_one_hot.flat[index_offset + labels_dense.ravel()] = 1
    return labels_one_hot


labels = dense_to_one_hot(labels_flat, labels_count)
labels = labels.astype(np.uint8)

test_images = images[:Test_Size]  #前2000作为测试数据
test_labels = labels[:Test_Size]

train_images = images[Test_Size:]
train_labels = labels[Test_Size:]


# ------------------------------------------------------
# 权重与偏置
def weight_variable(shape):
    initial = tf.truncated_normal(shape, stddev=0.1)  # 高斯初始化
    return tf.Variable(initial)

def bias_variable(shape):
    initial = tf.constant(0.1, shape=shape)
    return tf.Variable(initial)


# 指定卷积操作
def conv2d(x, W):
    return tf.nn.conv2d(x, W, strides=[1, 1, 1, 1], padding='SAME')

# strides=[batch_size(在batchsize上是否有滑动), 图像的高, 图像的宽, 图像的通道(RGB)]

# 指定池化操作
def max_pool_2x2(x):
    return tf.nn.max_pool(x, ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1], padding='SAME')

# 指定输入输出
x = tf.placeholder('float', shape=[None, image_size])
y_ = tf.placeholder('float', shape=[None, labels_count])

# 指定神经网络模型
# 第一层
W_conv1 = weight_variable([5, 5, 1, 32])  # filter为5x5x1,32个
b_conv1 = bias_variable([32])

# (58000,784) => (58000,28,28,1)
image = tf.reshape(x, [-1, image_width, image_height, 1])
# -1表示未知数，自动求解

h_conv1 = tf.nn.relu(conv2d(image, W_conv1) + b_conv1) # 卷积层
# h_conv1.get_shape() = (58000, 28, 28, 32)
h_pool1 = max_pool_2x2(h_conv1)                        # 池化层
# h_pool1.get_shape() = (58000, 14, 14, 32)

# 第二层
W_conv2 = weight_variable([5, 5, 32, 64]) # 经过一次pooling，filter个数翻倍
b_conv2 = bias_variable([64])

h_conv2 = tf.nn.relu(conv2d(h_pool1, W_conv2) + b_conv2)
# h_conv2.get_shape() = (58000, 14, 14, 64)
h_pool2 = max_pool_2x2(h_conv2)
# h_pool2.get_shape() = (58000, 7, 7, 64)

# 指定全连接层
W_fc1 = weight_variable([7*7*64, 1024])
b_fc1 = bias_variable([1024])

# 将特征提取结果展开  (58000, 7, 7, 64) => (58000, 3136)
h_pool2_flat = tf.reshape(h_pool2, [-1, 7*7*64])

h_fc1 = tf.nn.relu(tf.matmul(h_pool2_flat, W_fc1) + b_fc1)
# h_fc1.get_shape() = (58000, 1024)

# 指定dropout, 一般只加在最后的几层全连接层
keep_prob = tf.placeholder('float')
h_fc1_drop = tf.nn.dropout(h_fc1, keep_prob)

# 得到结果
W_fc2 = weight_variable([1024, labels_count])
b_fc2 = bias_variable([labels_count])

y = tf.nn.softmax(tf.matmul(h_fc1_drop, W_fc2) + b_fc2)
# y.getshape() = (58000,10)

# 损失函数
cross_entropy = -tf.reduce_sum(y_*tf.log(y))

# 优化器
train_step = tf.train.GradientDescentOptimizer(Learning_Rate).minimize(cross_entropy)

# 评估
correct_prediction = tf.equal(tf.argmax(y,1), tf.argmax(y_,1))
accuracy = tf.reduce_mean(tf.cast(correct_prediction, 'float'))
predict = tf.argmax(y,1)

epochs_completed = 0
index_in_epoch = 0
num_examples = train_images.shape[0]


# 批读入数据，定好起始地址、终止地址然后拿数据就好
def next_batch(batch_size):
    global train_images
    global train_labels
    global index_in_epoch
    global epochs_completed

    start = index_in_epoch
    index_in_epoch += batch_size

    if index_in_epoch > num_examples:
        # 一轮结束
        epochs_completed += 1
        # 刷新数据集
        perm = np.arange(num_examples)
        np.random.shuffle(perm)
        train_images = train_images[perm]
        train_labels = train_labels[perm]
        # 开始下一轮
        start = 0
        index_in_epoch = batch_size
        assert batch_size <= num_examples
    end = index_in_epoch
    return train_images[start:end], train_labels[start:end]


# 初始化
init = tf.global_variables_initializer()
sess = tf.InteractiveSession()
sess.run(init)

# 开始训练
test_accuracies = []
x_range = []

display_step = 1  # 采用动态输出，每输出10次，step*10，见下面

for i in range(Training_Iterations):
    # 批读入数据
    batch_xs, batch_ys = next_batch(Batch_Size)
    test_accuracy = accuracy.eval(feed_dict={x: test_images[0:Batch_Size],
                                             y_: test_labels[0:Batch_Size],
                                             keep_prob: 1.0})
    # 展示
    if (i + 1) % display_step == 0 or (i + 1) == Training_Iterations:
        # 训练数据精度
        train_accuracy = accuracy.eval(feed_dict={x: batch_xs,
                                                  y_: batch_ys,
                                                  keep_prob: 1.0})
        # 测试数据精度
        if (Test_Size):
            # keep_prob 是 dropout 的设置
            print(' training accuracy / test accuracy = %.2f / %.2f for step %d' % (
            train_accuracy, test_accuracy, i + 1))
        else:
            print(' training accuracy = %.2f for step %d' % (train_accuracy, i + 1))

        # 展示间隔增加
        if (i + 1) % (display_step * 10) == 0 and i:
            display_step *= 10
    # 用于画图
    if (i + 1) % 10 == 0:
        x_range.append(i + 1)
        test_accuracies.append(test_accuracy)
    # 开始
    sess.run(train_step, feed_dict={x: batch_xs, y_: batch_ys, keep_prob: Dropout})

# 结果展示表
if(Test_Size):
    test_accuracy = accuracy.eval(feed_dict={x:test_images[0:Test_Size],
                                                             y_:test_labels[0:Test_Size],
                                                             keep_prob: 0.6})
    print(' test accuracy = %.4f' % test_accuracy)
    plt.plot(x_range, test_accuracies, '-g', label='Test')
    plt.legend(loc='lower right', frameon=False)
    plt.ylim(ymax = 1.1, ymin = 0.7)
    plt.ylabel('accuracy')
    plt.xlabel('step')
    plt.show()