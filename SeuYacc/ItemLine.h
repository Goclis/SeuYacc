#ifndef _ITEMLINE_H_
#define _ITEMLINE_H_

#pragma once

#include "Symbol.h"

class ItemLine
{
public:
    int pid; // �����Ĳ���ʽ��id
    int dot_pos; // �����ڵ�λ�ã���dot_pos == pid.right.size()ʱ��ʾ����ĩβ�ˣ�
    Symbol lookahead; // ��ǰ�����ķ�����

    ItemLine(void);
	ItemLine(int p, int d, Symbol &look);
    ~ItemLine(void);

    bool equal(const ItemLine &other) const; // �ж�����ItemLine�Ƿ����
};

#endif // _ITEMLINE_H_