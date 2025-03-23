def load_data(file_path):
    with open(file_path, "r", encoding="utf-8") as file:
        data = file.read()
        chars = "\\`*_{}[]()>#+-.!$/<?,\"\':;&%0123456789"
        for c in chars:
            data = data.replace(c, "")

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
        word = word.lower()
        if word in frequency:
            frequency[word] += 1
        else:
            frequency[word] = 1

    return frequency


print(count_frequency(load_data("data\\OpenSubtitles.cs-en.cs")))