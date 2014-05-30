#ifndef _ITEM_H_
#define _ITEM_H_

#pragma once

#include <ItemLine.h>

class Item
{
public:
    vector<ItemLine> item_lines;

    Item(void);
    ~Item(void);
};

#endif // _ITEM_H_