import matplotlib.pyplot as plt

x = []
y = []
count = 1
fp = open("type_count.txt", "r", encoding="utf-8")  # 展示type_count结果
for line in fp:
    a, b, c = line.rstrip().split()
    x.append(count)
    y.append(c)
    count += 1
plt.plot(x, y)
plt.show()
