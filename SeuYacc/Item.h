#ifndef _ITEM_H_
#define _ITEM_H_

#pragma once

#include <vector>
#include "ItemLine.h"

using namespace std;

class Item
{
public:
    vector<ItemLine> item_lines; // ItemLine�ļ���
	int item_id; // Item��Ӧid

    Item(void);
	Item(int id);
    ~Item(void);
    
    bool equal(const Item &other) const; // �Ƚ�����Item�Ƿ���ȣ������е�ItemLine���
    void insert_item_line(ItemLine &i); // ����һ���µ�ItemLine
	void set_item_lines(const vector<ItemLine> &ils); // ����ItemLine
};

#endif // _ITEM_H_