import json


def get_part_of_data(type_N, type_count):  # 获取type计数大于type_N的jsonl数据，但是由于大部分数据包含“Human”项，故该提取数据的方法并不能去除大多冗余数据
    fp = open("./Original_data/wiki_fudandm2023.jsonl", "r", encoding="utf-8")
    data_fp = open("./data/parts_of_wiki_100.jsonl", "w", encoding="utf-8")
    for line in fp:
        data = json.loads(line)
        flag = False
        for t in data["types"]:
            if type_count[t] >= type_N:
                flag = True
                break
        if flag == True:
            json.dump(data, data_fp)
            data_fp.write("\n")
    fp.close()
    data_fp.close()


def make_record(N):  # 对前N条数据生成types + h_关系 + t_关系的record,未进行去重
    fp = open("./Original_data/wiki_fudandm2023.jsonl", "r", encoding="utf-8")
    data_fp = open("./data/record_" + str(N) + ".jsonl", "w", encoding="utf-8")
    count = 0
    for line in fp:
        count += 1
        record = []
        data = json.loads(line)
        for item in data["types"]:
            record.append(item)
        for head in data["head_triples"]:
            if record.count(head[1]) == 0:
                record.append(head[1])
        for tail in data["tail_triples"]:
            if record.count(tail[1]) == 0:
                record.append(tail[1])
        json.dump(record, data_fp)
        data_fp.write("\n")
        if count >= N:
            break
    fp.close()
    data_fp.close()


def make_record_ver2(N):  # 对前N条数据生成types + h_关系计数 + t_关系计数的jsonl数据，可观察关系项的多少
    fp = open("./Original_data/wiki_fudandm2023.jsonl", "r", encoding="utf-8")
    data_fp = open("./data/record_" + str(N) + ".jsonl", "w", encoding="utf-8")
    count = 0
    for line in fp:
        count += 1
        record = {}
        data = json.loads(line)
        if data["types"] == []:
            count -= 1
            continue
        record["types"] = data["types"]
        head_dic = {}
        for head in data["head_triples"]:
            if head_dic.get(head[1]) == None:
                head_dic[head[1]] = 1
            else:
                head_dic[head[1]] += 1
        record["head"] = head_dic
        tail_dic = {}
        for tail in data["tail_triples"]:
            if tail_dic.get(tail[1]) == None:
                tail_dic[tail[1]] = 1
            else:
                tail_dic[tail[1]] += 1
        record["tail"] = tail_dic
        json.dump(record, data_fp)
        data_fp.write("\n")
        if count >= N:
            break
    fp.close()
    data_fp.close()


def make_record_ver3(N, min_P):  # 已弃用
    fp = open("./data/record_100000_test.jsonl", "r", encoding="utf-8")
    data_fp = open("./data/record_" + str(N) + "_test_remake.jsonl", "w", encoding="utf-8")
    count = 0
    for line in fp:
        count += 1
        record = []
        data = json.loads(line)
        for type in data["types"]:
            record.append(type)
        for No, h_count in data["head"].items():
            if h_count >= min_P:
                record.append(No)
        for No, t_count in data["tail"].items():
            if t_count >= min_P:
                record.append(No)
        json.dump(record, data_fp)
        data_fp.write("\n")
        if count >= N:
            break
    fp.close()
    data_fp.close()


def make_type_record(N):  # 取数据前N条生成仅含有types内容的record
    fp = open("./Original_data/wiki_fudandm2023.jsonl", "r", encoding="utf-8")
    data_fp = open("./data/record_" + str(N) + "_type_withoutHuman.jsonl", "w", encoding="utf-8")
    count = 0
    for line in fp:
        count += 1
        data = json.loads(line)
        if data["types"] == []:
            count -= 1
            continue
        record = []
        for type in data["types"]:
            if type != "Q5":
                record.append(type)
        if record != []:
            json.dump(record, data_fp)
            data_fp.write("\n")
        if count >= N:
            break


def read_type_count():  # 获取原数据中的type统计数
    fp = open("./type_count.txt", "r")
    type_count = {}
    for line in fp:
        No, _, count = line.split()
        type_count[No] = count
    return type_count


def read_alias():  # 获取qid:alias的字典，便于从qid转换为具有实际含义的名称
    fp = open("./Original_data/alias_fudandm2023.jsonl", "r", encoding="utf-8")
    alias_dic = {}
    for line in fp:
        data = json.loads(line)
        No, alias = data["qid"], data["alias"]
        alias_dic[No] = alias
    return alias_dic


def get_type_relation_head(
        N):  # 对rehandle的数据前N条生成types + h_关系的record,该数据可以使用FPgrowth算法进行处理
    fp = open("./data/record_1000000_rehandle.jsonl", "r")
    data_fp = open("./data/record_1000000_head.jsonl", "w")
    count = 0
    for line in fp:
        count += 1
        record = []
        data = json.loads(line)
        for type in data["types"]:
            record.append(type)
        if data["head"] == {}:
            continue
        for head in data["head"].keys():
            record.append(head)
        json.dump(record, data_fp)
        data_fp.write("\n")
        if count >= N:
            break


