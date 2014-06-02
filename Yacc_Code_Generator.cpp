#include "Yacc_Code_Geneartor.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
int main(int  argc, char* argv[])
{
	string outStr = "";
	outStr = outStr +
	vector<map<string, int>> goto_table; // goto表
	vector<map<string, string>> action; // action表
	int state = 0;
	"outputFile.open(\"out.cpp\", ios::out);\n"
	"ifstream dotTknFile;\n"
	"dotTknFile.open(\"" + argv[1] + "\", ios::in);\n"
	"char *readPointer;\n"
	"vector<string> symbolStack;\n"
	"vector<string> symbolStack;\n"
	"symbolStack.push_back(\"$\");\n"
	"int buffersize = 512;\n"
	"char *tmpCstr = new char[buffersize];\n"
	"dotTknFile.getline(tmpCstr, buffersize);\n"
	"char *splitChar = \" \";\n" 
	"while(strcmp(tmpCstr, \"$\") != 0 && !symbolStack.empty()) {\n"
	"	readPointer = strtok(NULL, splitChar);\n"
		if(goto_table[state][readPointer] == NULL 

";


string output1, output2;
    vector<string> production_left;
    output1 = "vector<string> production_left;\n";
    vector<int> production_right_symbol_nums;
    output2 = "vector<int> production_right_symbol_nums;\n"
    // 遍历productions，保存相应值到vector中
    for (size_t i = 0; i < productions.size(); ++i) {
        Production current = productions.at(i);
        output1 += "production_left.push_back(" + current.left.value + ");\n";
        // production_left.push_back(current.left.value);
        //production_right_symbol_nums.push_back(current.right.size());
        output2 += "production_right_symbol_nums.push_back(" + current.right.size() + ");\n";
    }



    cout << "map<string, string> action;\n";
    for (size_t i = 0; i < l; i++) {
		map<string, string> ca = action.at(i);
		map<string, string>::iterator it = ca.begin();
		for (; it != ca.end(); ++it) {
			cout << "action[\"" << it->first << "\"] = " << it->second << ";" << endl;
		}
	}
	cout << "vector<map<string, int>> goto_table;\n" ;
	for (int  i = 0; i < goto_table.size(); ++i) {
		map<string, int> line = goto_table.at(i);
		map<string, int>::iterator it = line.begin();
		for(; it != line.end(); ++it) {
			cout << "goto_table[\"" << it->first << "\"] = " << it->second << ";" << endl;
		}
	}
}