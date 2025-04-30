import re
from collections import Counter
import numpy as np

def open_and_read_file(file_path: str) -> str:
    with open(file_path, 'r', encoding='utf-8') as f:
        text = f.read().replace('\n', ' ')
        chars = "\\`*_{}[]()>#–+-.!$/~<?,\"\':;&%0123456789\n\r"
        for c in chars:
            text = text.replace(c, "")
    return text

def tokenize(text: str) -> tuple:
    splitted = re.findall(r'\w+|[^\w\s]', text.lower(), re.UNICODE)

    vocab_size = 5000
    most_common = Counter(splitted).most_common(vocab_size)
    vocab = {word: idx+1 for idx, (word, _) in enumerate(most_common)} 
    vocab['<UNK>'] = 0
    indexed_tokens = [vocab.get(token, vocab['<UNK>']) for token in splitted]

    return indexed_tokens, vocab

def create_context_target_pairs(indexed_tokens: list, window_size: int = 2) -> tuple:
    X = []
    y = []

    for i in range(window_size, len(indexed_tokens) - window_size):
        context = indexed_tokens[i - window_size:i] + indexed_tokens[i + 1:i + window_size + 1]
        target = indexed_tokens[i]
        X.append(context)
        y.append(target)

    return X, y


def cbow_forward(context, E, W, b):
    context_embeddings = E[context]  # shape: (context_size, embedding_dim)
    h = np.mean(context_embeddings, axis=0)  # shape: (embedding_dim,)
    u = np.dot(h, W) + b  # shape: (vocab_size,)
    return u 


text = open_and_read_file('czech_news_content.txt')
tokens, vocab = tokenize(text)

X, y = create_context_target_pairs(tokens, window_size=2)
vocab_size = len(vocab)
embedding_dim = 100

E = np.random.normal(0, 0.1, (vocab_size, embedding_dim))
W = np.random.normal(0, 0.1, (embedding_dim, vocab_size))
b = np.zeros(vocab_size)

print(E[X[0]])