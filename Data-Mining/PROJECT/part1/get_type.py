import json


type_num = {}
count = 0
with open("zwiki_fudandm2023.jsonl","r",encoding="utf-8") as f:
# with open("test.jsonl","r",encoding="utf-8") as f:
    for line in f:
        count += 1
        data = json.loads(line)
        t = data["types"]
        for item in t:
            if type_num.get(item) == None:
                type_num[item] = 1
            else:
                type_num[item] += 1
        if count % 10000 == 0:
            print(count)
new_type_num = sorted(type_num.items(),key = lambda d:d[1],reverse=False)
# print(new_type_num)
fp = open("type_count.txt","w",encoding="utf-8")
for item in new_type_num:
    fp.write("{} : {}\n".format(item[0],item[1]))
