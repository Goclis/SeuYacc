#ifndef _YACCFRONT_H_
#define _YACCFRONT_H_

#include <fstream>
#include <vector>
#define BUFFERSIZE 512

using namespace std;

#pragma once

class YaccFront
{
public:
    ifstream readDefFile;	//文件输入流，指向文法定义文件
    ofstream outputFile;	//文件输出流，指向输出文件的一部分

    struct LeftOrRightOperator {
        int priority;	//优先级
        char leftOrRight;	//左或右结合
        string name;	//名字
    };	//操作符的结构体
    vector<LeftOrRightOperator> allOperators;	//存储所有操作符

    vector<string> allTokens;	//存储所有token

    struct production {
        string name;	//产生式左部
        vector <string> items;	//产生式右部的每个小项
    };
    vector<production> allProductions;	//存储所有产生式

    void readFromDefFile(char* filename);	//从文件读入定义的函数
    string cstr2String(char*);	//将C风格的char*转换到string

    YaccFront(void);
    ~YaccFront(void);
};

#endif // _YACCFRONT_H_