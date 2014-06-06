﻿#include "YaccManager.h"
#include "YaccFront.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <queue>

using namespace std;

YaccManager::YaccManager(void)
{
}

YaccManager::~YaccManager(void)
{
}

/************ Helper Function Begin **************/

/*
 * 检查某个文法符号是否已在一个文法符号集合中
 * @ss : vector<Symbol> 文法符号集合
 * @s : Symbol 待检查文法符号
 * @return : bool
 */
bool YaccManager::is_symbol_in_first_set(const vector<Symbol> &ss, const Symbol &s)
{
    for (size_t i = 0; i < ss.size(); i++) {
        if (ss.at(i).equal(s)) {
            return true;
        }
    }

    return false;
}

/*
 * 合并两个first集合，求并集，结果保存在第一个中
 * @s1 : vector<Symbol>
 * @s2 : vector<Symbol>
 * @return : void
 */
void YaccManager::merge_two_first_set(vector<Symbol> &s1, const vector<Symbol> &s2)
{
    for (size_t i = 0; i < s2.size(); i++) {
        Symbol ri = s2.at(i);
        if (!is_symbol_in_first_set(s1, ri)) {
            s1.push_back(ri);
        }
    }
}

/*
 * 去除epsilon
 * @s : vector<Symbol>
 * @return : vector<Symbol>
 */
vector<Symbol> YaccManager::remove_epsilon(const vector<Symbol> &s)
{
    vector<Symbol> result;
    for (size_t i = 0; i < s.size(); i++) {
        if (s.at(i).type != 0) {
            result.push_back(s.at(i));
        }
    }

    return result;
}

/*
 * 判断某个ItemLine是否在一个ItemLine集合中
 * @il : ItemLine
 * @vil : vector<ItemLine>
 * @return : bool
 */
bool YaccManager::is_item_line_in_item(const ItemLine &il, const vector<ItemLine> &vil)
{
    for (size_t i = 0; i < vil.size(); i++) {
		if (il.equal(vil.at(i))) {
			return true;
		}
    }

	return false;
}

/*
 * 判断某个Item是否已存在于items中
 * @i : Item
 * @return : Item 已存在则返回存在的Item，否则返回id为-1的Item
 */
Item YaccManager::is_item_exist(const Item &it)
{
    for (size_t i = 0; i < items.size(); i++) {
        if (it.equal(items.at(i))) {
            return items.at(i);
        }
    }
    
    return Item(-1);
}

/*
 * 把string和int拼接起来
 * @head : string
 * @i : int
 * @return : string
 */
string YaccManager::string_concat_int(const string &head, int i)
{
    stringstream sstm;
    sstm << head << i;
    return sstm.str();
}

/*
 * string.split
 * @s : string
 * @delim : char 分隔符
 * @return : vector<string>
 */
vector<string> YaccManager::string_split(const string &s, char delim)
{
    stringstream sstm(s);
    string item;
    vector<string> result;
    while (getline(sstm, item, delim)) {
        result.push_back(item);
    }
    return result;
}

/*
 * 判断某个Symbol是否已存在（于symbols中）
 * @s : Symbol
 * @return : bool
 */
bool YaccManager::is_symbol_exsit(const Symbol &s)
{
    return is_symbol_in_first_set(symbols, s);
}

/*
 * 打印ItemLine，格式为 [S'->·S, $]
 * @il : ItemLine
 * @return : void
 */
void YaccManager::print_item_line(const ItemLine &il)
{
	int dot_pos = il.dot_pos;
	Production p = productions[il.pid];
	cout << p.left.value << " -> ";
	vector<Symbol> right = p.right;
	for (size_t j = 0; j < right.size(); j++) {
		if (j == dot_pos) {
			cout << ". ";
		}
		cout << right.at(j).value << " ";
	}
	if (right.size() == dot_pos) {
		cout << ".";
	}
	cout << ", " << il.lookahead.value << endl;
}

/*
 * 打印Item，格式如下:
 * Item item_id
 * print all ItemLine(s)
 * @item : Item
 * @return : void
 */
