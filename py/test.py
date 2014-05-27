from main import Symbol, ItemLine


s1 = Symbol('x', 1)
s2 = Symbol('x', 1)

print s1 in [s2]

print [1, s1] == [s2, 1]