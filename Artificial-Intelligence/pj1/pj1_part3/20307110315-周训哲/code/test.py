import torch
import torchvision.transforms as transforms
import torchvision.datasets as datasets

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

model = torch.load("model.pth")

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
