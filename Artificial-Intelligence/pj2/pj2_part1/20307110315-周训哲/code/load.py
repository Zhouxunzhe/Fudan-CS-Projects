def load_data(file_path):
    states = []
    observation = []
    s = []
    o = []
    with open(file_path, 'r', encoding='UTF-8') as f:
        lines = f.readlines()
        for i in range(len(lines)):
            if lines[i] == "\n":
                states.append(s)
                observation.append(o)
                s = []
                o = []
                continue
            word, tag = lines[i].strip().split(" ")
            o.append(word)
            s.append(tag)
    f.close()
    return states, observation
