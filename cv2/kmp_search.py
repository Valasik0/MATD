def kmp_search(text, pattern):
    def build_lps(p):
        lps = [0] * len(p)
        length = 0
        for i in range(1, len(p)):
            while length > 0 and p[i] != p[length]:
                length = lps[length-1]
            if p[i] == p[length]:
                length += 1
            lps[i] = length
        return lps

    positions = []
    comparisons = 0
    n, m = len(text), len(pattern)
    
    if m == 0:
        return {"positions": [], "comparisons": 0}
    
    lps = build_lps(pattern)
    i = j = 0
    
    while i < n:
        comparisons += 1
        if pattern[j] == text[i]:
            i += 1
            j += 1
            if j == m:
                positions.append(i - j)
                j = lps[j-1]
        else:
            if j != 0:
                j = lps[j-1]
            else:
                i += 1
    
    return {"positions": positions, "comparisons": comparisons}