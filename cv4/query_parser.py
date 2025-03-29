class BooleanQueryParser:
    def __init__(self, query, inverted_index):
        self.query = query
        self.inverted_index = inverted_index
        self.pos = 0
        self.tokens = self.tokenize(query)

    def tokenize(self, query):
        return query.replace('(', ' ( ').replace(')', ' ) ').split()

    def parse(self):
        return self.parse_expression()

    def parse_expression(self):
        result = self.parse_term()
        while self.pos < len(self.tokens):
            if self.tokens[self.pos] == 'AND':
                self.pos += 1
                result = result & self.parse_term()
            elif self.tokens[self.pos] == 'OR':
                self.pos += 1
                result = result | self.parse_term()
            else:
                break
        return result

    def parse_term(self):
        if self.pos >= len(self.tokens):
            return set()
        
        token = self.tokens[self.pos]
        self.pos += 1

        if token == '(':
            result = self.parse_expression()
            if self.pos < len(self.tokens) and self.tokens[self.pos] == ')':
                self.pos += 1
            return result
        elif token == 'NOT':
            return self.get_all_docs() - self.parse_term()
        else:
            return self.get_docs_for_term(token)

    def get_docs_for_term(self, term):
        return set(self.inverted_index.get(term, {}).keys())

    def get_all_docs(self):
        return set(doc_id for term_dict in self.inverted_index.values() for doc_id in term_dict.keys())