from datasets import load_dataset
dataset = load_dataset("CIIRC-NLP/czech_news_simple-cs")

# Zobrazení první ukázky dat
print(dataset['test']['content'])