void YaccManager::print_item(const Item &item)
{
	cout << "Item " << item.item_id << endl;
	cout << "--------------------------" << endl;
	vector<ItemLine> ils = item.item_lines;
	for (size_t k = 0; k < ils.size(); k++) {
		ItemLine tmp_rs = ils.at(k);
		print_item_line(tmp_rs);
	}
	cout << "--------------------------" << endl;
}

/************ Helper Function End **************/

/*
 * 求单个文法符号的first集合
 * 此方法不处理左递归，针对左递归，假设其不存在epsilon
 * 如存在A->Aa，则不存在A->epsilon，针对A->Aa这样的产生式，直接忽略
 * @symbol : Symbol
 * @return : vector<Symbol>
 */
vector<Symbol> YaccManager::first(const Symbol &symbol)
{
    if (first_set.count(symbol.value) != 0) {
        // 已经缓存过了，直接返回
        return first_set[symbol.value];
    }

	int s_type = symbol.type;
	vector<Symbol> result;

	if (s_type == 2) { // nonterminal
		// 遍历一遍产生式，将可添加的先添加了（右部为空或开头为终结符的）
		for (size_t i = 0; i < productions.size(); i++) {
			Production current_production = productions.at(i);

			if (current_production.left.equal(symbol)) { // 产生式左部为symbol
				vector<Symbol> right = current_production.right;

				if (right.size() == 0) {
					if (!is_symbol_in_first_set(result, Symbol("epsilon", 0))) {
						result.push_back(Symbol("epsilon", 0));
					}
				} else if (right.at(0).type == 1) { // 开头是非终结符
					if (!is_symbol_in_first_set(result, right.at(0))) {
						result.push_back(right.at(0));
					}
				}
			}
		}

		// 再遍历一遍，只管右部开头为非终结符的
		for (size_t i = 0; i < productions.size(); i++) {
			Production current_production = productions.at(i);

			if (current_production.left.equal(symbol)) { // 产生式左部为symbol
				vector<Symbol> right = current_production.right;

				if (right.size() != 0 && right.at(0).type == 2) {
					vector<Symbol> before_first;
					// 遍历right，扩充result
					for (size_t ri = 0; ri < right.size(); ++ri) {
						Symbol current_right = right.at(ri);

						if (current_right.equal(symbol)) { // 遇到待求的Symbol
							if (!is_symbol_in_first_set(result, Symbol("epsilon", 0))) {
								// 不存在epsilon，后面的无法继续了
								break;
							} else {
								before_first = result;
							}
						} else if (current_right.type == 1) { // 遇到终结符
							if (is_symbol_in_first_set(before_first, Symbol("epsilon", 0))) {
								// 之前的那个符号的first集中有epsilon
								if (!is_symbol_in_first_set(result, current_right)) {
									result.push_back(current_right); // 这是最后一个
								}
							} 

							break; // 终结符不管什么情况都该break了
						} else { // 非终结符
							if (ri == 0) {
								before_first = first(current_right);
								if (ri == right.size() - 1) {
									// 末尾了，如果当前非终结符有epsilon，则左部也包含，不需要过滤
									merge_two_first_set(result, before_first);
								} else {
									merge_two_first_set(result, remove_epsilon(before_first));
								}
							} else if (is_symbol_in_first_set(before_first, Symbol("epsilon", 0))) {
								// 之前的那个符号的first集中有epsilon
								before_first = first(current_right); // 更新before_first集

								if (ri == right.size() - 1) {
									// 末尾了，如果当前非终结符有epsilon，则左部也包含，不需要过滤
									merge_two_first_set(result, before_first);
								} else {
									merge_two_first_set(result, remove_epsilon(before_first));
								}
							} else {
								break;
							}
						}
					}

				}
			}
		}
	} else {
		result.push_back(symbol);
	}
    
    // 缓存新生成的first
    first_set[symbol.value] = result;
	return result;
}

/*
 * 求一串文法符号的first集合
 * @symbols : vector<Symbol>
 * @return : vector<Symbol>
 */
