import time

def load_data(file_path):
    with open(file_path, "r", encoding="utf-8") as file:
        data = file.read()
        chars = "\\`*_{}[]()>#+-.!$/<?,\"\':;&%0123456789"
        for c in chars:
            data = data.lower().replace(c, "")

    return data


def levenstain_distance(word1, word2):
    m, n = len(word1), len(word2)
    matrix = [[0] * (n + 1) for _ in range(m + 1)]

    for i in range(m + 1):
        matrix[i][0] = i

    for j in range(n + 1):
        matrix[0][j] = j

    for i in range(1, m + 1):
        for j in range(1, n + 1):
            cost = 0 if word1[i - 1] == word2[j - 1] else 1
            matrix[i][j] = min(
                matrix[i - 1][j] + 1,
                matrix[i][j - 1] + 1,
                matrix[i - 1][j - 1] + cost
            )
    
    return matrix[m][n]

def count_frequency(data):
    words = data.split()
    frequency = {}
    for word in words:
        if word in frequency:
            frequency[word] += 1
        else:
            frequency[word] = 1

    return frequency

def generate_variants(word):
    variants = set()
    alphabet = "aábcčdďeéěfghiíjklmnňoópqrřsštťuúůvwxyýzž"
    
    for i in range(len(word) + 1):
        for letter in alphabet:
            variants.add(word[:i] + letter + word[i:])
    
    for i in range(len(word)):
        variants.add(word[:i] + word[i+1:])
    
    for i in range(len(word)):
        for letter in alphabet:
            variants.add(word[:i] + letter + word[i+1:])
    
    for i in range(len(word) - 1):
        variants.add(word[:i] + word[i+1] + word[i] + word[i+2:])
    
    return variants


def correct_word(word, frequency):
    variants = generate_variants(word)

    valid_variants = {w: frequency[w] for w in variants if w in frequency}
    
    if valid_variants:
        return max(valid_variants, key=valid_variants.get)
    
    return word

def closest_word(word, frequency):
    min_distance = float("inf")
    best_match = word

    for dict_word in frequency.keys():
        distance = levenstain_distance(word, dict_word)
        if distance < min_distance:
            min_distance = distance
            best_match = dict_word

    return best_match

def correct_sentence(sentence, frequency, method):
    words = sentence.split()

    if method == "levenstain":
        corrected_words = [closest_word(word, frequency) for word in words]
    elif method == "generate":   
        corrected_words = [correct_word(word, frequency) for word in words]

    return " ".join(corrected_words)

data = load_data("data\\text.txt") 
frequency = count_frequency(data)

sentence = "dneska si dám oběť v restauarci a pak půjdu zpěť domů kde se podívám na televezí."

start1 = time.time()
corrected1 = correct_sentence(sentence, frequency, "levenstain")
end1 = time.time()
levenstain_time = end1 - start1

start2 = time.time()
corrected2 = correct_sentence(sentence, frequency, "generate")
end2 = time.time()
generate_time = end2 - start2

print(f"Levenstain: {corrected1}: {levenstain_time:.4f}s")
print(f"Generated: {corrected2}: {generate_time:.4f}s")