import re
from collections import Counter

def open_and_read_file(file_path):
    with open(file_path, 'r', encoding='utf-8') as f:
        text = f.read().replace('\n', ' ')
        chars = "\\`*_{}[]()>#–+-.!$/~<?,\"\':;&%0123456789\n\r"
        for c in chars:
            text = text.replace(c, "")
    return text

def tokenize(text):
    splitted = re.findall(r'\w+|[^\w\s]', text.lower(), re.UNICODE)

    vocab_size = 25000
    most_common = Counter(splitted).most_common(vocab_size)
    vocab = {word: idx+1 for idx, (word, _) in enumerate(most_common)} 
    vocab['<UNK>'] = 0
    indexed_tokens = [vocab.get(token, vocab['<UNK>']) for token in splitted]

    return indexed_tokens, vocab

def create_context_target_pairs(indexed_tokens, window_size=2):
    X = []
    y = []

    for i in range(window_size, len(indexed_tokens) - window_size):
        context = indexed_tokens[i - window_size:i] + indexed_tokens[i + 1:i + window_size + 1]
        target = indexed_tokens[i]
        X.append(context)
        y.append(target)

    return X, y


text = open_and_read_file('czech_news_content.txt')
tokens = tokenize(text)