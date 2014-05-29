#ifndef _PRODUCTION_H_
#define _PRODUCTION_H_

#pragma once

#include "Symbol.h"
#include <vector>
using namespace std;

class Production
{
public:
    int pid;
    Symbol left;
    vector<Symbol> right;

    Production(int pid, Symbol &left, vector<Symbol> &right);
    ~Production(void);
};

#endif // _PRODUCTION_H_