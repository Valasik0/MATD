def bmh_search(text, pattern):
    positions = []
    comparisons = 0
    n, m = len(text), len(pattern)
    
    if m == 0:
        return {"positions": [], "comparisons": 0}
    
    bad_char = {}
    for i in range(m-1):
        bad_char[pattern[i]] = m - 1 - i
    
    i = 0
    while i <= n - m:
        j = m - 1
        
        while j >= 0:
            comparisons += 1
            if text[i+j] != pattern[j]:
                break
            j -= 1
        
        if j == -1:
            positions.append(i)
            i += 1
        else:
            shift = bad_char.get(text[i + j], m)
            i += max(1, shift)
    
    return {"positions": positions, "comparisons": comparisons}