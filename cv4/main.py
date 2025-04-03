from collections import defaultdict
from query_parser import BooleanQueryParser

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

def analyze_index(inverted_index):
    unique_tokens = len(inverted_index)
    avg_list_length = sum(len(v) for v in inverted_index.values()) / unique_tokens if unique_tokens > 0 else 0
    total_records = sum(len(docs) for docs in inverted_index.values())
    
    print(f"Počet unikátních tokenů: {unique_tokens}")
    print(f"Průměrná délka seznamů: {avg_list_length:.2f}")
    print(f"Celkový počet záznamů: {total_records}")

def search_interface(inverted_index):
    print("Pro ukončení zadejte 'exit' nebo 'quit'.")
    
    while True:
        query = input("\nZadejte dotaz: ")
        if query.lower() in ['exit', 'quit']:
            break
        
        try:
            parser = BooleanQueryParser(query, inverted_index)
            result = parser.parse()

            if result == "Nothing found":
                print("Nebyly nalezeny žádné dokumenty.")
                continue
            
            print(f"Nalezeno {len(result)} dokumentů:")
            for doc_id in result:
                try:
                    with open(f"data\\{doc_id}.txt", "r", encoding="utf-8") as file:
                        first_line = file.readline().strip()
                        print(f"- Dokument {doc_id}: {first_line[:100]}...")
                except FileNotFoundError:
                    print(f"- Dokument {doc_id}: [Soubor nenalezen]")
        except Exception as e:
            print(f"Chyba při zpracování dotazu: {e}")


inverted_index = load_and_process_data()
search_interface(inverted_index)

analyze_index(inverted_index)
query = "milk AND king AND (needels OR sugar) NOT (salad AND octopus)"

