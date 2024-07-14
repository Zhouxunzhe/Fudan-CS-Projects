import re


def load_feature(template_file):
    with open(template_file, 'r') as f:
        crf_template = f.readlines()
    features = []
    for line in crf_template:
        line = line.strip()
        if line.startswith('#'):
            # 忽略注释行
            continue
        elif line.startswith('U'):
            # 处理Unigram特征
            parts = line.split(':')
            expression = parts[1]
            matches = re.findall(r'\-?\d+', expression)
            if len(matches) == 2:
                matches = [int(matches[0])]
            if len(matches) == 4:
                matches = [int(matches[0]), int(matches[2])]
            feat = Feature(matches, 1)
            features.append(feat)
        elif line.startswith('B'):
            # 处理Bigram特征
            parts = line.split(':')
            expression = parts[1]
            matches = re.findall(r'\-?\d+', expression)
            if len(matches) == 2:
                matches = [int(matches[0])]
            if len(matches) == 4:
                matches = [int(matches[0]), int(matches[2])]
            feat = Feature(matches, 2)
            features.append(feat)
        else:
            # 忽略其他行
            continue

    return features


class Feature:
    def __init__(self, off, feat):
        self.size = len(off)
        self.off = off
        self.feat = feat
