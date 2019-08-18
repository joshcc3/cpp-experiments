

def solve(inp, N):
    mp = {}
    seen = set()
    start = 0
    result = (1, 0, 0)
    for ix, c in enumerate(inp):
        mp[c] = ix
        seen.add(c)
        if len(seen) > N:
            seen.remove(inp[start])
            start = mp[inp[start]] + 1
        if ix - start + 1 > result[0]:
            result = (ix - start + 1, start, ix)

    return inp[result[1]: result[2] + 1]


print(solve("uuttyytt", 2))


