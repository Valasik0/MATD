from collections import Counter
import math
import numpy as np

stop_words = set(["i", "he", "she", "is", "a", "some", "for", "the", "to", "it", "this", "of", "and", 'or', 'on', 'in', 'at', 'an', 'as', 'by', 'with', 'that', 'from', 'about', 'be', 'are', 'was', 'were', 'has', 'have', 'had', 'not', 'but', 'if', 'then'])

def load_and_process_data():
    result = {}
    for i in range(2,18):
        
        with open(f"data\\{i}.txt", "r", encoding="utf-8") as file:
            text = file.read()
            chars = "\\`*_{}[]()>#+-.!$/<?,\"\':;&%0123456789\n\r"
            for c in chars:
                text = text.lower().replace(c, "")
            words = text.split()
            words = [word for word in words if word not in stop_words]
            result[i] = words
    return result


def calculate_tf(data):
    result = {}
    for doc_id, words in data.items():
        word_counts = Counter(words)
        total_words = len(words)
        result[doc_id] = {word: count / total_words for word, count in word_counts.items()}
    return result

def calculate_idf(data):
    idf = {}
    N_docs = len(data)

    all_words = set(word for words in data.values() for word in words)

    for word in all_words:
        doc_count = sum(1 for words in data.values() if word in words)
        idf[word] = math.log((N_docs) / (doc_count))
    return idf


def calculate_tf_idf(tf_dict, idf_dict):
    tf_idf_dict = {}
    for doc_id, tf_values in tf_dict.items():
        tf_idf_dict[doc_id] = {term: tf * idf_dict[term] for term, tf in tf_values.items()}
    return tf_idf_dict

def calculate_score(query, tf_idf_dict):
    scores = {}
    
    query_terms = query.lower().replace("(", "").replace(")", "").split()
    required_terms = set() 
    optional_terms = set() 
    forbidden_terms = set() 
    
    mode = "OR"
    
    for term in query_terms:
        if term == "and":
            mode = "AND"
        elif term == "or":
            mode = "OR"
        elif term == "not":
            mode = "NOT"
        else:
            if mode == "AND":
                required_terms.add(term)
            elif mode == "OR":
                optional_terms.add(term)
            elif mode == "NOT":
                forbidden_terms.add(term)
    
    for doc_id, tf_idf_values in tf_idf_dict.items():
        doc_terms = set(tf_idf_values.keys())

        if required_terms and not required_terms.issubset(doc_terms):
            continue
        
        if forbidden_terms and forbidden_terms & doc_terms:
            continue
        
        scores[doc_id] = sum(tf_idf_values.get(term, 0) for term in required_terms | optional_terms)

    return sorted(scores.items(), key=lambda x: x[1], reverse=True)

def cosine_similarity(vec1, vec2):
    dot_product = np.dot(vec1, vec2)
    norm1 = np.linalg.norm(vec1)
    norm2 = np.linalg.norm(vec2)
    return dot_product / (norm1 * norm2) if norm1 != 0 and norm2 != 0 else 0

def create_tf_idf_matrix(tf_idf_dict):
    all_terms = sorted(set(term for doc in tf_idf_dict.values() for term in doc))  # Unikátní termy
    term_index = {term: i for i, term in enumerate(all_terms)}  # Indexace termů
    
    doc_vectors = []
    doc_ids = sorted(tf_idf_dict.keys())  # Seřazené ID dokumentů
    
    for doc_id in doc_ids:
        vector = np.zeros(len(all_terms))
        for term, value in tf_idf_dict[doc_id].items():
            vector[term_index[term]] = value  # Naplníme hodnoty TF-IDF
        doc_vectors.append(vector)
    
    return np.array(doc_vectors), doc_ids

def compute_all_cosine_similarities(tf_idf_dict):
    doc_vectors, doc_ids = create_tf_idf_matrix(tf_idf_dict)
    n = len(doc_vectors)
    
    similarities = {}
    max_sim = -1
    most_similar_docs = None

    for i in range(n):
        for j in range(i + 1, n):  # Porovnáváme jen unikátní dvojice
            sim = cosine_similarity(doc_vectors[i], doc_vectors[j])
            similarities[(doc_ids[i], doc_ids[j])] = sim
            
            if sim > max_sim:
                max_sim = sim
                most_similar_docs = (doc_ids[i], doc_ids[j])
    
    return similarities, most_similar_docs, max_sim

processed_docs = load_and_process_data()
tf_dict = calculate_tf(processed_docs)
idf_dict = calculate_idf(processed_docs)
tf_idf_dict = calculate_tf_idf(tf_dict, idf_dict)
similarities, most_similar_docs, max_sim = compute_all_cosine_similarities(tf_idf_dict)

print(f"Nejpodobnější dokumenty: {most_similar_docs} s kosinovou podobností {max_sim:.4f}")

query = "queen AND king"

sorted_docs = calculate_score(query, tf_idf_dict)

for sorted_doc in sorted_docs:
    doc_id, score = sorted_doc
    print(f"Dokument ID {doc_id} má skóre {score:.5f}")

"""
Jak by se výsledky změnily, kdyby se použilo jen tf bez idf? - 
  - častá slova by měly větší váhua u uměle by zvyšovala podobnost dokumentů
  - delší dokumenty mají více slov tím pádem i vyšší tf skore, to může zvýhodnit delší texty

Uveďte příklad oblasti, témata, kde by častá slova mohla být navzdory vysoké frekvenci velmi důležitá.
Například v oblasti medicíny nebo právních dokumentů, kde se často používají specifické termíny, které mohou mít zásadní význam pro pochopení textu.
    - odborné text např komprimace dat (entropie, komprese, kodování)
Vysvětlete, proč v takovém případě může být použití klasického idf nevhodné.
    - částé slova klíčove pro tuto oblast by byla považována za běžná a tím pádem by byla penalizována, což by vedlo k nižší relevanci výsledků.
Navrhněte úpravu výpočtu, která by tento problém zmírnila.
    - např ruční nastavení/selekci těchto termů, které by měly vysokou váhu i přes častost výskytu

"""