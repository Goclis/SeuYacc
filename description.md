# Yacc


##使用简介
###外部使用
当前Yacc默认读取同目录下的 grammar_definition.y 这个文法定义文件，并生成 generate_log.log（日志文件，包含每一步执行）和 syntax_analyzer.cpp（用于解析目标文法的程序）

###内部调用结构
构造了一个YaccManager实例，调用其run方法，run方法依次执行如下方法

 - convert_from_front_to_manager：负责读取文件并将文件中定义的内容转化成YaccManager中的对象
 - generate_items：负责生成所有的LR(1)项
 - generate_parsing_table：生成解析表
 - fix_conflict：解决解析表中存在的冲突
 - generate_code：生成目标代码（syntax_analyzer.cpp）

日志文件随着程序运行而扩充

###日志文件结构
主要分为三块

 - 生成items时的日志
 - 生成action时的日志
 - 解决冲突的日志

####生成items时的日志
生成一个新的Item时打印该Item，并且记录其goto状况，如下
```
# 本例表示Item 0 通过 int 产生了 Item 1

New Item Get
Item 1
--------------------------
type -> int ., IDENTIFIER
--------------------------
Item 0 -> Item 1 through [int]
```

####生成action的日志
主要有两种形式，一种为新增的，即原先该行列不存在值，如下
```
# 本例表示由于 [type -> ·int, IDENTIFIER] 生成了动作 s1

Output action[0]["int"]=s1 by ItemLine type -> . int , IDENTIFIER
```
另一种即原先存在，新产生了动作，即冲突发生，如下
```
# 本例表示由于 [arithmetic_expr -> arithmetic_expr . + arithmetic_expr , ;] 生成了新动作s72，和已有r39冲突

[Update]Output action[139]["+"]=r39|s72 by ItemLine arithmetic_expr -> arithmetic_expr . + arithmetic_expr , ;
```

####解决冲突的日志
格式如下
```
# 本例表示存在 / 和 - 的冲突，因为 / 的优先级高于 -，所以选择规约

Find conflict: action[292]["-"]=r40|s252
Level(/) > Level(-), select r40
```

##数据结构
###Symbol
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

###Production

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

###Priority

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

###ItemLine

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

###Item

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

###YaccManager

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

##算法简介
###first
求单个符号的first集合

基本是基于书上的first算法实现的，但是因为没有消除左递归，算法针对LR(1)文法做了特殊处理，并假设不存在间接左递归
```python
def first(symbol):
    result = []
    
    # 判断symbol的类型
    if symbol is nonterminal: # 非终结符，按以下步骤执行
        # 遍历一遍所有的产生式，找出直接结果
        for p in productions:
            if p.left == symbol: # 找出左部为symbol的产生式
                if len(p.right) == 0:  
                    # 右部为空，将epsilon添加到first集合中
                elif p.right[0] is terminal: 
                    # 右部开头为终结符，将该终结符加入first集合
                else:
                    pass # 非终结符放后面处理
                    
        # 再遍历一遍，筛选剩余的
        for p in productions:
            if p.left == symbol:
                if len(p.right) != 0 and p.right[0] is nonterminal: 
                    # 右部开头为非终结符
                    # 对整个右部的符号串求first，将所得集合合并至result中
                    # 这里有针对性的处理了 A->Aa 这样的左递归，具体如下
                    old_first = [] # 使用一个变量记录上一个符号的first结果
                    for r in p.right:
                        if r == symbol: # 当前符号等于所求symbol
                            # 判断上个符号的结果是否有epsilon
                            # 不存在则break，存在则将result赋给old_first
                        elif r is terminal: # 当前符号是非终结符
                            # 判断上个符号的结果是否有epsilon
                            # 存在则将当前符号加入result
                            break
                        else: # 当前符号是非终结符
                            # 判断上个符号的结果是否有epsilon
                            # 不存在则break，存在的话将当前符号的first合并到result中，并更新old_first
    else: # 终结符，first只包含自身
        result.append(symbol)
    return result
```