vector<Symbol> YaccManager::first_beta_a(const vector<Symbol> &symbols)
{
	vector<Symbol> result, before_first;

	// 遍历symbols中的每一个符号
	for (size_t i = 0; i < symbols.size(); i++) {
		Symbol current_symbol = symbols.at(i);
		int type = current_symbol.type;

		if (i == 0) {
			// 第一个元素特殊处理
			before_first = first(current_symbol);
			merge_two_first_set(result, remove_epsilon(before_first)); // 因为返回的集合肯定不包含epsilon，所以直接过滤掉
		} else {
			if (type == 1) {
				// 检查before_first中是否有epsilon
				if (is_symbol_in_first_set(before_first, Symbol("epsilon", 0))) {
					if (!is_symbol_in_first_set(result, current_symbol)) {
						result.push_back(current_symbol); // 如果result中不存在则压入
					}
				} 
				break; // 无论有没有都结束（因为是终结符）
			} else {
				// 非终结符的处理
				// 检查before_first是否有epsilon
				if (is_symbol_in_first_set(before_first, Symbol("epsilon", 0))) {
					// 存在则更新before_first
					before_first = first(current_symbol);
					merge_two_first_set(result, remove_epsilon(before_first));
				} else {
					break;
				}
			}
		}
	}

	return remove_epsilon(result);
}

/*
 * 求闭包
 * @item : Item
 * @return : Item
 */
Item YaccManager::closure(Item &item)
{
    vector<ItemLine> item_lines = item.item_lines;
    queue<ItemLine> item_line_queue;
    for (size_t i = 0; i < item_lines.size(); i++) {
        item_line_queue.push(item_lines.at(i));
    }

    while (!item_line_queue.empty()) {
        ItemLine current = item_line_queue.front();
		item_line_queue.pop();

        int dot_pos = current.dot_pos;
        Production current_production = productions.at(current.pid);
        vector<Symbol> right = current_production.right;
        int new_dot_pos = 0;
        Symbol symbol_after_dot;
        // 检查dot_pos是否在末尾了
        if (dot_pos < (int) right.size()) {
            symbol_after_dot = right.at(dot_pos);
            new_dot_pos = dot_pos + 1;
        } else {
            continue;
        }

        int type = symbol_after_dot.type;
        if (type == 2) {
            // 该symbol之后的所有符号
            vector<Symbol> symbols_after_dot(
                right.begin() + new_dot_pos, right.end());
            symbols_after_dot.push_back(current.lookahead);
            
            // 求first_beta_a
            vector<Symbol> first_rs = first_beta_a(symbols_after_dot);
            
            // 遍历产生式，寻找左部为symbol_after_dot的
            for (size_t i = 0; i < productions.size(); i++) {
                Production iterate_p = productions.at(i);
                if (iterate_p.left.equal(symbol_after_dot)) {
                    // 遍历first_beta_a集合，添加ItemLine
                    for (size_t j = 0; j < first_rs.size(); j++) {
                        ItemLine new_item_line(iterate_p.pid, 0, first_rs.at(j));
                        // 检查是否已存在于Item中
                        if (!is_item_line_in_item(new_item_line, item_lines)) {
							item_lines.push_back(new_item_line);
							item_line_queue.push(new_item_line);
                        }
                    }
                }
            }
        }
    }

	item.item_lines = item_lines;
	return item;
}

/*
 * goto，根据Symbol求下一个Item
 * @item : Item
 * @symbol : Symbol
 * @return : Item
 */
Item YaccManager::_goto(const Item &item, const Symbol &symbol)
{
    vector<ItemLine> item_lines = item.item_lines;
    Item new_Item((int) items.size());

    for (size_t i = 0; i < item_lines.size(); i++) {
        ItemLine current_item_line = item_lines.at(i);
        int dot_pos = current_item_line.dot_pos;
        vector<Symbol> right = productions[current_item_line.pid].right;

        // dot边界检查
        if (dot_pos < (int) right.size() && right[dot_pos].equal(symbol)) {
            ItemLine new_item_line(current_item_line.pid, dot_pos + 1, current_item_line.lookahead);
            new_Item.insert_item_line(new_item_line);
        }
    }
    
    // TODO:这里可以加一步判断减少一次无用的闭包操作（针对空的item_lines）
    return closure(new_Item);
}

/*
 * 生成所有的项目集（Item），结果保存至items
 * 同时保存goto信息
 * @return : void
 */