def get_type_relation_tail(N):  # 对rehandle的数据前N条生成types + t_关系的record，该数据可以使用FPgrowth算法进行处理
    fp = open("./data/record_1000000_rehandle.jsonl", "r")
    data_fp = open("./data/record_1000000_tail.jsonl", "w")
    count = 0
    for line in fp:
        count += 1
        record = []
        data = json.loads(line)
        for type in data["types"]:
            record.append(type)
        if data["tail"] == {}:
            continue
        for tail in data["tail"].keys():
            record.append(tail)
        json.dump(record, data_fp)
        data_fp.write("\n")
        if count >= N:
            break


def rehandle_record(N):  # 对ver2生成的数据取前N条，生成types + h_关系 + t_关系的数据，其中关系项计数大于min_P,即已经进行了一次筛选并去重
    count = 0
    fp = open("./data/record_1000000.jsonl", "r", encoding="utf-8")
    save_fp = open('./data/record_1000000_rehandle.jsonl', "w", encoding="utf-8")
    for line in fp:
        count += 1
        record = {}
        data = json.loads(line)
        record["types"] = data["types"]
        head = {}
        for No, p_count in data["head"].items():
            if p_count >= 8:
                head[No] = p_count
        tail = {}
        for No, p_count in data["tail"].items():
            if p_count >= 8:
                tail[No] = p_count
        record["head"] = head
        record["tail"] = tail
        json.dump(record, save_fp)
        save_fp.write("\n")
        if count >= N:
            break


def handle_head_result(resultpath):  # 对FPgrowth算法生成的关于head的规则进行提取，提取出"QXXX" => "PXXX"的规则
    fp = open(resultpath, "r")
    save_fp = open("./result/head_relation_1000000_3sup.jsonl", "w")
    for line in fp:
        data = json.loads(line)
        if data[0].__len__() == 1 and data[1].__len__() == 1:
            if data[0][0][0] == "Q" and data[1][0][0] == "P":
                json.dump(data, save_fp)
                save_fp.write("\n")


def handle_tail_result(resultpath):  # 对FPgrowth算法生成的关于tail的规则进行提取，提取出"PXXX" => "QXXX"的规则
    fp = open(resultpath, "r")
    save_fp = open("./result/tail_relation_1000000_3sup.jsonl", "w")
    for line in fp:
        data = json.loads(line)
        if data[0].__len__() == 1 and data[1].__len__() == 1:
            if data[0][0][0] == "P" and data[1][0][0] == "Q":
                json.dump(data, save_fp)
                save_fp.write("\n")


def transform_rules(rules, alias_dict, savename=None, save_flag=False,
                    show=False):  # 对上述两个函数生成的文件进行具体名称转换，可保存结果，也可仅输出结果
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


def read_rules(rulepath):  # 读取handle_head_result，handle_tail_result函数产生文件，便于上一函数进行转换
    fp = open(rulepath, "r")
    rules = []
    for line in fp:
        data = json.loads(line)
        rules.append(data)
    return rules


def connect_rules(headpath, tailpath,
                  savepath):  # 读取handle_head_result，handle_tail_result函数产生文件,并对关系项相同项进行连接
    head_fp = open(headpath, "r")
    type2head = {}
    for line in head_fp:
        data = json.loads(line)
        type2head[data[0][0]] = data[1][0]
    tail2type = {}
    tail_fp = open(tailpath, "r")
    for line in tail_fp:
        data = json.loads(line)
        if tail2type.get(data[0][0]) == None:
            tail2type[data[0][0]] = []
            tail2type[data[0][0]].append(data[1][0])
        else:
            tail2type[data[0][0]].append(data[1][0])
    save_fp = open(savepath, "w")
    for head, relation in type2head.items():
        if tail2type.get(relation) != None:
            tails = tail2type[relation]
            for tail in tails:
                result_rules = []
                result_rules.append(head)
                result_rules.append(relation)
                result_rules.append(tail)
                json.dump(result_rules, save_fp)
                save_fp.write("\n")


def transform_connected_rules(connected_rulespath, alias_dict, trans_savepath):  # 读取上一函数产生文件，并将其转换为实际名称
    rules_fp = open(connected_rulespath, "r")
    save_fp = open(trans_savepath, "w", encoding="utf-8")
    for line in rules_fp:
        data = json.loads(line)
        result = []
        result.append(alias_dict[data[0]])
        result.append(alias_dict[data[1]])
        result.append(alias_dict[data[2]])
        json.dump(result, save_fp)
        save_fp.write("\n")


# t = read_type_count()
# get_part_of_data(str(100),t)

# make_record(100)
# make_record_ver2(1000000)
# make_record_ver3(100000,10)
# make_type_record(1000000)

# rehandle_record(1000000)
# get_type_relation_head(1000000)

alias_dict = read_alias()
# rules = read_rules("./result/tail_relation_1000000.jsonl")
# transform_rules(rules,alias_dict,show=True)
# resultpath = "./result/rules_3sup_0.8con_1000000_tail.jsonl"
# handle_head_result(resultpath)
# get_type_relation_tail(1000000)
# handle_tail_result(resultpath)

# headpath = "./result/head_relation_1000000_3sup.jsonl"
# tailpath = "./result/tail_relation_1000000_3sup.jsonl"
# savepath = "./result/connected_rules.jsonl"
# connect_rules(headpath,tailpath,savepath)

connected_rules_path = "./result/connected_rules.jsonl"
savepath = "./result/trans_result.jsonl"
transform_connected_rules(connected_rules_path, alias_dict, savepath)
print("Done")
