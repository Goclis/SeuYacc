#ifndef _SYMBOL_H_
#define _SYMBOL_H_

#pragma once

#include <string>
using namespace std;

class Symbol
{
public:
    /* �������������ȫpublic */
    
    string value; // ֵ
    // �ķ����ŵ����ͣ�0Ϊepsilon��1Ϊterminal��2Ϊnonterminal
    int type; 
    
    Symbol();
    Symbol(string v, int t);
    Symbol(const Symbol &);
    ~Symbol(void);

    bool equal(const Symbol &other) const;
};

#endif // _SYMBOL_H_