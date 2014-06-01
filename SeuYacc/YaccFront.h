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
    ifstream readDefFile;	//�ļ���������ָ���ķ������ļ�
    ofstream outputFile;	//�ļ��������ָ������ļ���һ����

    struct LeftOrRightOperator {
        int priority;	//���ȼ�
        char leftOrRight;	//����ҽ��
        string name;	//����
    };	//�������Ľṹ��
    vector<LeftOrRightOperator> allOperators;	//�洢���в�����

    vector<string> allTokens;	//�洢����token

    struct production {
        string name;	//����ʽ��
        vector <string> items;	//����ʽ�Ҳ���ÿ��С��
    };
    vector<production> allProductions;	//�洢���в���ʽ

    void readFromDefFile(char* filename);	//���ļ����붨��ĺ���
    string cstr2String(char*);	//��C����char*ת����string

    YaccFront(void);
    ~YaccFront(void);
};

#endif // _YACCFRONT_H_