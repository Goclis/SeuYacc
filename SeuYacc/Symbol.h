#ifndef _SYMBOL_H_
#define _SYMBOL_H_

#pragma once

#include <string>
using namespace std;

class Symbol
{
public:
    /* 方便起见，变量全public */
    
    string value; // 值
    // 文法符号的类型，0为epsilon，1为terminal，2为nonterminal
    int type; 
    
    Symbol();
    Symbol(string v, int t);
    Symbol(const Symbol &);
    ~Symbol(void);

    bool equal(const Symbol &other) const; // 比较两个Symbol是否相等
};

#endif // _SYMBOL_H_