void YaccManager::generate_items()
{
    queue<Item> item_queue;
    Item start_item(0);
    start_item.insert_item_line(ItemLine(0, 0, Symbol("$", 1)));
    start_item = closure(start_item);
    items.push_back(start_item);
    item_queue.push(start_item);
	
	cout << "Generating Items\n============================" << endl;
	// print Item 0
	print_item(start_item);

    while (!item_queue.empty()) {
        Item current = item_queue.front();
        item_queue.pop();

        for (size_t si = 0; si < symbols.size(); si++) {
            Item next_item = _goto(current, symbols.at(si));

            if (next_item.item_lines.size()) { // item_lines不为空，说明生成了Item
                Item check = is_item_exist(next_item); // 检查Item是否已存在

                int t_id; // goto table使用的id
                if (check.item_id == -1) {
                    items.push_back(next_item);
                    item_queue.push(next_item);
                    t_id = next_item.item_id;
					
					// log new item
					cout << "New Item Get\n";
					print_item(next_item);

                } else {
                    t_id = check.item_id;
                }

                // 更新goto_table
                int s_id = current.item_id;
                if (s_id >= (int) goto_table.size()) {
                    // 容量不够，扩容
                    goto_table.resize(s_id + 1);
                }
				goto_table[s_id][symbols.at(si).value] = t_id;

				// log goto
				cout << "Item " << s_id << " -> Item " << t_id << " through [" << symbols.at(si).value << "]\n"; 
            }
        }
    }
}

/*
 * 生成解析表，保存至action中，这里不解决冲突
 * @return : void
 */
void YaccManager::generate_parsing_table()
{
	cout << "\n\n\nGenerating Parsing Table\n=======================" << endl;
    action.resize(items.size()); // 设置action的大小，和items一样
    // 遍历items，生成action
    for (size_t item_index = 0; item_index < items.size(); item_index++) {
        Item current_item = items.at(item_index);
        int item_id = current_item.item_id;
        vector<ItemLine> item_lines = current_item.item_lines;
        
        // 遍历ItemLine
        for (size_t item_line_i = 0; item_line_i < item_lines.size(); item_line_i++) {
            ItemLine current_item_line = item_lines.at(item_line_i);
            Production p = productions.at(current_item_line.pid);
            int dot_pos = current_item_line.dot_pos;
            
            Symbol symbol_after_dot;
            if (dot_pos < (int) p.right.size()) {
                // 点不在末尾
                symbol_after_dot = p.right[dot_pos];
            } else {
                symbol_after_dot.value = "";
            }

            if (symbol_after_dot.value.empty()) { // 点在最后了
                if (current_item_line.pid == 0) { // [S'->S·, $]
                    action[item_id]["$"] = "acc";
					// log 
					cout << "Output action[" << item_id << "][$]=\"acc\" by ItemLine ";
					print_item_line(current_item_line);
                    break; // 这个Item没必要继续了（只可能有一个）
                } else {
                    const char *key = current_item_line.lookahead.value.c_str();

                    string before_action = action[item_id][current_item_line.lookahead.value];
                    string current_action = string_concat_int(string("r"), current_item_line.pid);
                    
                    if (before_action.empty()) {
                        before_action = current_action;
						cout << "Output action[" << item_id << "][\"" << current_item_line.lookahead.value
							<< "\"]=" << before_action << " by ItemLine ";
						print_item_line(current_item_line);
                    } else if (before_action.find(current_action.c_str()) != -1) {
                        // 已经存在此动作
                    } else {
                        before_action += "|" + current_action;
						cout << "[Update]Output action[" << item_id << "][\"" << current_item_line.lookahead.value
							<< "\"]=" << before_action << " by ItemLine ";
						print_item_line(current_item_line);
                    }

                    action[item_id][current_item_line.lookahead.value] = before_action;
                }
            } else if (symbol_after_dot.type == 1) {  // terminal
                int target = goto_table[item_id][symbol_after_dot.value];
                
                string before_action = action[item_id][symbol_after_dot.value];
                string current_action = string_concat_int(string("s"), target);
                
                if (before_action.empty()) {
                    before_action = current_action;
					cout << "Output action[" << item_id << "][\"" << symbol_after_dot.value
						<< "\"]=" << before_action << " by ItemLine ";
					print_item_line(current_item_line);
                } else if (before_action.find(current_action.c_str()) != -1) {
                    // 已经存在此动作
                } else {
                    before_action += "|" + current_action;
					cout << "[Update]Output action[" << item_id << "][\"" << symbol_after_dot.value
						<< "\"]=" << before_action << " by ItemLine ";
					print_item_line(current_item_line);
                }

                action[item_id][symbol_after_dot.value] = before_action;
            }
        }
    }
}

