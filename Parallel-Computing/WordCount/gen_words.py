import random
from nltk.corpus import words

MAX_WORDS = 5
word_list = words.words()
my_word_list = {}
for i in range(1, MAX_WORDS+1):
    filtered_words = [s for s in word_list if len(s) == i]
    my_word_list[i] = filtered_words


def generate_word(length):
    filtered_strings = my_word_list[length]
    if filtered_strings:
        return random.choice(filtered_strings)
    else:
        return None


def generate_sentence(length):
    sentence = []
    while len(sentence) < length:
        word_len = random.randint(1, MAX_WORDS)
        word = generate_word(word_len)
        sentence.append(word)
    return ' '.join(sentence)


def generate_article(length):
    article = []
    word_size = 0
    while word_size < length:
        sentence_length = random.randint(5, 15)
        sentence = generate_sentence(sentence_length)
        article.append(sentence)
        word_size += sentence_length
    return '\n'.join(article)


length = 90000000
article = generate_article(length)
with open('C:/Users/ZhouXunZhe/Desktop/WordCount/input/' + str(length) + '.txt', 'w') as f:
    f.write(article)
