/*
 * 这个文件中的代码应该粘贴到yacc的代码生成部分,用于最终.cpp中所需变量的初始化
 */

/* 生成产生式的左部和右部数量vector */
string output1, output2;
vector<string> production_left;
output1 = "vector<string> production_left;\n";
vector<int> production_right_symbol_nums;
output2 = "vector<int> production_right_symbol_nums;\n"
// 遍历productions，保存相应值到vector中
for (size_t i = 0; i < productions.size(); ++i) {
	Production current = productions.at(i);
	output1 += "production_left.push_back(" + current.left.value + ");\n";
	output2 += "production_right_symbol_nums.push_back(" + current.right.size() + ");\n";
}
cout << output1 << output2;


/* 上面那段代码可以换成下面这段 */
// stringstream output1, output2;
// vector<string> production_left;
// output1 << "vector<string> production_left;\n";
// vector<int> production_right_symbol_nums;
// output2 << "vector<int> production_right_symbol_nums;\n";
// // 遍历productions，保存相应值到vector中
// for (size_t i = 0; i < productions.size(); ++i) {
	// Production current = productions.at(i);
	// output1 << "production_left.push_back(" << current.left.value << ");\n";
	// output2 << "production_right_symbol_nums.push_back(" << current.right.size() << ");\n";
// }
// cout << output1.str() << output2.str();


/* 在.cpp中初始化action表 */
cout << "map<string, string> action;\n";
for (size_t i = 0; i < l; i++) {
	map<string, string> ca = action.at(i);
	map<string, string>::iterator it = ca.begin();
	for (; it != ca.end(); ++it) {
		cout << "action[\"" << it->first << "\"] = " << it->second << ";" << endl;
	}
}


/* 在.cpp中输出化goto表 */
cout << "vector<map<string, int>> goto_table;\n" ;
for (int  i = 0; i < goto_table.size(); ++i) {
	map<string, int> line = goto_table.at(i);
	map<string, int>::iterator it = line.begin();
	for(; it != line.end(); ++it) {
		cout << "goto_table[\"" << it->first << "\"] = " << it->second << ";" << endl;
	}
}
