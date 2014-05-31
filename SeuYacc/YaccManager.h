#ifndef _YACCMANAGER_H_
#define _YACCMANAGER_H_

#pragma once

#include <vector>
#include <string>
#include <hash_map>
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
    
    Item closure(Item &item);
    Item _goto(Item &item, Symbol &symbol);
    vector<Symbol> first(Symbol &symbol);
    vector<Symbol> first_beta_a(vector<Symbol> &symbols);
    void generate_items();
    void generate_parsing_table();
    void fix_conflict();

    void set_productions(vector<Production> &ps);
    void set_symbols(vector<Symbol> &ss);
    void set_priorities(map<string, Priority> &ps);
    void test_run();
private:
    vector<Production> productions;
    vector<Symbol> symbols;
    vector<Item> items;
    vector<map<string, int>> goto_table;
    vector<map<string, string>> action;
    map<string, Priority> priorities;

    bool is_symbol_in_first_set(const vector<Symbol> &s, const Symbol &i);
    void merge_two_first_set(vector<Symbol> &s1, const vector<Symbol> &s2);
    vector<Symbol> remove_epsilon(const vector<Symbol> &s);
    bool is_item_line_in_item(const ItemLine &il, const vector<ItemLine> &vil);
    Item is_item_exist(const Item &i);
    string string_concat_int(string &head, int i);
    vector<string> string_split(string &s, char delim);
};

#endif // _YACCMANAGER_H_