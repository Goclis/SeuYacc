#ifndef _PRIORITY_H_
#define _PRIORITY_H_

#pragma once

class Priority
{
public:
    int level; // �ȼ���Խ��Խ��
    int lr; // ���ϻ����ҽ�ϣ���0����1

    Priority(void);
    Priority(int l, int lr);
    ~Priority(void);
};

#endif // _PRIORITY_H_
