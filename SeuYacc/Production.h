#ifndef _PRODUCTION_H_
#define _PRODUCTION_H_

#pragma once

#include "Symbol.h"
#include <vector>
using namespace std;

class Production
{
public:
    int pid; // 产生式id
    Symbol left; // 产生式左部Symbol
    vector<Symbol> right; // 产生式右部Symbol序列

    Production(int pid, Symbol &left, vector<Symbol> &right);
    ~Production(void);
};

#endif // _PRODUCTION_H_