
class FPNode:
    def __init__(self, item, count, parent):
        self.item = item
        self.count = count              
        self.parent = parent
        self.next = None               
        self.children = {}

    def display(self, ind=1):
        print(' '*ind, self.item, ':', self.count)
        for child in self.children.values():
            child.display(ind+1)

class FPtree:
    def __init__(self, min_support=3, min_confidence=0.6, freq = 5):
        self.min_support = min_support
        self.min_confidence = min_confidence
        self.max_len_freq = freq


    def transfer2FrozenDataSet(self, data):
        frozen_data = {}
        for elem in data:
            frozen_data[frozenset(elem)] = 1
        return frozen_data


    def updataTree(self, data, FP_tree, header, count):
        frequent_item = data[0]
        if frequent_item in FP_tree.children:
            FP_tree.children[frequent_item].count += count
        else:
            FP_tree.children[frequent_item] = FPNode(frequent_item, count, FP_tree)
            if header[frequent_item][1] is None:
                header[frequent_item][1] = FP_tree.children[frequent_item]
            else:
                self.updateHeader(header[frequent_item][1], FP_tree.children[frequent_item]) 

        if len(data) > 1:
            self.updataTree(data[1::], FP_tree.children[frequent_item], header, count) 


    def updateHeader(self, head_node, tail_node):
        while head_node.next is not None:
            head_node = head_node.next
        head_node.next = tail_node


    def createFPTree(self, train_data):
        initial_header = {}

        for record in train_data:
            for item in record:
                initial_header[item] = initial_header.get(item, 0) + train_data[record]


        header = {}
        for k in initial_header.keys():
            if initial_header[k] >= self.min_support:
                header[k] = initial_header[k]
        frequent_set = set(header.keys())
        if len(frequent_set) == 0:
            return None, None


        for k in header:
            header[k] = [header[k], None]


        FP_tree = FPNode('root', 1, None)        
        for record, count in train_data.items():
            frequent_item = {}
            for item in record:                
                if item in frequent_set:       
                    frequent_item[item] = header[item][0]

            if len(frequent_item) > 0:
                ordered_frequent_item = [val[0] for val in sorted(frequent_item.items(), key=lambda val:val[1], reverse=True)] 
                self.updataTree(ordered_frequent_item, FP_tree, header, count) 

        return FP_tree, header


    def ascendTree(self, node):
        prefix_path = []
        while node.parent != None and node.parent.item != 'root':
            node = node.parent
            prefix_path.append(node.item)
        return prefix_path


    def getPrefixPath(self, base, header):
        prefix_path = {}
        start_node = header[base][1]
        prefixs = self.ascendTree(start_node)
        if len(prefixs) != 0:
            prefix_path[frozenset(prefixs)] = start_node.count

        while start_node.next is not None:
            start_node = start_node.next
            prefixs = self.ascendTree(start_node)
            if len(prefixs) != 0:
                prefix_path[frozenset(prefixs)] = start_node.count
        return prefix_path


    def findFrequentItem(self, header, prefix, frequent_set):

        header_items = [val[0] for val in sorted(header.items(), key=lambda val: val[1][0])]
        if len(header_items) == 0:
            return

        for base in header_items:
            new_prefix = prefix.copy()
            new_prefix.add(base)
            if new_prefix.__len__() > self.max_len_freq:
                continue
            support = header[base][0]
            frequent_set[frozenset(new_prefix)] = support

            prefix_path = self.getPrefixPath(base, header)
            if len(prefix_path) != 0:
                conditonal_tree, conditional_header = self.createFPTree(prefix_path)
                if conditional_header is not None:
                    self.findFrequentItem(conditional_header, new_prefix, frequent_set)


    def generateRules(self, frequent_set, rules):
        for frequent_item in frequent_set:
            if len(frequent_item) > 1:
                self.getRules(frequent_item, frequent_item, frequent_set, rules)


    def removeItem(self, current_item, item):
        tempSet = []
        for elem in current_item:
            if elem != item:
                tempSet.append(elem)
        tempFrozenSet = frozenset(tempSet)
        return tempFrozenSet


    def getRules(self, frequent_item, current_item, frequent_set, rules):
        for item in current_item:
            subset = self.removeItem(current_item, item)
            if frequent_set.get(subset) != None and frequent_set.get(frequent_item) != None:
                confidence = frequent_set[frequent_item]/frequent_set[subset]
            else:
                confidence = 0
            if confidence >= self.min_confidence:
                flag = False
                for rule in rules:
                    if (rule[0] == subset) and (rule[1] == frequent_item - subset):
                        flag = True

                if flag == False:
                    rules.append((subset, frequent_item - subset, confidence))

                if (len(subset) >= 2):
                    self.getRules(frequent_item, subset, frequent_set, rules)


    def train(self, data, display=True):
        data = self.transfer2FrozenDataSet(data)
        FP_tree, header = self.createFPTree(data)                               #生成FPtree
        frequent_set = {}
        prefix_path = set([])
        self.findFrequentItem(header, prefix_path, frequent_set)                #对FPtree挖掘频繁项集
        rules = []
        self.generateRules(frequent_set, rules)                                 #对频繁项集挖掘规则

        if display:
            print("Result of FPgrowth:")
            print("Frequent Items:")
            for item in frequent_set:
                print(item)
            print("_______________________________________")
            print("Association Rules:")
            for rule in rules:
                print(rule)
        return frequent_set, rules
