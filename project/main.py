import re
from collections import Counter
import numpy as np
from typing import List, Tuple, Dict
from sklearn.decomposition import PCA
import matplotlib.pyplot as plt
import random


def open_and_read_file(file_path: str, size: int = 50000) -> str:
    with open(file_path, 'r', encoding='utf-8') as f:
        text = f.read().replace('\n', ' ')
        chars = "\\`*_{}[]()>#–+-.!$/~<?,\"\':;&%0123456789\n\r"
        for c in chars:
            text = text.replace(c, "")
    return text[:size]

def tokenize(text: str) -> Tuple[List[int], Dict[str, int]]:
    splitted = re.findall(r'\w+|[^\w\s]', text.lower(), re.UNICODE)

    vocab_size = 10000
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

def softmax(x: np.ndarray) -> np.ndarray: #prevede z (z metodz cbow_forward) na pravděpodobnosti
    e_x = np.exp(x - np.max(x))
    return e_x / e_x.sum()

def cross_entropy_loss(y_pred: np.ndarray, y_true: int) -> float:
    return -np.log(y_pred[y_true] + 1e-10)

def cosine_similarity(a, b):
    return np.dot(a, b) / (np.linalg.norm(a) * np.linalg.norm(b))

def cbow_forward(context: List[int], E: np.ndarray, W: np.ndarray, b: np.ndarray) -> np.ndarray:
    context_embeddings = E[context]
    h = np.mean(context_embeddings, axis=0)
    z = np.dot(h, W) + b #z = W * V_context + b, shape (vocab_size), logits
    return z 

def cbow_backpropagation(context: List[int], target: int, y_pred: np.ndarray, E: np.ndarray, W: np.ndarray, b: np.ndarray, learning_rate: float) -> None:
    vocab_size = y_pred.shape[0]
    context_size = len(context)
    h = np.mean(E[context], axis=0)
    
    y_true = np.zeros(vocab_size)
    y_true[target] = 1 # one-hot
    
    delta = y_pred - y_true

    dW = np.outer(h, delta)
    db = delta
    dh = np.dot(W, delta) 
    
    W -= learning_rate * dW
    b -= learning_rate * db
    for idx in context:
        E[idx] -= learning_rate * (dh / context_size)

def train(X, y, E, W, b, epochs=5, learning_rate=0.05):
    for epoch in range(epochs):
        total_loss = 0
        for i in range(len(X)):
            z = cbow_forward(X[i], E, W, b)
            y_pred = softmax(z)
            loss = cross_entropy_loss(y_pred, y[i])
            total_loss += loss
            cbow_backpropagation(X[i], y[i], y_pred, E, W, b, learning_rate)

        print(f"Epoch {epoch+1}, průměrná loss: {total_loss / len(X):.4f}")

def find_nearest(word: str, vocab: Dict[str, int], E: np.ndarray, topn: int = 5):
    idx = vocab.get(word, 0)
    emb = E[idx]
    sims = []
    for w, i in vocab.items():
        if w == word or i == 0:
            continue
        sim = cosine_similarity(emb, E[i])
        sims.append((w, sim))
    sims.sort(key=lambda x: -x[1])
    return sims[:topn]


if __name__ == "__main__":
    text = open_and_read_file('czech_news_content.txt')
    tokens, vocab = tokenize(text)

    X, y = create_context_target_pairs(tokens, window_size=2)
    vocab_size = len(vocab)
    embedding_dim = 100

    E = np.random.normal(0, 0.01, (vocab_size, embedding_dim))
    W = np.random.normal(0, 0.01, (embedding_dim, vocab_size))
    b = np.zeros(vocab_size)

    train(X, y, E, W, b, epochs=40, learning_rate=0.1)

    my_word = "mluvčí"

    nearest_words = find_nearest(my_word, vocab, E, topn=10)
    nearest_word_list = [w for w, _ in nearest_words]
    nearest_indices = [vocab[w] for w in nearest_word_list]
    print(f"Nejbližší slova k {my_word}:")
    for word, sim in nearest_words:
        print(f"{word}: {sim:.4f}")

    all_indices = set(range(1, len(vocab)))
    used_indices = set(nearest_indices)
    idx_to_word = {idx: word for word, idx in vocab.items()}
    random_indices = random.sample(list(all_indices - used_indices), 10)
    random_word_list = [idx_to_word[i] for i in random_indices]

    words = nearest_word_list + random_word_list
    indices = nearest_indices + random_indices
    vectors = E[indices]

    pca = PCA(n_components=2)
    reduced = pca.fit_transform(vectors)

    plt.figure(figsize=(8, 8))
    plt.scatter(reduced[:, 0], reduced[:, 1])
    for i, word in enumerate(words):
        if word in vocab:
            plt.annotate(word, (reduced[i, 0], reduced[i, 1]))
    plt.title("Vizualizace embeddingů pomocí PCA")
    plt.xlabel("PCA 1")
    plt.ylabel("PCA 2")
    plt.show()