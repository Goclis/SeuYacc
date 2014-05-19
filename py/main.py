class Terminal(object):
	"""docstring for Terminal"""
	def __init__(self, arg):
		super(Terminal, self).__init__()
		self.arg = arg


class NonTerminal(object):
	"""docstring for NonTerminal"""
	def __init__(self, arg):
		super(NonTerminal, self).__init__()
		self.arg = arg


class Production(object):
	"""docstring for Production"""
	def __init__(self, arg):
		super(Production, self).__init__()
		self.arg = arg

class ItemLine(object):
	"""docstring for ItemLine"""
	def __init__(self, arg):
		super(ItemLine, self).__init__()
		self.arg = arg

class Item(object):
	"""docstring for Item"""
	def __init__(self, arg):
		super(Item, self).__init__()
		self.arg = arg


terminal_list = []  # a list of all terminals
nonterminal_list = []  # a list of all nonterminals
production_list = ["S'->S"]  # a list of all productions

def read_from_yy():
	# read file
	# ....

	# after read file
	global terminal_list, nonterminal_list, production_list
	terminal_list = []
	nonterminal_list = []
	production_list = []

	# use lists to generate LR(1) items

def first(x):
	pass

def goto(item, x):
	pass

def generate_items():
	pass

def closure(item):
	item_lines = item['item_lines']  # a list of item_line

	# 对每个 [A -> @·BC, a]
	for item_line in item_lines:
		dot_pos = item_line['dot_pos']  # 点的位置
		ele_after_dot = item_line[dot_pos + 1]  # 未处理末尾情况

		for production in production_list:
			left = production['left']  # 产生式左部

			if left == ele_after_dot:  # [B -> x]这类产生式
				left_eles = item_line[dot_pos + 2 : ]  # 除了ele_after_dot以外，在点后的所有
				first_list = first(left_eles)  # 求first(Ca)

				for f in first_list:
					new_item_line = ItemLine()  # [B -> ·x, f]
					item_lines.append(new_item_line)  # 加到item_lines的末尾，一会儿会遍历到它（建议使用队列）