/*
 * 根据定义的优先级解决冲突
 * @return : void
 */
void YaccManager::fix_conflict()
{
	cout << "\n\n\nFix conflict\n===========================" << endl;

    // 遍历action，找到有冲突的项
    size_t l = items.size();
    for (size_t i = 0; i < l; i++) {
        map<string, string> ca = action.at(i);

        map<string, string>::iterator it = ca.begin();
        for (; it != ca.end(); ++it) {
            string current_action = it->second;

            if (current_action.find("|") != -1) { // 存在冲突
				cout << "Find conflict: action[" << i << "][\"" << it->first
					<< "\"]=" << it->second << endl;

                int conflict_item_id = i;
                string conflict_symbol_value = it->first;
                vector<string> conflict_actions = string_split(current_action, '|');
                
                int pid;
                int target_item_id;
                // 假设只会存在两种冲突。。 移近和规约冲突
                // 遍历查找冲突的产生式的id
                for (size_t ai = 0; ai < conflict_actions.size(); ++ai) {
                    string as = conflict_actions.at(ai);
                    if (as.find("r") != -1) {
                        string r = as.substr(as.find("r") + 1);
                        pid = atoi(r.c_str());
                    } else if (as.find("s") != -1) {
                        string s = as.substr(as.find("s") + 1);
                        target_item_id = atoi(s.c_str());
                    }
                }

                Production p = productions.at(pid);
                vector<Symbol> right = p.right;
                Symbol symbol_inside("epsilon", 0); // 产生式中的终结符                
                // 从后往前找symbol_inside
                int index = right.size() - 1;
                while (index >= 0) {
                    Symbol current = right.at(index);
                    if (current.type == 1) {
                        symbol_inside = current;
                        break;
                    }
                    --index;
                }

                // 查询内外的优先级，根据优先级解决冲突
				Priority inside_priority, outside_priority;
				map<string, Priority>::iterator it;
				it = priorities.find(symbol_inside.value);
				if (it != priorities.end()) {
					inside_priority = it->second;
				} else {
					inside_priority = Priority(-1, -1);
				}
				it = priorities.find(conflict_symbol_value);
				if (it != priorities.end()) {
					outside_priority = it->second;
				} else {
					outside_priority = Priority(-1, -1);
				}
                
                if (symbol_inside.value == "epsilon") {
                    // A->·，此类暂不处理 = =#不知道怎么弄优先级
					cout << "Ignored\n" << endl;
                } else {
                    if (inside_priority.level > outside_priority.level) { // 内部优先级高
                        // 规约
                        action[conflict_item_id][conflict_symbol_value] 
                            = string_concat_int(string("r"), pid);

						cout << "Level(" << symbol_inside.value << ") > Level("
							<< conflict_symbol_value << "), select " << string_concat_int(string("r"), pid) << "\n" <<endl;
                    } else if (inside_priority.level < outside_priority.level) { // 外部优先级高
                        // 移进
                        action[conflict_item_id][conflict_symbol_value]
                            = string_concat_int(string("s"), target_item_id);

						cout << "Level(" << symbol_inside.value << ") < Level("
							<< conflict_symbol_value << "), select " << string_concat_int(string("s"), target_item_id) << "\n" <<endl;
                    } else {
                        // 这里假设两个符号结合律相同
                        if (inside_priority.lr == 0) {
                            // 左结合，规约
                            action[conflict_item_id][conflict_symbol_value] 
                                = string_concat_int(string("r"), pid);

							cout << symbol_inside.value << " " << conflict_symbol_value 
								<< " is same Level, left combination, so select " 
								<< string_concat_int(string("r"), pid) << "\n" <<endl;
                        } else {
                            // 右结合，移进
                            action[conflict_item_id][conflict_symbol_value]
                                = string_concat_int(string("s"), target_item_id);

							cout << symbol_inside.value << " " << conflict_symbol_value  
								<< " is same Level, right combination, so select " 
								<< string_concat_int(string("s"), target_item_id) << "\n" <<endl;
                        }
                    }
                }
            }
        }
    }
}

