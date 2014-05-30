#ifndef _ITEM_H_
#define _ITEM_H_

#pragma once

#include <vector>
#include "ItemLine.h"

using namespace std;

class Item
{
public:
    vector<ItemLine> item_lines;
	int item_id;

    Item(void);
	Item(int id);
    ~Item(void);

	void set_item_lines(const vector<ItemLine> &ils);
};

#endif // _ITEM_H_