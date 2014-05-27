# -*- coding:utf-8 -*-

from Queue import Queue

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

# 文法符号，包含terminal，nonterminal和epsilon
class Symbol(object):
	def __init__(self, s_value, s_type):
		self.value = s_value  # Symbol值 : string TODO:可以考虑维护一个Symbol list，然后这里保存id
		self.type = s_type  # Symbol类型 : int  0为epsilon，1为terminal，2为nonterminal

	def __eq__(self, other):
		return self.value == other.value \
			and self.type == other.type

# 产生式
class Production(object):
	def __init__(self, pid, left, right):
		self.pid = pid  # 产生式的ID : int
		self.left = left  # 产生式左部 : Symbol
		self.right = right  # 产生式右部序列 : list<Symbol>


productions = []  # 所有的产生式
symbols = []  # 所有的文法符号
items = []  # 所有的状态机
goto_table = {}
action = {}

# 求item的闭包（扩展item）
# @item : Item
# @return 返回扩展后的item（有可能没改变）
def closure(item):
	global productions

	item_lines = item.item_lines  # a list of item_line

	item_line_queue = Queue()  # 用于跟踪的队列
	for i in item_lines:
		item_line_queue.put_nowait(i)

	while not item_line_queue.empty():
		# 弹出一个ItemLine  [A -> alpha·B beta, a]
		current_item_line = item_line_queue.get_nowait()

		dot_pos = current_item_line.dot_pos
		current_production = productions[current_item_line.pid]
		right = current_production.right
		# TODO：需要添加检查dot_pos是否越界
		if dot_pos < len(right):
			symbol_after_dot = right[dot_pos] # 点之后的那个Symbol，不需要+1
			new_dot_pos = dot_pos + 1
		else:
			continue

		if symbol_after_dot.type == 2:  # nonterminal
			# 获取该symbol之后所有的symbol
			symbols_after_dot = right[new_dot_pos : ]
			symbols_after_dot.append(current_item_line.lookahead) # beta a

			# 求First
			# TODO: 实现first_beta_a
			first = first_beta_a(symbols_after_dot)

			# 遍历所有产生式，找头部为symbol_after_dot的产生式
			for p in productions:
				if p.left == symbol_after_dot:
					# 对该产生式，生成新的ItemLine
					for f in first:
						new_item_line = ItemLine(p.pid, 0, f)
						# 将新的ItemLine加入队列并更新至item中
						# TODO：检查该ItemLine是否已经存在
						if not new_item_line in item_lines:
							item_lines.append(new_item_line)
							item_line_queue.put_nowait(new_item_line)

		else:
			# terminal或者epsilon不用处理
			pass

	item.item_lines = item_lines

	return item

# 求item通过symbol能到达的下一个item
# @item : Item
# @symbol : Symbol
# @return 直接返回生成的item，即使为空
def goto(item, symbol):
	global items, productions
	new_item = Item(len(items))  # 新建一个Item

	item_lines = item.item_lines
	for i in item_lines:
		dot_pos = i.dot_pos
		right = productions[i.pid].right

		# TODO: dot_pos越界检查
		if dot_pos < len(right) and right[dot_pos] == symbol:
			# 新的ItemLine
			new_item_line = ItemLine(i.pid, dot_pos + 1, i.lookahead)
			new_item.insert_item_line(new_item_line)

	# TODO: 这里可以加一步判断减少一次无用的闭包操作（针对空的item_lines）
	return closure(new_item)

# 生成所有的Item
def generate_items():
	global items, symbols, goto_table

	item_queue = Queue()

	# 将第一个产生式[S->S']的闭包加进items
	start_production = productions[0]
	start_item = Item(0)
	start_item.insert_item_line(
		ItemLine(start_production.pid, 0, Symbol('$', 1)))
	start_item = closure(start_item)
	items.append(start_item)
	item_queue.put_nowait(start_item)

	while not item_queue.empty():
		current_item = item_queue.get_nowait()  # 弹出一个Item
		for s in symbols:
			next_item = goto(current_item, s)
			# 检查next_item是否为空（即无条目）以及items中是否已经存在，这里应该要自己写判断函数
			if next_item.item_lines:
				tmp = is_item_exist(next_item)
				if not tmp:
					items.append(next_item)
					item_queue.put_nowait(next_item)
				else:
					next_item = tmp
				
				# 添加到goto表中
				s_id = current_item.item_id
				
				if s_id not in goto_table.keys():
					goto_table[s_id] = {}

				goto_table[s_id][s.value] = next_item.item_id  # 建立边

	# items生成完毕。。。

