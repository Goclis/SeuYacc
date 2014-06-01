#ifndef _ITEMLINE_H_
#define _ITEMLINE_H_

#pragma once

#include "Symbol.h"

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

#endif // _ITEMLINE_H_