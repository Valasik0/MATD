def brute_force_search(arr, target):
    positions = []
    comparisons = 0
    match = False
    for i in range(len(arr) - len(target) + 1):
        match = True
        for j in range(len(target)):
            comparisons += 1
            if arr[i + j] != target[j]:
                match = False
                break
        if match:
            positions.append(i)

    return {"positions": positions, "comparisons": comparisons}