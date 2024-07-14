import time
import os
import operator

input_path = 'C:/Users/ZhouXunZhe/Desktop/WordCount/input/'
wordcount = {}
start = time.perf_counter()

for root, dirs, files in os.walk(input_path):
    for file in files:
        with open(input_path + file, 'r') as f:
            lines = f.readlines()
            for line in lines:
                words = line.split()
                for word in words:
                    if word not in wordcount:
                        wordcount[word] = 0
                    wordcount[word] += 1
wordcount = dict(sorted(wordcount.items(), key=operator.itemgetter(0)))
with open('C:/Users/ZhouXunZhe/Desktop/WordCount/output/myoutput.txt', 'w') as g:
    for word in wordcount.keys():
        g.write('%s\t%s\n' % (word, wordcount[word]))

end = time.perf_counter()
print(str(end - start) + ' s')
