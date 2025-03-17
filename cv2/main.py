from brute_force import brute_force_search
from kmp_search import kmp_search
from bmh_search import bmh_search
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

with open("data\\DNA-sequence.txt", "r") as f:
    DNA_sequence = f.read()

with open("data\\long.txt", "r", encoding="utf-8") as f:
    long = f.read()

with open("data\\short.txt", "r", encoding="utf-8") as f:
    short = f.read()

with open("data\\alphabet.txt", "r", encoding="utf-8") as f:
    alphabet = f.read()

patterns = {
    "DNA-sequence": ["AGCT", "TTGA", "ATATAACAACCG"],
    "long": ["ale", "a", "které"],
    "short": ["Můj", "pro", "a"],
    "alphabet": ["abab", "ba", "abababababababababababababababababababababa"]
}

# Výsledky testů
results = {"Dataset": [], "Pattern": [], "Algorithm": [], "Comparisons": []}

# Spuštění testů pro každý dataset a jeho vzory
for dataset_name, dataset in zip(patterns.keys(), [DNA_sequence, long, short, alphabet]):
    for pattern in patterns[dataset_name]:
        for algorithm_name, algorithm in [("Brute Force", brute_force_search), 
                                          ("KMP", kmp_search), 
                                          ("BMH", bmh_search)]:
            result = algorithm(dataset, pattern)
            results["Dataset"].append(dataset_name)
            results["Pattern"].append(pattern)
            results["Algorithm"].append(algorithm_name)
            results["Comparisons"].append(result["comparisons"])

df = pd.DataFrame(results)
print(df)

sns.set_theme(style="whitegrid")

for dataset_name in df["Dataset"].unique():
    dataset_df = df[df["Dataset"] == dataset_name]
    
    plt.figure(figsize=(10, 6))
    sns.barplot(x="Pattern", y="Comparisons", hue="Algorithm", data=dataset_df, palette="viridis")
    
    plt.xlabel("Vzor")
    plt.ylabel("Počet porovnání znaků")
    plt.title(f"Porovnání výkonu algoritmů – {dataset_name}")
    plt.xticks(rotation=45)
    
    plt.legend(title="Algoritmus")
    plt.ylim(0, df["Comparisons"].max() * 1.1)
    
    plt.show()