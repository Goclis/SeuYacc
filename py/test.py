from main import Symbol, ItemLine


s1 = Symbol('x', 1)
s2 = Symbol('x', 1)

il1 = ItemLine(1, 0, s1)
il2 = ItemLine(1, 0, s1)

i1 = [il1, il1, il2]
i2 = [il2, il1, il1]

i1.sort()
i2.sort()
# i1.sort(key = lambda x: (x.pid, x.dot_pos, x.lookahead))
# i2.sort(key = lambda x: (x.pid, x.dot_pos, x.lookahead))

print i1 == i2