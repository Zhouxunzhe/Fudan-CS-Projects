import torch
from torch import nn
import matplotlib.pyplot as plt
import torch.nn.functional as tf
from torch.utils.data import DataLoader
import torch.optim as optim
from torchvision import datasets, transforms

device = torch.device('cuda:0' if torch.cuda.is_available() else 'cpu')
print("cuda is available" if torch.cuda.is_available() else "cuda is not available")
transform = transforms.Compose([transforms.ToTensor(), transforms.Normalize(mean=[0.5],std=[0.5])])
train_data = datasets.MNIST(root="./data", transform=transform, train=True, download=True)
test_data = datasets.MNIST(root="./data", transform=transform,  train=False)
train_loader = DataLoader(train_data, batch_size=64, shuffle=True, num_workers=0)
test_loader = DataLoader(test_data, batch_size=64,  shuffle=True, num_workers=0)


# 定义网络
class CNN(nn.Module):
    def __init__(self):
        super(CNN, self).__init__()
        self.conv1 = nn.Conv2d(1, 32, kernel_size=5, stride=1, padding='same')
        self.pool = nn.MaxPool2d(2, 2)
        self.conv2 = nn.Conv2d(32, 64, kernel_size=5, stride=1, padding='same')
        self.pool = nn.MaxPool2d(2, 2)
        self.fc1 = nn.Linear(64*7*7, 1024)  # 两个池化，所以是7*7而不是14*14
        self.fc2 = nn.Linear(1024, 10)
        # self.fc3 = nn.Linear(512, 10)
#         self.dp = nn.Dropout(p=0.5)


    def forward(self, x):
        x = self.pool(tf.relu(self.conv1(x)))
        x = self.pool(tf.relu(self.conv2(x)))

        x = x.view(-1, 64 * 7* 7)  # 将数据平整为一维的
        x = tf.relu(self.fc1(x))
#         x = self.fc3(x)
#         self.dp(x)
#         x = tf.relu(self.fc2(x))
        x = self.fc2(x)
#         x = tf.log_softmax(x,dim=1) NLLLoss()才需要，交叉熵不需要
        return x


net = CNN().to(device)

# 损失函数
criterion = nn.CrossEntropyLoss()
optimizer = optim.SGD(net.parameters(), lr=0.001, momentum=0.9)

# 模型训练
train_accs = []
train_loss = []
test_accs = []
device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
net = net.to(device)
for epoch in range(3):
    running_loss = 0.0
    for i, data in enumerate(train_loader, 0):  # 0是下标起始位置默认为0
        # data 的格式[[inputs, labels]]
        # inputs,labels = data
        inputs, labels = data[0].to(device), data[1].to(device)
        # 初始为0，清除上个batch的梯度信息
        optimizer.zero_grad()

        # 前向+后向+优化
        outputs = net(inputs)
        loss = criterion(outputs, labels)
        loss.backward()
        optimizer.step()

        # 训练曲线的绘制 一个batch中的准确率
        _, predicted = torch.max(outputs.data, 1)
        total = labels.size(0)  # labels 的长度
        correct = (predicted == labels).sum().item()  # 预测正确的数目
        running_acc = 100 * correct / total
        train_accs.append(running_acc)

        # loss 的输出，每个一百个batch输出，平均的loss
        running_loss += loss.item()
        if i % 100 == 99:
            print('[%d,%5d] loss :%.3f , accuracy :%.4f' % (epoch+1, i+1, running_loss/100, running_acc))
            running_loss = 0.0
        train_loss.append(loss.item())


# 模型评估
def draw_train_process(title, iters, costs, accs, label_cost, lable_acc):
    plt.title(title, fontsize=24)
    plt.xlabel("iter", fontsize=20)
    plt.ylabel("acc(\%)", fontsize=20)
    plt.plot(iters, costs, color='red', label=label_cost)
    plt.plot(iters, accs, color='green', label=lable_acc)
    plt.legend()
    plt.grid()
    plt.show()


train_iters = range(len(train_accs))
draw_train_process('Training', train_iters, train_loss, train_accs, 'training loss', 'training acc')