/*
 * 调用YaccFront进行解析，并转换出来
 */
void YaccManager::convert_from_front_to_manager(char *filename)
{
    YaccFront yf;
    yf.readFromDefFile(filename); // 读取解析

    // 转换
    // 将allTokens转换为Symbol里的terminal
    vector<string> tokens = yf.allTokens;
    for (size_t i = 0; i < tokens.size(); i++) {
        symbols.push_back(Symbol(tokens.at(i), 1));
    }
    // 将allOperators转换成priorities
    vector<YaccFront::LeftOrRightOperator> operators = yf.allOperators;
    for (size_t i = 0; i < operators.size(); ++i) {
        YaccFront::LeftOrRightOperator current = operators.at(i);
        Priority p;
        p.lr = (current.leftOrRight == 'l' ? 0 : 1);
        p.level = current.priority;

        priorities[current.name] = p;
    }

    vector<YaccFront::production> ps = yf.allProductions;
    // 添加第一个产生式 E'->E
    YaccFront::production p0 = ps.at(0);
    Symbol p0l = Symbol("S'", 2);
    vector<Symbol> right0;
    right0.push_back(Symbol(p0.name, 2));
    productions.push_back(Production(0, p0l, right0));
    symbols.push_back(p0l);
    // 遍历所有产生式添加至productions，并将非终结符加至symbols
    for (size_t i = 0; i < ps.size(); ++i) {
        YaccFront::production current = ps.at(i);
        vector<string> cr = current.items;
        Symbol left = Symbol(current.name, 2);
        if (!is_symbol_exsit(left)) {
            symbols.push_back(left);
        }
        vector<Symbol> right;
        // 遍历production的右部，找寻非终结符，并同时将其加至right
        if (cr.size() == 0) {
            // epsilon，直接留空
            // right.push_back(Symbol("epsilon", 0));
        } else {
            for (size_t ri = 0; ri < cr.size(); ++ri) {
                Symbol maybe_new(cr.at(ri), 1); // 先假设为终结符
                if (is_symbol_exsit(maybe_new)) {
                    // 存在该终结符（如果是则一定存在，因为所有终结符已经存在于symbols中）
                    right.push_back(maybe_new);
                    continue;
                }

                maybe_new.type = 2; // 为非终结符
                if (!is_symbol_exsit(maybe_new)) {
                    // 不存在该非终结符，添加新的非终结符
                    symbols.push_back(maybe_new);
                }
                right.push_back(maybe_new);
            }
        }
        productions.push_back(Production(productions.size(), left, right));
    }
}

