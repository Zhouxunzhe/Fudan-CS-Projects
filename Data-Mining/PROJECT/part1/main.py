import FPtree
import json
from data_handler import read_alias


def make_datasets(datapath):
    fp = open(datapath, "r")
    datasets = []
    for line in fp:
        data = json.loads(line)
        datasets.append(data)
    return datasets


def transform_rules(rules, alias_dict, savename=None, save_flag=False, show=False):
    if save_flag == True:
        fp = open(savename, "w", encoding="utf-8")
    for rule in rules:
        pre_names = []
        last_names = []
        for pre_item in rule[0]:
            if pre_item == "P2439": continue
            pre_names.append(alias_dict[pre_item])
        for last_item in rule[1]:
            last_names.append(alias_dict[last_item])
        pre = ""
        for name in pre_names:
            pre = pre + name + ","
        pre = "(" + pre + ")"
        last = ""
        for name in last_names:
            last = last + name + ","
        last = "(" + last + ")"
        out = pre + " => " + last + " confidence : {}".format(rule[2])
        if show == True:
            print(out)
        if save_flag == True:
            fp.write(out + "\n")


def save_raw_rules(rules, savename):
    fp = open(savename, "w", encoding="utf-8")
    for rule in rules:
        result = []
        pre_names = []
        last_names = []
        for pre_item in rule[0]:
            pre_names.append(pre_item)
        for last_item in rule[1]:
            last_names.append(last_item)
        result.append(pre_names)
        result.append(last_names)
        result.append(rule[2])
        json.dump(result, fp)
        fp.write("\n")
    fp.close()


savepath = "./result/rules_3sup_0.8con_1000000_tail.txt"  # 对rules转换为实际名称后文件的保存路径
raw_rule_savepath = "./result/rules_3sup_0.8con_1000000_tail.jsonl"  # FPgrowth算法生成的rule的保存路径，可以由data_handler中的handle_head_result，handle_tail_result函数处理
# datapath = "./data/record_100000_type.jsonl"                              #对type类型挖掘时的输入数据，freq不需做限制，可取一个大值
datapath = "./data/record_1000000_tail.jsonl"  # get_type_relation_tail，get_type_relation_head函数生成的record进行挖掘，使用该record挖掘时，freq最大长度应限制为2，否则用时过长
dataset = make_datasets(datapath)  # 加载数据集
fun = FPtree.FPtree(min_support=3, min_confidence=0.8, freq=2)  # 设置FPgrowth算法的参数
frequent_items, rules = fun.train(dataset, display=False)  # 进行挖掘
save_raw_rules(rules, raw_rule_savepath)  # 保存rule数据
alias_dict = read_alias()  # 读取转换实际名称的字典
transform_rules(rules, alias_dict, save_flag=True, savename=savepath)  # 保存转换后的数据
