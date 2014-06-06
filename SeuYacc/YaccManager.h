#ifndef _YACCMANAGER_H_
#define _YACCMANAGER_H_

#pragma once

#include <vector>
#include <string>
#include <map>
#include "Item.h"
#include "Symbol.h"
#include "Production.h"
#include "Priority.h"

using namespace std;

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

    void generate_code();

    // driver
    void run();
    
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

#endif // _YACCMANAGER_H_