import re
from collections import Counter
import random
import math

def load_text(path):
    with open(path, 'r', encoding='utf-8') as file:
        data = file.read()
    
    return data

def preprocess_text(text):
    text = text.lower()
    text = re.sub(r'[^\w\s]', '', text)
    text = re.sub(r'\s+', ' ', text)
    tokens = text.split(' ')

    return tokens

def create_ngram_model(tokens, n=2, alpha=1):
    ngrams = [' '.join(tokens[i:i+n]) for i in range(len(tokens)-n+1)]
    
    ngram_freq = Counter(ngrams)
    context_freq = Counter([' '.join(ngram.split()[:n-1]) for ngram in ngrams])
    
    vocab_size = len(set(tokens))
    
    def laplace_smoothing(count, context_count):
        return (count + alpha) / (context_count + alpha * vocab_size)
    
    ngram_prob = {}
    for ngram, freq in ngram_freq.items():
        context = ' '.join(ngram.split()[:n-1])
        ngram_prob[ngram] = laplace_smoothing(freq, context_freq[context])
    
    return ngram_prob

def predict_next_word(ngram_model, context, top_k=3):
    predictions = {ngram.split()[-1]: prob for ngram, prob in ngram_model.items() if ngram.startswith(context)}
    
    if not predictions:
        return ["(žádná predikce)"]

    sorted_predictions = sorted(predictions.items(), key=lambda x: x[1], reverse=True)
    
    return [word for word, _ in sorted_predictions[:top_k]]


def generate_text(trigram_model, start_words=None, max_words=30, max_repeats=3):
    if start_words is None:
        start_words = random.choice(list(trigram_model.keys()))
    
    words = start_words.split()
    seen_ngrams = {}

    for _ in range(max_words):
        context = ' '.join(words[-2:])
        next_words = {ngram.split()[-1]: prob for ngram, prob in trigram_model.items() if ngram.startswith(context)}
        
        if not next_words:
            break
        
        next_word = max(next_words, key=next_words.get)
        #next_word = random.choices(list(next_words.keys()), weights=next_words.values())[0]
        
        trigram = ' '.join(words[-2:] + [next_word])
        seen_ngrams[trigram] = seen_ngrams.get(trigram, 0) + 1

        if seen_ngrams[trigram] > max_repeats:
            break  
        words.append(next_word)
            
    return ' '.join(words)

def calculate_perplexity(ngram_model, test_tokens, n=2):
    log_prob_sum = 0
    total_words = 0
    
    for i in range(n-1, len(test_tokens)):
        context = ' '.join(test_tokens[i-n+1:i])
        word = test_tokens[i]
        
        ngram = context + " " + word
        prob = ngram_model.get(ngram, 0)
        
        if prob > 0:
            log_prob_sum += math.log(prob)
        else:
            log_prob_sum += math.log(1e-10)
        
        total_words += 1
    
    perplexity = math.exp(-log_prob_sum / total_words)
    return perplexity

data = load_text('data\\cs.txt')
tokens = preprocess_text(data)

unigram_model = create_ngram_model(tokens, n=1)
bigram_model = create_ngram_model(tokens, n=2)
trigram_model = create_ngram_model(tokens, n=3)


print("Nejpravděpodobnější bigramy:")
print(sorted(bigram_model.items(), key=lambda x: x[1], reverse=True)[:10])

print("\nNejpravděpodobnější trigramy:")
print(sorted(trigram_model.items(), key=lambda x: x[1], reverse=True)[:10])

print("Predikce pro 'já':", predict_next_word(bigram_model, "já"))
print("Predikce pro 'mám':", predict_next_word(bigram_model, "mám"))

print("Predikce pro 'já jsem':", predict_next_word(trigram_model, "já jsem"))
print("Predikce pro 'on byl':", predict_next_word(trigram_model, "on byl"))

print(generate_text(trigram_model, start_words="já jsem"))
print(generate_text(trigram_model, start_words="on byl"))

print("Perplexity pro unigramy:", calculate_perplexity(unigram_model, tokens, n=1))
print("Perplexity pro bigramy:", calculate_perplexity(bigram_model, tokens, n=2))
print("Perplexity pro trigramy:", calculate_perplexity(trigram_model, tokens, n=3))