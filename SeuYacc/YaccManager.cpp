#include "YaccManager.h"
#include <iostream>
#include <sstream>
#include <queue>

using namespace std;

YaccManager::YaccManager(void)
{
    // TODO: 动态扩展goto_table和action的大小，这里定死了。
    goto_table = vector<map<string, int>>(1000);
    action = vector<map<string, string>>(1000);
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
 * 判断某个Item是否已存在
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
string YaccManager::string_concat_int(string &head, int i)
{
    stringstream sstm;
    sstm << head << i;
    return sstm.str();
}

/************ Helper Function End **************/

/*
 * 求单个文法符号的first集合
 * 此方法不处理左递归，针对左递归，假设其不存在epsilon
 * 如存在A->Aa，则不存在A->epsilon，针对A->Aa这样的产生式，直接忽略
 * @symbol : Symbol
 * @return : vector<Symbol>
 */
vector<Symbol> YaccManager::first(Symbol &symbol)
{
    int s_type = symbol.type;
    vector<Symbol> result;
    
    if (s_type == 2) { // nonterminal
        // 遍历产生式
        for (size_t i = 0; i < productions.size(); i++) {
            Production current_production = productions.at(i);
            if (current_production.left.equal(symbol)) { // 产生式左部为symbol
                vector<Symbol> right = current_production.right;
                Symbol right0 = right.at(0);
                if (right0.equal(symbol)) {
                    // 类似A->Aa，忽略
                } else if (right0.type == 0) {
                    // epsilon
                    if (!is_symbol_in_first_set(result, right0)) {
                        result.push_back(right0);
                    }
                } else {
                    vector<Symbol> right_first = first_beta_a(right);  
                    merge_two_first_set(result, right_first);
                }
            }
        }
    } else {
        result.push_back(symbol);
    }

    return result;
}

/*
 * 求一串文法符号的first集合
 * @symbols : vector<Symbol>
 * @return : vector<Symbol>
 */
vector<Symbol> YaccManager::first_beta_a(vector<Symbol> &symbols)
{
    vector<Symbol> result;
    
    // 遍历symbols中的每一个符号
    for (size_t i = 0; i < symbols.size(); i++) {
        Symbol current_symbol = symbols.at(i);
        int type = current_symbol.type;

        if (type == 1) {
            // 检查 0 ~ i-1 是否有epsilon，只要有一个没有就直接返回result
            Symbol epsilon("epsilon", 0);
            for (size_t bs = 0; bs < i; bs++) {
                if (!is_symbol_in_first_set(first(symbols.at(bs)), epsilon)) {
                    return result;
                }
            }
            result.push_back(current_symbol);
            return remove_epsilon(result);
        } else if (type == 2) {
            // 检查 0 ~ i-1 是否有epsilon，只要有一个没有就直接返回result
            Symbol epsilon("epsilon", 0);
            for (size_t bs = 0; bs < i; bs++) {
                if (!is_symbol_in_first_set(first(symbols.at(bs)), epsilon)) {
                    return remove_epsilon(result);
                }
            }
            merge_two_first_set(result, first(current_symbol));
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
Item YaccManager::_goto(Item &item, Symbol &symbol)
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
                } else {
                    t_id = check.item_id;
                }

                // 更新goto_table
                int s_id = current.item_id;
//                 if (s_id < (int) goto_table.max_size() - 1) {
//                     // 容量不够，扩容
//                     goto_table.resize(goto_table.max_size() * 2);
//                 }
                goto_table[s_id][symbols.at(si).value] = t_id;
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
                    break; // 这个Item没必要继续了（只可能有一个）
                } else {
                    const char *key = current_item_line.lookahead.value.c_str();

                    string before_action = action[item_id][current_item_line.lookahead.value];
                    string current_action = string_concat_int(string("r"), current_item_line.pid);
                    
                    if (before_action.empty()) {
                        before_action = current_action;
                    } else if (before_action.find(current_action.c_str()) != -1) {
                        // 已经存在此动作
                    } else {
                        before_action += "|" + current_action;
                    }

                    action[item_id][current_item_line.lookahead.value] = before_action;
                }
            } else if (symbol_after_dot.type == 1) {  // terminal
                int target = goto_table[item_id][symbol_after_dot.value];
                
                string before_action = action[item_id][symbol_after_dot.value];
                string current_action = string_concat_int(string("s"), target);
                
                if (before_action.empty()) {
                    before_action = current_action;
                } else if (before_action.find(current_action.c_str()) != -1) {
                    // 已经存在此动作
                } else {
                    before_action += "|" + current_action;
                }

                action[item_id][symbol_after_dot.value] = before_action;
            }
        }
    }
}

/*
 * 设置产生式
 * @ps : vector<Production>
 * @return : void
 */
void YaccManager::set_productions(vector<Production> &ps)
{
    this->productions = ps;
}

/*
 * 设置文法符号
 * @ss : vector<Symbol>
 * @return : void
 */
void YaccManager::set_symbols(vector<Symbol> &ss)
{
    this->symbols = ss;
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

    

    // test generate_items
    generate_items();

    for (size_t i = 0; i < items.size(); i++) {
        Item ri = items.at(i);
        vector<ItemLine> rs_ils = ri.item_lines;

        for (size_t k = 0; k < rs_ils.size(); k++) {
             ItemLine tmp_rs = rs_ils.at(k);
             Production p = productions[tmp_rs.pid];
             cout << p.left.value << " -> ";
             vector<Symbol> right = p.right;
             for (size_t j = 0; j < right.size(); j++) {
                 cout << right.at(j).value << "";
             }
             cout << ", " << tmp_rs.dot_pos << ", " << tmp_rs.lookahead.value << endl;
        }

        cout << endl;
    }

    generate_parsing_table();
    
    size_t l = items.size();
    for (size_t i = 0; i < l; i++) {
        map<string, string> ca = action.at(i);

        map<string, string>::iterator it = ca.begin();
        for (; it != ca.end(); ++it) {
            cout << "(" << it->first << ", " << it->second << ")" << endl;
        }

        cout << endl;
    }
}