void YaccManager::generate_code()
{
	ofstream of("out.cpp", ios::app);
	streambuf *cout_buf = cout.rdbuf();
	streambuf *file_buf = of.rdbuf();
	cout.rdbuf(file_buf);
	
	cout << "int main() \n" <<
		"{\n";
	
	cout << "vector<map<string, string>> action(" << action.size() << ");\n";
 	cout << "map<string, string> actionItem;\n";
 	for (size_t i = 0; i < action.size(); i++) {
 		//if (!action.at(i).empty()) {
 			map<string, string> ca = action.at(i);
 			//cout << "map<string, string> actionItem;\n";
 			// cout << "action.push_back(actionItem);\n";
 			map<string, string>::iterator it = ca.begin();
 			for (; it != ca.end(); ++it) {
 				cout << "action.at(" << i << ")[\"" << it->first << "\"] = \"" << it->second << "\";" << endl;
 			}
 		//}
 	}
 	cout << "vector<map<string, int>> goto_table(" << goto_table.size() << ");\n";
 	cout << "map<string, int> gotoItem;\n";
 	for (size_t i = 0; i < goto_table.size(); ++i) {
 		//if (!goto_table.at(i).empty()) {
 			map<string, int> line = goto_table.at(i);
 			//cout << "map<string, int> gotoItem;\n";
 			// cout << "goto_table.push_back(gotoItem);\n";
 			map<string, int>::iterator it = line.begin();
 			for (; it != line.end(); ++it) {
				if (!is_symbol_exsit(Symbol(it->first, 1))) {
					cout << "goto_table.at(" << i << ")[\"" << it->first << "\"] = " << it->second << ";" << endl;
				}
 			}
 		//}
 	}
	
	// production left and right
	stringstream output1, output2;
	vector<string> production_left;
	output1 << "vector<string> production_left;\n";
	vector<int> production_right_symbol_nums;
	output2 << "vector<int> production_right_symbol_nums;\n";
	// 遍历productions，保存相应值到vector中
	for (size_t i = 0; i < productions.size(); ++i) {
		Production current = productions.at(i);
		output1 << "production_left.push_back(\"" << current.left.value << "\");\n";
		output2 << "production_right_symbol_nums.push_back(" << current.right.size() << ");\n";
	}
	cout << output1.str() << output2.str();

 	string code = "";
 	code = code +
 		/* token序列文件的读入流 */
 		"ifstream dotTknFile;\n"
 		"dotTknFile.open(\"token.tkn\", ios::in);\n"
 		"\n"
 		/* PDA的相关结构和初始化 */
 		"vector<string> symbolStack;\n"
 		"symbolStack.push_back(\"$\");\n"
 		"vector<int > stateStack;\n"
 		"stateStack.push_back(0);\n"
 		"\n"
 		/* 用于解析.tkn文件的一些cstring, 包括读头 */
 		"int buffersize = 512;\n"
 		"char *tmpCstr = new char[buffersize];\n"
 		"char *readPointer = new char[buffersize];\n"
 		"\n"
 		/* 开始读入.tkn文件, 按照.tkn定义,读头应该是每行的第二个元素 */
 		"dotTknFile.getline(tmpCstr, buffersize);\n"
 		"char *splitChar = \" \"; \n"
 		"readPointer = strtok(tmpCstr, splitChar);\n"
 		"readPointer = strtok(NULL, splitChar);\n"
 		"\n"
 		/* 具体的自动机过程 */
 		"while(strcmp(tmpCstr, \"$\") != 0 || !symbolStack.empty()) {\n"
 		"	int currentState = stateStack.at(stateStack.size() - 1);	\n"
 		/* goto操作 */
 		/*"	if(goto_table.at(currentState)[readPointer] != NULL) {\n"
 		"		stateStack.push_back(goto_table.at(currentState)[readPointer][readPointer]);\n"
 		"	}\n"*/
 		/* action操作 */
 		"if (action.at(currentState)[readPointer] != \"\") {\n"
 		/* 得到action的sn或rn的n(int) */
 		"		stringstream ss;\n"
 		"		string actionStr;\n"
 		"		int production_num;\n"
 		"		for (int i = 1; i < action.at(currentState)[readPointer].size(); ++i) {\n"
 		"			actionStr += action.at(currentState)[readPointer][i];\n"
 		"		}\n"
 		"		int actionNum;\n"
 		"		ss << actionStr;\n"
 		"		ss >> actionNum;\n"
 		/* 对于sn的移入操作,即状态转换,符号进栈,读头前进 */
 		"		if(action.at(currentState)[readPointer][0] == 's') {\n"
 		"			stateStack.push_back(actionNum);\n"
 		"			symbolStack.push_back(readPointer);\n"
 		"			dotTknFile.getline(tmpCstr, buffersize);\n"
 		"			readPointer = strtok(tmpCstr, splitChar);\n"
 		"			readPointer = strtok(NULL, splitChar);\n"
 		"		}\n"
 		/* 对于rn的规约操作,即状态出栈,符号出栈,产生式左部进栈 */
 		"		else if (action.at(currentState)[readPointer][0] == 'r') {\n"
 		"			for (int i = 0; i < production_right_symbol_nums[actionNum]; ++i) {\n"
 		"				symbolStack.pop_back();\n"
 		"				stateStack.pop_back();\n"
 		"			}\n"
 		"			symbolStack.push_back(production_left[actionNum]);\n"
		"			stateStack.push_back(goto_table.at(stateStack.at(stateStack.size() - 1))[symbolStack.at(symbolStack.size() - 1)]);\n"
 		"		}\n"
 		/* accept */
 		"		else if (action.at(currentState)[readPointer][0] == 'a') {\n"
 		"			cout << \"accept\" << endl;\n"
 		"			return 1;\n"
 		"		}\n"
 		/* 不是上面的情况则出错 */
 		"		else {\n"
 		"			cout << \"error\" << endl;\n"
 		"			return -1;\n"
 		"		}\n"
 		"	}\n"
 		/* 不是上面的情况则出错 */
 		"	else {\n"
 		"		cout << \"error\" << endl;\n"
 		"		return -1;\n"
 		"	}\n"
 		"}\n";
 	cout << code;

	cout << "}";
}

