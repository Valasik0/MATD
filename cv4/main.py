from collections import defaultdict
from query_parser import BooleanQueryParser
#Boolean Information Retrieval – Invertovaný index a dotazy
stop_words = set(["i", "he", "she", "is", "a", "some", "for", "the", "to", "it", "this", "of", "and", 'or', 'on', 'in', 'at', 'an', 'as', 'by', 'with', 'that', 'from', 'about', 'be', 'are', 'was', 'were', 'has', 'have', 'had', 'not', 'but', 'if', 'then'])

def load_and_process_data():
    inverted_index = defaultdict(lambda: defaultdict(int))
    for i in range(1,51):
        with open(f"data\\{i}.txt", "r", encoding="utf-8") as file:
            text = file.read()
            chars = "\\`*_{}[]()>#+-.!$/<?,\"\':;&%0123456789\n\r"
            for c in chars:
                text = text.lower().replace(c, "")
            words = text.split()
            words = [word for word in words if word not in stop_words]
            for word in words:
                inverted_index[word][i] += 1
    return inverted_index

query = "milk AND king AND (needels OR sugar) AND (NOT salad AND queen)"
parser = BooleanQueryParser(query, load_and_process_data())
result = parser.parse()

print(f"Query: {query}")
print(f"Result: {result}")