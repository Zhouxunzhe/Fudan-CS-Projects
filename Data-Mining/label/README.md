# text-cluster
文本聚类
## K-means
K-means算法是输入聚类个数k，以及包含 n个数据对象的数据库，输出满足方差最小标准k个聚类的一种算法。

**基本流程：**
- 步骤1：
从n个数据对象任意选择 k 个对象作为初始聚类中心；而对于所剩下其它对象，则根据它们与这些聚类中心的相似度（距离），分别将它们分配给与其最相似的
（聚类中心所代表的）聚类；
- 步骤2：
再计算每个所获新聚类的聚类中心（该聚类中所有对象的均值）；
- 步骤3：
不断重复这一过程直到标准测度函数开始收敛为止。

一般都采用均方差作为标准测度函数. k个聚类具有以下特点：各聚类本身尽可能的紧凑，而各聚类之间尽可能的分开。

算法的时间复杂度上界为O(n\*k\*t), 其中t是迭代次数。

k-means算法是一种基于样本间相似性度量的间接聚类方法，属于非监督学习方法。此算法以k为参数，把n 个对象分为k个簇，以使簇内具有较高的相似度，而且簇间的相似度较低。

## 中心点的个数从3到80对应的误差值的曲线
![](https://github.com/yanqiangmiffy/text-cluster/blob/master/assets/result.png)

## 10个簇的数据集的个数
```text
Cluster distribution:
{3: 133, 9: 93, 1: 131, 4: 497, 5: 328, 6: 263, 8: 78, 2: 46, 0: 164, 7: 67}
```
## 10个类的tfidf标签结果
```text
Top terms per cluster:
Cluster 0 工作 人类 时间 学习 事情 员工 研究 生活 团队 能力
Cluster 1 区块 货币 比特 加密 星球 数字 交易所 交易 项目 技术
Cluster 2 公寓 房租 长租 租金 租赁 租房 房源 上涨 北京 资本
Cluster 3 手机 苹果 三星 小米 华为 科技 智能手机 厂商 发布 推出
Cluster 4 中国 特斯拉 电影 美国 明星 马斯克 谷歌 科技 产品 攻略
Cluster 5 融资 投资 企业 中国 教育 服务 平台 行业 产品 亿美元
Cluster 6 用户 品牌 平台 内容 产品 视频 电商 社交 程序 消费
Cluster 7 城市 房价 上海 房地产 房子 北京 上涨 经济 一线 数据
Cluster 8 汽车 驾驶 自动 特斯拉 生产 技术 中国 未来 亿美元 苹果
Cluster 9 腾讯 游戏 用户 玩家 中国 内容 娱乐 视频 平台 世界
```
## 评分
```text
0.8191335472911944
``` 
## 说明
代码大部分来自这篇文章：[K-means算法及文本聚类实践](https://blog.csdn.net/havedream_one/article/details/45146829),只是没有数据集，然后自己手撕写了个36kr文章的爬虫的程序。φ(>ω<*) 
