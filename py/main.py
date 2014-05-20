from Queue import Queue

# Item中的单个条目
class ItemLine(object):
	def __init__(self, left, right, dot_pos, lookahead):
		self.left = left  # 产生式左部 : Symbol
		self.right = right  # 产生式右部序列 : list<Symbol>
		self.dot_pos = dot_pos  # 点在右部的位置 : int
		self.lookahead = lookahead  # 向前看的符号 : Symbol

		# dot_pos 示例
		# A -> ·aBc  dot_pos = 0
		# A -> a·Bc  dot_pos = 1
		# A -> aB·c  dot_pos = 2
		# A -> aBc·  dot_pos = 3，即dot_pos == len(ItemLine.right)时就已经没法继续goto了

# 状态机中的每个Item
class Item(object):
	def __init__(self, item_id):
		self.item_id = item_id  # 项id : int
		self.item_lines = []  # Item中包含的条目 : list<ItemLine>

	# 添加新的ItemLine
	def insert_item_line(self, item_line):
		self.item_lines.append(item_line)
		# TODO: 可以考虑修改成保存ID，将所有ItemLine抽取出来

# 文法符号，包含terminal，nonterminal和epsilon
class Symbol(object):
	def __init__(self, s_value, s_type):
		self.value = s_value  # Symbol值 : string TODO:可以考虑维护一个Symbol list，然后这里保存id
		self.type = s_type  # Symbol类型 : int  0为epsilon，1为terminal，2为nonterminal

# 产生式
class Production(object):
	def __init__(self, left, right):
		self.left = left  # 产生式左部 : Symbol
		self.right = right  # 产生式右部序列 : list<Symbol>


productions = []  # 所有的产生式
symbols = []  # 所有的文法符号
items = []  # 所有的状态机

# 求item的闭包（扩展item）
# @item : Item
# @return 返回扩展后的item（有可能没改变）
def closure(item):
	global productions

	item_lines = item['item_lines']  # a list of item_line

	item_line_queue = Queue()  # 用于跟踪的队列
	for i in item_lines:
		item_line_queue.put_nowait(i)

	while not item_line_queue.empty():
		# 弹出一个ItemLine  [A -> alpha·B beta, a]
		current_item_line = item_line_queue.get_nowait()

		dot_pos = current_item_line['dot_pos']
		right = current_item_line['right']
		# TODO：需要添加检查dot_pos是否越界
		symbol_after_dot = right[dot_pos] # 点之后的那个Symbol，不需要+1
		new_dot_pos = dot_pos + 1

		if symbol_after_dot['type'] == 'nonterminal':
			# 获取点之后所有的symbol
			symbols = right[dot_pos : ]
			symbols.append(current_item_line['lookahead']) # beta a

			# 求First
			# TODO: 实现first_beta_a
			first = first_beta_a(symbols)

			# 遍历所有产生式，找头部为symbol_after_dot的产生式
			for p in productions:
				if p['left'] == symbol_after_dot:
					# 对该产生式，生成新的ItemLine
					for f in first:
						new_item_line = ItemLine(symbol_after_dot, p['right'], new_dot_pos, f)
						# 将新的ItemLine加入队列并更新至item中
						# TODO：检查该ItemLine是否已经存在
						item_lines.append(new_item_line)
						item_line_queue.put_nowait(new_item_line)

		else:
			# terminal或者epsilon不用处理
			pass

	item['item_lines'] = item_lines

	return item

# 求item通过symbol能到达的下一个item
# @item : Item
# @symbol : Symbol
# @return 直接返回生成的item，即使为空
def goto(item, symbol):
	global items
	new_item = Item(len(items))  # 新建一个Item

	item_lines = item['item_lines']
	for i in item_lines:
		dot_pos = i['dot_pos']
		right = i['right']

		# TODO: dot_pos越界检查
		if right[dot_pos] == symbol:
			# 新的ItemLine
			new_item_line = ItemLine(i['left'], right, dot_pos + 1, i['lookahead'])
			new_item.insert_item_line(new_item_line)

	# TODO: 这里可以加一步判断减少一次无用的闭包操作（针对空的item_lines）
	return closure(new_item)

# 生成所有的Item
def generate_items():
	global items, symbols

	item_queue = Queue()

	# 将第一个产生式[S->S']的闭包加进items
	start_production = productions[0]
	start_item = Item(0)
	start_item.insert_item_line(
		ItemLine(start_production['left'], start_production['right'], 0, Symbol('$', 1)))
	items.append(start_item)
	item_queue.put_nowait(start_item)

	while not item_queue.empty():
		current_item = item_queue.get_nowait()  # 弹出一个Item

		for s in symbols:
			next_item = goto(current_item, s)
			# 检查next_item是否为空（即无条目）以及items中是否已经存在，这里应该要自己写判断函数
			if not next_item['item_lines'] and items.indexOf(next_item) == -1:
				items.append(next_item)
				item_queue.put_nowait(next_item)

	# items生成完毕。。。

# 求一个符号串的first集合
# @symbols : list<Symbol> 符号串
# @return : list<Symbol> first集合
def first_beta_a(symbols):
	pass

# 求单个符号的first集合
# @symbol : Symbol 单个符号
# @reutn : list<Symbol> first集合
def first(symbol):
	pass

# e = 'epsilon'

# #@x : 串
# def first_beta_a(x):
# 	global e

# 	result = []
# 	for i in range(len(x)):
# 		current = x[i]

# 		if current is 'terminal':
# 			# check 0 ~ i-1
# 			for j in range(i):
# 				if e not in first(x[j]):
# 					return result
# 			result.append(current)
# 			return result
# 		elif current is e:
# 			return [e]
# 		else:
# 			for j in range(i):
# 				if e not in first(x[j]):
# 					return result
# 			result.extend(first(current))

# 	return result


# # for single nonterminal
# def first(x):
# 	result = []
# 	# 遍历所有的产生式
# 	for production in productions:
# 		if production['left'] == x:
# 			right_list = production['right']


