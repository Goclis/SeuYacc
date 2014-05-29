#ifndef _YACCMANAGER_H_
#define _YACCMANAGER_H_

#pragma once

#include <vector>
#include <string>
#include "Item.h"
#include "Symbol.h"
#include "Production.h"

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

    void set_productions(vector<Production> &ps);
    void set_symbols(vector<Symbol> &ss);
    void test_run();
private:
    vector<Production> productions;
    vector<Symbol> symbols;
    vector<Item> items;

    bool is_symbol_in_first_set(const vector<Symbol> &s, const Symbol &i);
    void merge_two_first_set(vector<Symbol> &s1, const vector<Symbol> &s2);
    vector<Symbol> remove_epsilon(const vector<Symbol> &s);
};

#endif // _YACCMANAGER_H_