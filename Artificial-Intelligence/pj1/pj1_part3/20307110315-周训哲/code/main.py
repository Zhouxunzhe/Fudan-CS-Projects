import torch
import numpy as np
import torch.optim as optim
import torchvision.transforms as transforms
import torchvision.datasets as datasets
from matplotlib import pyplot as plt
from model import *

device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
print("Using device:", device)

# load MNIST
transform = transforms.Compose([
    transforms.ToTensor(),
    transforms.Normalize((0.1307,), (0.3081,))
])
train_set = datasets.MNIST(root='./data', train=True, download=True, transform=transform)
test_set = datasets.MNIST(root='./data', train=False, download=True, transform=transform)
# 训练集与测试集都是批处理，增加运算速度
train_data = torch.utils.data.DataLoader(train_set, batch_size=32, shuffle=True)
test_data = torch.utils.data.DataLoader(test_set, batch_size=32, shuffle=False)

epochs = 1
model = ResNet18().to(device)
criterion = nn.CrossEntropyLoss()
optimizer = optim.Adam(model.parameters())

# train
loss_list = []
for epoch in range(epochs):
    running_loss = 0.0
    data_loss = []
    for i, (inputs, labels) in enumerate(train_data, 0):
        inputs, labels = inputs.to(device), labels.to(device)
        optimizer.zero_grad()
        outputs = model(inputs)
        loss = criterion(outputs, labels)
        loss.backward()
        optimizer.step()

        data_loss.append(loss.item())
        running_loss += loss.item()
        if i % 100 == 99:
            print('[%d, %5d] loss: %f' % (epoch + 1, i + 1, running_loss / 100))
            running_loss = 0.0
    loss_list.append(data_loss)

loss_list = np.array(loss_list)
loss_list = loss_list.mean(axis=1)
plt.figure()
plt.scatter(range(epochs), loss_list, label='loss')
plt.legend()
plt.show()

# test
correct = 0
total = 0
with torch.no_grad():
    for data in test_data:
        images, labels = data
        images, labels = images.to(device), labels.to(device)
        outputs = model(images)
        _, predicted = torch.max(outputs.data, 1)
        total += labels.size(0)
        correct += (predicted == labels).sum().item()

print('Accuracy: ', correct / total)
# torch.save(model, "model.pth")
