SeuYacc
=======

An easy yacc


### Class
__ItemLine__
```
# Item中的单个条目
class ItemLine(object):
    def __init__(self, pid, dot_pos, lookahead):
        self.pid = pid  # 对应产生式的id
        self.dot_pos = dot_pos  # 点在右部的位置 : int
        self.lookahead = lookahead  # 向前看的符号 : Symbol

        # dot_pos 示例
        # A -> ·aBc  dot_pos = 0
        # A -> a·Bc  dot_pos = 1
        # A -> aB·c  dot_pos = 2
        # A -> aBc·  dot_pos = 3，即dot_pos == len(ItemLine.right)时就已经没法继续goto了

    def __eq__(self, other):
        return self.pid == other.pid \
            and self.dot_pos == other.dot_pos \
            and self.lookahead == other.lookahead
```

__Item__
```
# 状态机中的每个Item
class Item(object):
    def __init__(self, item_id):
        self.item_id = item_id  # 项id : int
        self.item_lines = []  # Item中包含的条目 : list<ItemLine>

    # 添加新的ItemLine
    def insert_item_line(self, item_line):
        self.item_lines.append(item_line)
        # TODO: 可以考虑修改成保存ID，将所有ItemLine抽取出来

    def __eq__(self, other):
        s_i = self.item_lines
        o_i = other.item_lines

        s_i.sort(key = lambda x: (x.pid, x.dot_pos, x.lookahead.value))
        o_i.sort(key = lambda x: (x.pid, x.dot_pos, x.lookahead.value))

        if s_i == o_i:
            return True
        else:
            return False
```

__Production__
```
# 产生式
class Production(object):
    def __init__(self, pid, left, right):
        self.pid = pid  # 产生式的ID : int
        self.left = left  # 产生式左部 : Symbol
        self.right = right  # 产生式右部序列 : list<Symbol>
```

__Symbol__
```
# 文法符号，包含terminal，nonterminal和epsilon
class Symbol(object):
    def __init__(self, s_value, s_type):
        self.value = s_value  # Symbol值 : string TODO:可以考虑维护一个Symbol list，然后这里保存id
        self.type = s_type  # Symbol类型 : int  0为epsilon，1为terminal，2为nonterminal

    def __eq__(self, other):
        return self.value == other.value \
            and self.type == other.type
```