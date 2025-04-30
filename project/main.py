import re
from collections import Counter
import numpy as np
from typing import List, Tuple, Dict

def open_and_read_file(file_path: str) -> str:
    with open(file_path, 'r', encoding='utf-8') as f:
        text = f.read().replace('\n', ' ')
        chars = "\\`*_{}[]()>#–+-.!$/~<?,\"\':;&%0123456789\n\r"
        for c in chars:
            text = text.replace(c, "")
    return text

def tokenize(text: str) -> Tuple[List[int], Dict[str, int]]:
    splitted = re.findall(r'\w+|[^\w\s]', text.lower(), re.UNICODE)

    vocab_size = 5000
    most_common = Counter(splitted).most_common(vocab_size)
    vocab = {word: idx+1 for idx, (word, _) in enumerate(most_common)} 
    vocab['<UNK>'] = 0
    indexed_tokens = [vocab.get(token, vocab['<UNK>']) for token in splitted]

    return indexed_tokens, vocab

def create_context_target_pairs(indexed_tokens: List[int], window_size: int = 2) -> Tuple[List[List[int]], List[int]]:
    X = []
    y = []

    for i in range(window_size, len(indexed_tokens) - window_size):
        context = indexed_tokens[i - window_size:i] + indexed_tokens[i + 1:i + window_size + 1]
        target = indexed_tokens[i]
        X.append(context)
        y.append(target)

    return X, y

def softmax(x): #prevede z (z metodz cbow_forward) na pravděpodobnosti
    e_x = np.exp(x - np.max(x))
    return e_x / e_x.sum()

def cross_entropy_loss(y_pred, y_true):
    return -np.log(y_pred[y_true] + 1e-10)

def cbow_forward(context: List[int], E: np.ndarray, W: np.ndarray, b: np.ndarray) -> np.ndarray:
    context_embeddings = E[context]
    h = np.mean(context_embeddings, axis=0)
    z = np.dot(h, W) + b #z = W * V_context + b, shape (vocab_size), logits
    return z 

def cbow_backpropagation(context, target, y_pred, E, W, b, learning_rate):
    vocab_size = y_pred.shape[0]
    context_size = len(context)
    h = np.mean(E[context], axis=0)
    
    y_true = np.zeros(vocab_size)
    y_true[target] = 1
    
    delta = y_pred - y_true
    
    # 3. Gradienty
    dW = np.outer(h, delta)  # (embedding_dim, vocab_size)
    db = delta
    dh = np.dot(W, delta)  # (embedding_dim,)
    
    # 4. Update váhy
    W -= learning_rate * dW
    b -= learning_rate * db
    for idx in context:
        E[idx] -= learning_rate * (dh / context_size)


text = open_and_read_file('czech_news_content.txt')
tokens, vocab = tokenize(text)

X, y = create_context_target_pairs(tokens, window_size=2)
vocab_size = len(vocab)
embedding_dim = 100

E = np.random.normal(0, 0.1, (vocab_size, embedding_dim))
W = np.random.normal(0, 0.1, (embedding_dim, vocab_size))
b = np.zeros(vocab_size)

z = cbow_forward(X[0], E, W, b)

print(softmax(z))