###first_beta_a
求一串符号的first集合

基本算法和书上所将类似，遍历该串的每个符号，并将沿途的符号的first集合合并到result中，直到某个符号的first集合中无法产生空，则终止返回。
```
def first_beta_a(symbols):
    result = []
    
    # 使用一个变量记录上一个符号的first结果，初始化为第一个符号的first集合
    old_first = first(symbols[0]) 
    # 遍历symbols，但跳过第一个
    for s in symbols:
        if s is terminal: # 当前符号为终结符
            # 判断上个符号的结果是否有epsilon
            # 存在则把当前符号加入result，不存在则直接结束
        else: # 当前符号为非终结符
            # 判断上个符号的结果是否有epsilon
            # 存在则将该符号的first集合加入result，并更新old_first
            
    return result
        
```

###closure
闭包算法，求某个item的闭包

算法同龙书
```
def closure(item):
    line_queue = Queue() # 使用队列来缓存item中未处理的行
    # 遍历item将已有行加入队列
    # ...
    
    while not line_queue.empty(): # 队列非空
        i = line_queue.pop() # 取出一个
        
        # 如果是 A->alpha·Bbeta, a
        first_set = first_beta_a(...) # 求i的first(beta a)
        # 遍历所得集合
        for s in first_set:
            # 将 B->·alpha, s 加入到item中，同时加入队列
            
    return item
```

###goto
根据某个项以及某个symbol产生新的项

算法同龙书给出
```python
def goto(item, symbol):
    new_item = Item()
    # 遍历item的每一行
    for i in item.lines:
        # 若i的点未到末尾，将i加入new_item
        
    return closure(new_item) # 求闭包并返回
```
###generate_items
生成所有的LR(1)项

算法来自龙书
```
def generate_items():
    item_queue = Queue() # 使用队列缓存未处理的item
    
    # 根据第一个产生式 S'->S 构造一行，加入start_item
    start_item = Item()
    start_item.insert([S'->S, $])
    items.insert(start_item) # 加入items结果集
    item_queue.push(start_item) # 加入队列
    
    while not item_queue.empty(): # 队列非空
        i = item_queue.pop() # 取出一个
        # 遍历所有symbol
        for s in symbols:
            ri = goto(i, s) # 求goto
            
            # 如果ri不存在于items结果集中，将其加入items和队列
            if ri not in items:
                items.insert(ri)
                item_queue.push(ri)
                
            # 同时更新goto_table
```

###generate_parsing_table
生成解析表

算法基于龙书
```
def generate_parsing_table():
    # 遍历所有的Item
    for i in items:
        # 遍历当前item的所有行
        for l in i:
            # 如果l是 [S'->S·, $]，将i所在行对应$的列赋值为acc
            # ...
            
            # 如果l是 [A->alpha·, a]，将i所在行对应a的列赋值为rX，其中X为相应产生式
            
            # 如果l是 [A->alpha·aBeta, s]，查询goto_table，获取目标item的id（假设为X），将i所在行对应a的列赋值为sX
            
            # 以上动作如果存在冲突则均保留
```

###fix_conflict
解决解析表中的冲突

基于优先级和左右结合进行解决，目前只能解决产生移近/规约冲突的文法的冲突
```
def fix_conflict():
    # 遍历action表，找其中存在冲突的地方
    for unit in action:
        if conflict in unit:
            # 找出冲突的两个符号
            a = ... # 从规约的产生式中获取
            b = ... # 待移近的符号
            
            # 比较两个符号优先级
            if a.level > b.level:
                # 规约
            elif a.level < b.level:
                # 移近
            else:
                # 优先级相同，根据左右结合来判断
                # 目前只能接解决同结合规则
                if a.lr = 'l': # 左结合
                    # 规约
                elif a.lr = 'r': # 右结合
                    # 移近
```
