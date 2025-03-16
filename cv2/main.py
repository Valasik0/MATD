def broot_force_search(arr, target):
    positions = []
    for i in range(len(arr) - len(target) + 1):
        for j in range(len(target)):
            if arr[i + j] != target[j]:
                break
        else:
            positions.append(i)

    return positions

