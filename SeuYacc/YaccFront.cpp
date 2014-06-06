#define _CRT_SECURE_NO_WARNINGS	//这个宏定义是因为某些VC版本会对strtok报unsafe的错，加了就忽略了这个问题
#include "YaccFront.h"

YaccFront::YaccFront(void)
{
}

YaccFront::~YaccFront(void)
{
}

/*
 * 这个函数主要是读入按照.y的语法格式定义的文法文件
 * getline函数读取一行文件
 * strcmp比较两个字符串，如果完全相同则返回0
 * strtok以第二个参数分割字符串，返回所有到分隔符前的接受的字符，\
 * 同时有一个默认全局变量记录当前位置，所以可以用strtok(NULL,char*)
 */
void YaccFront::readFromDefFile(char* filename)
{
    char *tmpCstr = new char[BUFFERSIZE];
    string tmpString;
    readDefFile.open(filename, ios::in);
    outputFile.open("syntax_analyzer.cpp", ios::out);
    readDefFile.getline(tmpCstr, BUFFERSIZE);	//读取第一行，不做处理，默认为%}
    /* 将第一段直接输出到生成的文件 */
    readDefFile.getline(tmpCstr, BUFFERSIZE);
    while (strcmp(tmpCstr, "%}") !=  0) {
        outputFile << tmpCstr << endl;
        readDefFile.getline(tmpCstr, BUFFERSIZE);
    }
    /* 第二段，左右结合运算符和token */
    readDefFile.getline(tmpCstr, BUFFERSIZE);
    int priority = 0;
    while(strcmp(tmpCstr, "%%") != 0) {
        switch (tmpCstr[1]) {
            /* 如果是%token */
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
            /* 如果是%left */
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
            /* 如果是%right */
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
        memset(tmpCstr, 0, BUFFERSIZE);	//将字符串清零，不这么做出现了最后一个输出重复的情况
        readDefFile.getline(tmpCstr, BUFFERSIZE);
    }
    /* 第三段，表达式定义解析 */
    readDefFile.getline(tmpCstr, BUFFERSIZE);
    char *colon = ":", *semicolon = ";", *verticalBar = "|", *blank = " ";
    char *firstToken, *token;
    production tmpProduction;
    while (strcmp(tmpCstr, "%%") != 0) {
        firstToken = strtok(tmpCstr, blank);	//以第一个非空字符决定当行的属性
        /* 空行忽略 */
        if (firstToken == NULL) {
            readDefFile.getline(tmpCstr, BUFFERSIZE);
            continue;
        }
        /* 非竖线或分号的视作产生式的左部 */
        if (strcmp(firstToken, verticalBar) != 0 && strcmp(firstToken, semicolon) != 0) {
            tmpProduction.name = cstr2String(firstToken);
            token = strtok(NULL, blank);	//跳过冒号
            token = strtok(NULL, blank);
            while (token != NULL) {
                tmpProduction.items.push_back(token);
                token = strtok(NULL, blank);
            }
            allProductions.push_back(tmpProduction);
        }
        /* 竖线的保留产生式左部，清空右部项后读入新的项 */
        if (strcmp(firstToken, verticalBar) == 0) {
            tmpProduction.items.clear();
            token = strtok(NULL, blank);
            while (token != NULL) {
                tmpProduction.items.push_back(token);
                token = strtok(NULL, blank);
            }
            allProductions.push_back(tmpProduction);
        }
        /* 分号清除产生式左部，.y文件规范则下一行应该可以赋值新的左部 */
        if (strcmp(firstToken, semicolon) == 0) {
            tmpProduction.items.clear();
        }
        readDefFile.getline(tmpCstr, BUFFERSIZE);
    }
    /* 将第四段直接输出到生成的文件 */
    /*readDefFile.getline(tmpCstr, BUFFERSIZE);
    while (strcmp(tmpCstr, "") != 0) {
        outputFile << tmpCstr << endl;
        readDefFile.getline(tmpCstr, BUFFERSIZE);
    }*/
	outputFile.close();
    readDefFile.close();
}

string YaccFront::cstr2String(char* cstr)
{
    string str = cstr;
    return str;
}