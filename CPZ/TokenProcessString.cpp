/* token序列文件的读入流 */
"ifstream dotTknFile;\n"
"dotTknFile.open(\"" + argv[1] + "\", ios::in);\n"
"\n"
/* PDA的相关结构和初始化 */
"vector<string> symbolStack;\n"
"symbolStack.push_back(\"$\");\n"
"vector<int > stateStack;\n"
"stateStack.push_back(0);\n"
"\n"
/* 用于解析.tkn文件的一些cstring, 包括读头 */
"int buffersize = 512;\n"
"char *tmpCstr = new char[buffersize];\n"
"char *readPointer = new char[buffersize];\n"
"\n"
/* 开始读入.tkn文件, 按照.tkn定义,读头应该是每行的第二个元素 */
"dotTknFile.getline(tmpCstr, buffersize);\n"
"char *splitChar = \" \"; \n"
"readPointer = strtok(tmpCstr, splitChar);\n"
"readPointer = strtok(NULL, splitChar);\n"
"\n"
/* 具体的自动机过程 */
"while(strcmp(tmpCstr, \"$\") != 0 && !symbolStack.empty()) {\n"
"	int currentState = stateStack.at(stateStack.size() - 1);	\n"
/* goto操作 */
"	if(!goto_table[currentState][readPointer] == NULL) {\n"	
"		stateStack.push_back(goto_table[currentState][readPointer]);\n"
"	}\n"
/* action操作 */
"	else if (!action[currentState][readPointer] == NULL) {\n"	
/* 得到action的sn或rn的n(int) */
"		sstream ss;\n"
"		string reduction;\n"
"		int production_num;\n"
"		for (int i = 1; i < b.size(); ++i) {\n"
"			reduction += action[currentState][i];\n"
"		}\n"
"		ss << actionStr;\n"	
"		ss >> actionNum;\n"	
/* 对于sn的移入操作,即状态转换,符号进栈,读头前进 */
"		if(action[currentState][readPointer][0] == 's') {\n"
"			stateStack.push_back[actionNum];\n"
"			symbolStack.push_back(readPointer);\n"
"			dotTknFile.getline(tmpCstr, buffersize);\n"
"			readPointer = strtok(tmpCstr, splitChar);\n"
"			readPointer = strtok(NULL, splitChar);\n"
"		}\n"
/* 对于rn的规约操作,即状态出栈,符号出栈,产生式左部进栈 */
"		else if (action[currentState][readPointer][0] == 'r') {\n"
"			for (int i = 0; i < production_right_symbol_nums[actionNum]; ++i) {\n"
"				symbolStack.pop_back();\n"
"				stateStack.pop_back();\n"
"			}\n"
"			symbolStack.push_back(production_left[actionNum]);\n"
"		}\n"
/* accept */
"		else if (action[currentState][readPointer][0] == 'a') {\n"
"			cout << \"accept\" << endl;\n"
"			return 1;\n"
"		}\n"
/* 不是上面的情况则出错 */
"		else {\n"
"			cout << \"error\" << endl;\n"
"			return -1;\n"
"		}\n"
"	}\n"
/* 不是上面的情况则出错 */
"	else {\n"
"		cout << \"error\" << endl;\n"
"		return -1;\n"
"	}\n"
"}\n";