/*
 * 总的驱动函数，负责完成从YaccFront的转换并调用各方法
 */
void YaccManager::run()
{
    ofstream of("generate_log.log");
    streambuf *cout_buf = cout.rdbuf(); // backup
    log_file = of.rdbuf();
    cout.rdbuf(log_file);

    convert_from_front_to_manager("GrammarDefinition2.y");
    generate_items();
    generate_parsing_table();
    fix_conflict();
    generate_code();

    of.close();
    log_file = NULL;
    cout.rdbuf(cout_buf);
    cout << "Finished" << endl;
}

/*
 * 设置产生式
 * @ps : vector<Production>
 * @return : void
 */
void YaccManager::set_productions(const vector<Production> &ps)
{
    this->productions = ps;
}

/*
 * 设置文法符号
 * @ss : vector<Symbol>
 * @return : void
 */
void YaccManager::set_symbols(const vector<Symbol> &ss)
{
    this->symbols = ss;
}

/*
 * 设置优先级
 * @ps : map<string, Priority>
 * @return : void
 */
void YaccManager::set_priorities(const map<string, Priority> &ps)
{
    priorities = ps;
}

/* 测试用例 */
void YaccManager::test_run()
{
    // test first_beta_a
//     vector<Symbol> rs = first_beta_a(productions.at(0).right);
// 
//     for (size_t i = 0; i < rs.size(); i++) {
//         Symbol crs = rs.at(i);
// 
//         cout << crs.value << " ";
//     }
// 
//     cout << endl;

    // test closure
//     Item start_item(0);
//     vector<ItemLine> item_lines;
//     item_lines.push_back(ItemLine(0, 0, Symbol("$", 1)));
//     start_item.item_lines = item_lines;
//     start_item = closure(start_item);
//     items.push_back(start_item);
// 
//     Item result = _goto(start_item, symbols.at(1));
// 
//     vector<ItemLine> rs_ils = result.item_lines;
//     for (size_t i = 0; i < rs_ils.size(); i++) {
//         ItemLine tmp_rs = rs_ils.at(i);
//         Production p = productions[tmp_rs.pid];
//         cout << p.left.value << " -> ";
//         vector<Symbol> right = p.right;
//         for (size_t j = 0; j < right.size(); j++) {
//             cout << right.at(j).value << "";
//         }
//         cout << ", " << tmp_rs.dot_pos << ", " << tmp_rs.lookahead.value << endl;
//     }

    // test Item.equal
//     Item start_item(0);
//     vector<ItemLine> item_lines;
//     item_lines.push_back(ItemLine(0, 0, Symbol("$", 1)));
//     start_item.item_lines = item_lines;
//     start_item = closure(start_item);
//     items.push_back(start_item);
	
	ofstream of("generate_log.log");
	streambuf *cout_buf = cout.rdbuf(); // backup
	log_file = of.rdbuf();
	cout.rdbuf(log_file);
    
	convert_from_front_to_manager("GrammarDefinition2.y");
	// convert_from_front_to_manager("test.y");
    // test generate_items
    generate_items();

    generate_parsing_table();
    fix_conflict();
	
	generate_code();
	
	of.close();
	log_file = NULL;
	cout.rdbuf(cout_buf);
	cout << "Finished" << endl;
	
}
