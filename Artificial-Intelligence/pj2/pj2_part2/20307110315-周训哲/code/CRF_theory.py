from torch import nn
import torch


class CRF(nn.Module):
    """
    CRF类
    给定 '标签序列'和'发射矩阵分数' 计算对数似然（也就是损失函数）
    同时有decode函数，通过维特比算法，根据发射矩阵分数计算最优的标签序列（暂不展示）

    超参数：
        num_tags: 标签的个数（一般为自行设计的BIO标签的个数）
    学习参数：
        transitions (torch.nn.Parameter): 转移矩阵， shape=(num_tags, num_tags) 方阵
    """

    def __init__(self, num_tags):
        super().__init__()
        self.num_tags = num_tags

        # 学习参数
        self.transitions = nn.Parameter(torch.empty(num_tags, num_tags))  # (num_tags, num_tags)
        # 重新随机初始化参数矩阵，初始化为-0.1 ~ 0.1的均匀分布
        nn.init.uniform_(self.transitions, -0.1, 0.1)

    def forward(self,
                emissions: torch.Tensor,
                tags: torch.LongTensor,
                mask: torch.ByteTensor = None,
                reduction: str = 'mean') -> torch.Tensor:
        """计算给定发射分数张量和真实标签序列，来计算序列的条件对数似然 (conditional log likelihood)
        参数：
            （1）emissions: 发射分数，一般是LSTM/BERT的输出表示,
                           shape = (batch_size, seq_len, num_tags)
            （2）tags:  真实标签序列，每一条BIO类似标注，
                        shape = (batch_size, seq_len)
            （3）mask:  mask矩阵，排除padding的影响，
                        shape = (batch_size, seq_len)
            （4）reduction: 计算最后输出的策略，可选（none|sum|mean|token_mean）。
                       （none什么不做，sum在batch维度求和，mean在batch维度平均，token_mean在token维度平均）
        返回：
            对数似然分数(log likelihood)，如果reduction='none', 则shape=(batch_size,) ，否则返回一个常数
        """
        if reduction not in ('none', 'sum', 'mean', 'token_mean'):
            # 判断是否非法
            raise ValueError(f'invalid reduction: {reduction}')
        if mask is None:
            mask = torch.ones_like(tags, dtype=torch.uint8, device=tags.device)  # (batch_size, seq_len) 全为1
        if mask.dtype != torch.uint8:
            mask = mask.byte()

        # 计算分子
        numerator = self._compute_score(emissions, tags, mask)  # shape=(batch_size,)
        # 计算分母
        denominator = self._compute_normalizer(emissions, mask)  # shape=(batch_size,)

        llh = numerator - denominator  # 极大对数似然分数，如果nn反向传播需要加个负号 ，shape=(batch_size)
        if reduction == 'none':
            return llh
        if reduction == 'sum':
            return llh.sum()  # 常数张量
        if reduction == 'mean':
            return llh.mean()  # 常数张量
        # 下面解释一下：mask.float().sum()代表这一batch里一共有多少个有效字，然后把这些序列的所有字累加。
        # （主要还是去除padding的影响）
        return llh.sum() / mask.float().sum()  # 常数张量

    def _compute_score(self, emissions, tags, mask):
        """
        emissions: (batch_size, seq_len, num_tags)
        tags:      (batch_size, seq_len)
        mask:      (batch_size, seq_len)
        返回对数似然的分子： (batch_size)
        """
        batch_size, seq_length = tags.shape
        mask = mask.float()

        # 序列第一个位置的发射分数，记为score
        score = emissions[torch.arange(batch_size), 0, tags[:, 0]]  # (batch_size)

        for i in range(1, seq_length):  # 对接下来序列的每一个字进行遍历，相加计算总分数
            # 开始累加转移分数，当且仅当mask=1（排除padding）执行
            score += self.transitions[tags[:, i - 1], tags[:, i]] * mask[:, i]  # （batch_size, )

            # 开始累加发射分数，当且仅当mask=1（排除padding）执行
            score += emissions[torch.arange(batch_size), i, tags[:, i]] * mask[:, i]  # (batch_size, )
        # 完事，返回分数
        return score

    def _compute_normalizer(self, emissions, mask):
        """
        emissions: (batch_size, seq_len, num_tags)
        mask: (batch_size, seq_len)
        返回对数似然的分母（归一化因子）：(batch_size,)
        """
        seq_length = emissions.size(1)

        # 开始发射矩阵的分数，注意到因为要求归一化分母，所以需要知道每个tag的似然分数 (根据公式(5))
        # 即，shape = (batch_size, num_tags)
        score = emissions[:, 0]  # (batch_size, num_tags)

        for i in range(1, seq_length):
            # 广播score, 方便算每条样本的所有tag之间的转移
            broadcast_score = score.unsqueeze(2)  # (batch_size, num_tags, 1)

            # 广播emissions，方便算每条样本的所有tag之间的转移
            broadcast_emissions = emissions[:, i].unsqueeze(1)  # (batch_size, 1, num_tags)

            # 对于每个序列，计算仅到此时刻t的归一化因子
            next_score = broadcast_score + self.transitions + broadcast_emissions  # (bs, num_tags, num_tags)

            # 先指数求和再计算对数，即 logsumexp
            next_score = torch.logsumexp(next_score, dim=1)  # (bs, num_tags)

            # 去除padding的影响
            score = torch.where(mask[:, i].unsqueeze(1), next_score, score)

        return torch.logsumexp(score, dim=1)  # (batch_size,)
