SeuYacc
=======

An easy yacc

### C++ Class
__Symbol__

文法符号类
```
class Symbol
{
public:
    /* 方便起见，变量全public */
    string value; // 值
    int type; // 文法符号的类型，0为epsilon，1为terminal，2为nonterminal
    
    Symbol();
    Symbol(string v, int t);
    Symbol(const Symbol &);
    ~Symbol(void);

    bool equal(const Symbol &other) const; // 比较两个Symbol是否相等
};
```

__Production__

产生式类
```
class Production
{
public:
    int pid; // 产生式id
    Symbol left; // 产生式左部Symbol
    vector<Symbol> right; // 产生式右部Symbol序列

    Production(int pid, Symbol &left, vector<Symbol> &right);
    ~Production(void);
};
```

__Priority__

优先级定义类
```
class Priority
{
public:
    int level; // 优先级，越大越高
    int lr; // 左结合还是右结合，左0，右1

    Priority(void);
    Priority(int l, int lr);
    ~Priority(void);
};
```

__ItemLine__

Item中的行
```
class ItemLine
{
public:
    int pid; // 关联的产生式的id
    int dot_pos; // 点所在的位置（当dot_pos == pid.right.size()时表示点在末尾了）
    Symbol lookahead; // 向前看的文法符号

    ItemLine(void);
	ItemLine(int p, int d, Symbol &look);
    ~ItemLine(void);

    bool equal(const ItemLine &other) const; // 判断两个ItemLine是否相等
};
```

__Item__

项
```
class Item
{
public:
    vector<ItemLine> item_lines; // ItemLine的集合
	int item_id; // Item对应id

    Item(void);
	Item(int id);
    ~Item(void);
    
    bool equal(const Item &other) const; // 比较两个Item是否相等，即所有的ItemLine相等
    void insert_item_line(ItemLine &i); // 插入一个新的ItemLine
	void set_item_lines(const vector<ItemLine> &ils); // 设置ItemLine
};
```

__YaccManager__

整体驱动类，流程大致如下：

 - 建立一个YaccManager实体
 - 解析.y文件并转化为YaccManager中的成员（convert_from_front_to_manager）
 - 生成items（generate_items）
 - 生成parsing_table（generate_parsing_table）
 - 去除冲突（fix_conflict）
 - 生成目标代码（generate_code）
```
class YaccManager
{
public:
    YaccManager(void);
    ~YaccManager(void);
    
    // 读入.y文件进行解析
    void read_from_definition_file(char *filename);

    // 求闭包
    Item closure(Item &item);

    // goto
    Item _goto(const Item &item, const Symbol &symbol);

    // first(单个文法符号)
    vector<Symbol> first(const Symbol &symbol);

    // first(文法符号串)
    vector<Symbol> first_beta_a(const vector<Symbol> &symbols);

    // 生成所有项
    void generate_items();

    // 生成解析表，前提是已生成项
    void generate_parsing_table();

    // 解决已生成解析表中的冲突
    void fix_conflict();
    
    // 生成目标代码
    void generate_code();

    // driver，针对某个file
    void run(char *filename);
    
    // public setter and test_run
    void set_productions(const vector<Production> &ps);
    void set_symbols(const vector<Symbol> &ss);
    void set_priorities(const map<string, Priority> &ps);
    void test_run();
private:
    vector<Production> productions; // 产生式集合
    vector<Symbol> symbols; // 文法符号集合
    vector<Item> items; // 项集合
    vector<map<string, int>> goto_table; // goto表
    vector<map<string, string>> action; // action表
    map<string, Priority> priorities; // 优先级关系
    map<string, vector<Symbol>> first_set; // 缓存first，空间换时间
	streambuf *log_file; // 日志文件
    
    /* Helper Functions */

    // 判断某个文法符号是否在某文法符号集合中
    bool is_symbol_in_first_set(const vector<Symbol> &s, const Symbol &i);

    // 合并两个文法符号集合，做并操作
    void merge_two_first_set(vector<Symbol> &s1, const vector<Symbol> &s2);

    // 去除某文法符号集合中的epsilon符号
    vector<Symbol> remove_epsilon(const vector<Symbol> &s);

    // 判断某个ItemLine是否在某个Item中
    bool is_item_line_in_item(const ItemLine &il, const vector<ItemLine> &vil);

    // 判断某个Item是否已存在(于items中）
    Item is_item_exist(const Item &i);

    // 判断某个Symbol是否已存在（于symbols中）
    bool is_symbol_exsit(const Symbol &);

    // string + int
    string string_concat_int(const string &head, int i);

    // string.split(delim)
    vector<string> string_split(const string &s, char delim);

    // 从YaccFront转换到YaccManger
    void convert_from_front_to_manager(char *);

	// 打印ItemLine
	void print_item_line(const ItemLine &il);

	// 打印Item
	void print_item(const Item &item);
};
```


### Python Class
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
