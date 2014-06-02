#define _CRT_SECURE_NO_WARNINGS	//����궨������ΪĳЩVC�汾���strtok��unsafe�Ĵ����˾ͺ������������
#include "YaccFront.h"

YaccFront::YaccFront(void)
{
}

YaccFront::~YaccFront(void)
{
}

/*
 * ���������Ҫ�Ƕ��밴��.y���﷨��ʽ������ķ��ļ�
 * getline������ȡһ���ļ�
 * strcmp�Ƚ������ַ����������ȫ��ͬ�򷵻�0
 * strtok�Եڶ��������ָ��ַ������������е��ָ���ǰ�Ľ��ܵ��ַ���\
 * ͬʱ��һ��Ĭ��ȫ�ֱ�����¼��ǰλ�ã����Կ�����strtok(NULL,char*)
 */
void YaccFront::readFromDefFile(char* filename)
{
    char *tmpCstr = new char[BUFFERSIZE];
    string tmpString;
    readDefFile.open(filename, ios::in);
    outputFile.open("out.cpp", ios::out);
    readDefFile.getline(tmpCstr, BUFFERSIZE);	//��ȡ��һ�У���������Ĭ��Ϊ%}
    /* ����һ��ֱ����������ɵ��ļ� */
    readDefFile.getline(tmpCstr, BUFFERSIZE);
    while (strcmp(tmpCstr, "%}") !=  0) {
        outputFile << tmpCstr << endl;
        readDefFile.getline(tmpCstr, BUFFERSIZE);
    }
    /* �ڶ��Σ����ҽ���������token */
    readDefFile.getline(tmpCstr, BUFFERSIZE);
    int priority = 0;
    while(strcmp(tmpCstr, "%%") != 0) {
        switch (tmpCstr[1]) {
            /* �����%token */
        case 't':
            {
                char *splitChar = " ";
                char *token;
                token = strtok(tmpCstr, splitChar);
                token = strtok(NULL, splitChar);
                while (token != NULL) {
                    allTokens.push_back(cstr2String(token));
                    token = strtok(NULL, splitChar);;
                }
                break;
            }
            /* �����%left */
        case 'l':
            {
                priority++;
                char *splitChar = " ";
                char *token;
                token = strtok(tmpCstr, splitChar);
                token = strtok(NULL, splitChar);
                while (token != NULL) {
                    LeftOrRightOperator lOperator;
                    lOperator.leftOrRight = 'l';
                    lOperator.name = cstr2String(token);
                    lOperator.priority = priority;
                    allOperators.push_back(lOperator);
                    token = strtok(NULL, splitChar);;
                }
                break;
            }
            /* �����%right */
        case 'r':
            {
                priority++;
                char *splitChar = " ";
                char *token;
                token = strtok(tmpCstr, splitChar);
                token = strtok(NULL, splitChar);
                while (token != NULL) {
                    LeftOrRightOperator rOpeartor;
                    rOpeartor.leftOrRight = 'r';
                    rOpeartor.name = cstr2String(token);
                    rOpeartor.priority = priority;
                    allOperators.push_back(rOpeartor);
                    token = strtok(NULL, splitChar);;
                }
                break;
            }
        default:
            break;
        }
        memset(tmpCstr, 0, BUFFERSIZE);	//���ַ������㣬����ô�����������һ������ظ������
        readDefFile.getline(tmpCstr, BUFFERSIZE);
    }
    /* �����Σ����ʽ������� */
    readDefFile.getline(tmpCstr, BUFFERSIZE);
    char *colon = ":", *semicolon = ";", *verticalBar = "|", *blank = " ";
    char *firstToken, *token;
    production tmpProduction;
    while (strcmp(tmpCstr, "%%") != 0) {
        firstToken = strtok(tmpCstr, blank);	//�Ե�һ���ǿ��ַ��������е�����
        /* ���к��� */
        if (firstToken == NULL) {
            readDefFile.getline(tmpCstr, BUFFERSIZE);
            continue;
        }
        /* �����߻�ֺŵ���������ʽ���� */
        if (strcmp(firstToken, verticalBar) != 0 && strcmp(firstToken, semicolon) != 0) {
            tmpProduction.name = cstr2String(firstToken);
            token = strtok(NULL, blank);	//����ð��
            token = strtok(NULL, blank);
            while (token != NULL) {
                tmpProduction.items.push_back(token);
                token = strtok(NULL, blank);
            }
            allProductions.push_back(tmpProduction);
        }
        /* ���ߵı�������ʽ�󲿣�����Ҳ��������µ��� */
        if (strcmp(firstToken, verticalBar) == 0) {
            tmpProduction.items.clear();
            token = strtok(NULL, blank);
            while (token != NULL) {
                tmpProduction.items.push_back(token);
                token = strtok(NULL, blank);
            }
            allProductions.push_back(tmpProduction);
        }
        /* �ֺ��������ʽ�󲿣�.y�ļ��淶����һ��Ӧ�ÿ��Ը�ֵ�µ��� */
        if (strcmp(firstToken, semicolon) == 0) {
            tmpProduction.items.clear();
        }
        readDefFile.getline(tmpCstr, BUFFERSIZE);
    }
    /* �����Ķ�ֱ����������ɵ��ļ� */
    readDefFile.getline(tmpCstr, BUFFERSIZE);
    while (strcmp(tmpCstr, "") != 0) {
        outputFile << tmpCstr << endl;
        readDefFile.getline(tmpCstr, BUFFERSIZE);
    }
    readDefFile.close();
}

string YaccFront::cstr2String(char* cstr)
{
    string str = cstr;
    return str;
}