# 生成ACTION和GOTO
def generate_parsing_table():
	global items, action, productions, symbols, goto_table

	for i in items:
		item_id = i.item_id
		if item_id not in action.keys():
			action[item_id] = {}

		item_lines = i.item_lines
		for l in item_lines:
			p = productions[l.pid]
			dot_pos = l.dot_pos

			if dot_pos < len(p.right):
				symbol_after_dot = p.right[dot_pos]  # 在点后面的符号
			else:
				symbol_after_dot = ''
			
			if not symbol_after_dot:  # 点在最后了
				if l.pid == 0:  # [S'->S·, $]
					action[item_id]['$'] = 'acc'
					break  # 针对这个item没必要继续了
				else:
					# TODO：解决移入规约冲突
					before_action = action[item_id].get(l.lookahead.value, '')
					current_action = 'r' + str(l.pid)
					if before_action:
						# 已经存在了
						if before_action == current_action:  # 相同
							pass
						else:
							if before_action.__class__ == ''.__class__: # 字符串
								action[item_id][l.lookahead.value] = [
									before_action, current_action
								]
							else:
								if current_action not in before_action:
									before_action.append(current_action)
									action[item_id][l.lookahead.value] = before_action
					else:
						action[item_id][l.lookahead.value] = current_action  # 规约
			elif symbol_after_dot.type == 1:  # terminal
				current_item_goto = goto_table.get(item_id, '')
				if current_item_goto:
					next_item_id = current_item_goto.get(symbol_after_dot.value, '')
					if next_item_id:
						# 收集移入规约冲突
						before_action = action[item_id].get(symbol_after_dot.value, '')
						current_action = 's' + str(next_item_id)
						if before_action:
							# 已经存在了
							if before_action == current_action:  # 相同
								pass
							else:
								if before_action.__class__ == ''.__class__: # 字符串
									action[item_id][symbol_after_dot.value] = [
										before_action, current_action
									]
								else:
									if current_action not in before_action:
										before_action.append(current_action)
										action[item_id][symbol_after_dot.value] = before_action
						else:
							action[item_id][symbol_after_dot.value] = 's' + str(next_item_id)  # 移入
			


# 求一个符号串的first集合
# @symbols : list<Symbol> 符号串
# @return : list<Symbol> first集合
def first_beta_a(symbols):
	result = []

	for i in range(len(symbols)):
		current_symbol = symbols[i]

		if current_symbol.type == 0:
			return ['epsilon']
		elif current_symbol.type == 1:
			# check 0 ~ i-1
			for j in range(i):
				if 'epsilon' not in first(symbols[i]):
					return result
			result.append(current_symbol)
			return result
		else:
			# check 0 ~ i-1
			for j in range(i):
				if 'epsilon' not in first(symbols[i]):
					return result
			result.extend(first(current_symbol))

	return result

# 求单个符号的first集合
# 对存在左递归的非终结符，不允许其存在epsilon，即不能有A->Ac | e (e为epsilon)
# @symbol : Symbol 单个符号
# @reutn : list<Symbol> first集合
# TODO：添加对所有非终结符的first集合的临时缓存，若存在直接使用
def first(symbol):
	global productions

	result = []
	# 遍历产生式
	for production in productions:
		if production.left == symbol:
			right_list = production.right

			if right_list[0] == symbol:  # 直接左递归，直接忽略
				pass
			else:
				result.extend(first_beta_a(right_list))

	return result

def print_items():
	global items, productions

	for i in items:
		lines = i.item_lines

		for l in lines:
			p = productions[l.pid]
			print p.left.value, [j.value for j in p.right], l.dot_pos, l.lookahead.value

		print '\n'

# 检查该Item是否已存在
# @return 存在则返回已存在的item，否则返回False
def is_item_exist(item):
	global items

	for i in items:
		if i == item:
			return i

	return False


def print_item(item):
	lines = item.item_lines
	lines.sort()
	for l in lines:
		p = productions[l.pid]
		print p.left.value, [j.value for j in p.right], l.dot_pos, l.lookahead.value

def test():
	global productions, symbols, items, goto_table, action

	# nt_Sp = Symbol("S'", 2)
	# nt_S = Symbol('S', 2)
	# nt_C = Symbol('C', 2)
	# t_c = Symbol('c', 1)
	# t_d = Symbol('d', 1)

	# symbols = [
	# 	nt_Sp,
	# 	nt_S,
	# 	nt_C,
	# 	t_c,
	# 	t_d
	# ]

	# productions = [
	# 	Production(0, nt_Sp, [nt_S]),
	# 	Production(1, nt_S, [nt_C, nt_C]),
	# 	Production(2, nt_C, [t_c, nt_C]),
	# 	Production(3, nt_C, [t_d])
	# ]

	nt_Ep = Symbol("E'", 2)
	nt_E = Symbol('E', 2)
	t_plus = Symbol('+', 1)
	t_mul = Symbol('*', 1)
	t_left = Symbol('(', 1)
	t_right = Symbol(')', 1)
	t_id = Symbol('i', 1)

	symbols = [
		nt_Ep,
		nt_E,
		t_plus,
		t_mul,
		t_left,
		t_right,
		t_id
	]

	productions = [
		Production(0, nt_Ep, [nt_E]),
		Production(1, nt_E, [nt_E, t_plus, nt_E]),
		Production(2, nt_E, [nt_E, t_mul, nt_E]),
		Production(3, nt_E, [t_left, nt_E, t_right]),
		Production(4, nt_E, [t_id])
	]

	generate_items()

	print_items()

	generate_parsing_table()
	print goto_table, '\n'
	print action

test()