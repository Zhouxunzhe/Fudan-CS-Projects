import torch
import torch.nn as nn


class LeNet5(nn.Module):
    def __init__(self):
        super(LeNet5, self).__init__()
        # w' = (w + 2 * p - k) / s + 1
        # w: 28 (conv) 24 (pool) 12 (conv) 8 (pool) 4
        self.conv1 = nn.Conv2d(in_channels=1, out_channels=6, kernel_size=5, stride=1, padding=2)
        self.pool = nn.MaxPool2d(kernel_size=2, stride=2)
        self.conv2 = nn.Conv2d(in_channels=6, out_channels=16, kernel_size=5, stride=1, padding=0)
        self.fc1 = nn.Linear(in_features=16 * 5 * 5, out_features=120)
        self.fc2 = nn.Linear(in_features=120, out_features=84)
        self.fc3 = nn.Linear(in_features=84, out_features=12)

    def forward(self, x):
        # view(batch_size, in_channels, height, width)
        x = x.view(x.size(0), 1, x.size(1), x.size(2))
        x = self.conv1(x)
        x = nn.functional.relu(x)
        x = self.pool(x)
        x = self.conv2(x)
        x = nn.functional.relu(x)
        x = self.pool(x)
        # 将数据进行展平操作
        x = x.view(x.size(0), -1)
        x = self.fc1(x)
        x = nn.functional.relu(x)
        x = self.dropout(x)
        x = self.fc2(x)
        x = nn.functional.relu(x)
        x = self.dropout(x)
        x = self.fc3(x)
        return x


class MyCNN(nn.Module):
    def __init__(self):
        super(MyCNN, self).__init__()
        # 卷积核处理之后size不变
        self.conv1 = nn.Conv2d(in_channels=1, out_channels=16, kernel_size=3, stride=1, padding=1)
        self.bn1 = nn.BatchNorm2d(16)
        self.pool = nn.MaxPool2d(kernel_size=2, stride=2)
        self.conv2 = nn.Conv2d(in_channels=16, out_channels=32, kernel_size=3, stride=1, padding=1)
        self.bn2 = nn.BatchNorm2d(32)
        self.fc1 = nn.Linear(in_features=32 * 7 * 7, out_features=120)
        self.fc2 = nn.Linear(in_features=120, out_features=12)
        self.fc = nn.Linear(in_features=32 * 7 * 7, out_features=12)
        self.dropout = nn.Dropout(p=0.3)

    def forward(self, x):
        x = x.view(x.size(0), 1, x.size(1), x.size(2))
        x = self.conv1(x)
        x = self.bn1(x)
        x = nn.functional.relu(x)
        x = self.pool(x)
        x = self.conv2(x)
        x = self.bn2(x)
        x = nn.functional.relu(x)
        x = self.pool(x)
        x = x.view(-1, 32 * 7 * 7)
        # x = self.fc1(x)
        # x = nn.functional.relu(x)
        # x = self.dropout(x)
        # x = self.fc2(x)
        x = self.fc(x)
        return x


class CNNBoosting(nn.Module):
    def __init__(self, model_size):
        super(CNNBoosting, self).__init__()
        self.model_size = model_size
        self.models = nn.ModuleList([MyCNN(), MyCNN(), MyCNN(), MyCNN(), MyCNN()])
        self.alphas = nn.Parameter(torch.Tensor(model_size).fill_(1.0 / model_size))

    def forward(self, x, y_test):
        outputs = []
        alpha = self.alphas.clone().detach()
        for i in range(self.model_size):
            y_hat = self.models[i](x)
            alpha[i] = 1.0 / nn.CrossEntropyLoss()(y_hat, y_test)
            outputs.append(y_hat)
        alpha_sum = torch.sum(alpha)
        alpha /= alpha_sum
        self.alphas = nn.Parameter(alpha)
        outputs = torch.stack(outputs, dim=0)
        weights = nn.functional.softmax(self.alphas, dim=0)
        output = torch.sum(outputs * weights.unsqueeze(-1).unsqueeze(-1), dim=0)
        return output


class MetaModel(nn.Module):
    def __init__(self):
        super(MetaModel, self).__init__()
        self.fc1 = nn.Linear(in_features=5 * 12, out_features=64)
        self.fc2 = nn.Linear(in_features=64, out_features=12)

    def forward(self, x):
        x = x.view(x.size(0), 1, x.size(1), x.size(2))
        x = x.view(-1, 5 * 12)
        x = self.fc1(x)
        # x = nn.functional.relu(x)
        x = torch.tanh(x)
        x = self.fc2(x)
        return x
