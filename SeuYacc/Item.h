#ifndef _ITEM_H_
#define _ITEM_H_

#pragma once

#include <vector>
#include "ItemLine.h"

using namespace std;

class Item
{
public:
    vector<ItemLine> item_lines; // ItemLine的集合
	int item_id; // Item对应id

    Item(void);
	Item(int id);
    ~Item(void);
    
    bool equal(const Item &other) const; // 比较两个Item是否相等，即所有的ItemLine相等
    void insert_item_line(ItemLine &i); // 插入一个新的ItemLine
	void set_item_lines(const vector<ItemLine> &ils); // 设置ItemLine
};

#endif // _ITEM_H_