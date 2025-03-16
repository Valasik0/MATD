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

results = {
    "Dataset": ["DNA-sequence", "DNA-sequence", "DNA-sequence", 
                "long", "long", "long", 
                "short", "short", "short"],
    "Algorithm": ["Brute Force", "KMP", "BMH"] * 3,
    "Comparisons": [
        brute_force_search(DNA_sequence, "AGCT")["comparisons"],
        kmp_search(DNA_sequence, "AGCT")["comparisons"],
        bmh_search(DNA_sequence, "AGCT")["comparisons"],

        brute_force_search(long, "ale")["comparisons"],
        kmp_search(long, "ale")["comparisons"],
        bmh_search(long, "ale")["comparisons"],

        brute_force_search(short, "Můj")["comparisons"],
        kmp_search(short, "Můj")["comparisons"],
        bmh_search(short, "Můj")["comparisons"]
    ]
}

df = pd.DataFrame(results)
print(df)

sns.set_theme(style="whitegrid")

plt.figure(figsize=(10, 6))
ax = sns.barplot(x="Dataset", y="Comparisons", hue="Algorithm", data=df, palette="viridis")

plt.xlabel("Typ datasetu")
plt.ylabel("Počet porovnání znaků")
plt.title("Porovnání výkonu algoritmů")
plt.legend(title="Algoritmus")
plt.show()