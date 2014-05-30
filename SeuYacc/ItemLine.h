#ifndef _ITEMLINE_H_
#define _ITEMLINE_H_

#pragma once

#include "Symbol.h"

class ItemLine
{
public:
    int pid;
    int dot_pos;
    Symbol lookahead;

	bool equal(const ItemLine &other) const;

    ItemLine(void);
	ItemLine(int p, int d, Symbol &look);
    ~ItemLine(void);
};

#endif // _ITEMLINE_H_