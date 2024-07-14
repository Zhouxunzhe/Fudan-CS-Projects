import json
import pickle
from transformers import BertTokenizer, BertModel
from sklearn.cluster import KMeans
from sklearn.decomposition import PCA
import numpy as np
import torch

# tokenizer = BertTokenizer.from_pretrained("hfl/chinese-roberta-wwm-ext-large")
# model = BertModel.from_pretrained("hfl/chinese-roberta-wwm-ext-large",cache_dir = "./")

tokenizer = BertTokenizer.from_pretrained("hfl/chinese-roberta-wwm-ext")  # 选取词嵌入的模型以及其对应的分词器
model = BertModel.from_pretrained("hfl/chinese-roberta-wwm-ext", cache_dir="./")

fp = open("./Original_data/ift_cluster_given_fudandm2023.jsonl", "r", encoding="utf-8")  # 读取源文件
sents = []
for line in fp:
    data = json.loads(line)
    sents.append(data["input"])

result = np.zeros((sents.__len__(), 768))  # 结果矩阵

for i in range(sents.__len__()):
    print("{}/{}".format(i, sents.__len__()))
    out = tokenizer.encode(  # 先进行分词，再输入到模型中
        text=sents[i],
        # text_pair=sents[1],  # 一次编码两个句子，若没有text_pair这个参数，就一次编码一个句子
        # 当句子长度大于max_length时,截断
        truncation=True,  # 句子长度限制为512，过长则截断，短了就padding
        max_length=512,
        return_tensors='pt',
    )
    output = model(out)
    pooler_out = output.pooler_output  # 只取出句特征向量进行保存
    result[i] = pooler_out.detach().numpy()

fp2 = open("sent_vector_data2.pkl", "wb")
pickle.dump(result, fp2)
print("end")
