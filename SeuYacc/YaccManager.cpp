#include "YaccManager.h"
#include <iostream>

using namespace std;

YaccManager::YaccManager(void)
{
}

YaccManager::~YaccManager(void)
{
}

/************ Helper Function Begin **************/

/*
 * 检查某个文法符号是否已在一个文法符号集合中
 * @ss : vector<Symbol> 文法符号集合
 * @s : Symbol 待检查文法符号
 * @return : bool
 */
bool YaccManager::is_symbol_in_first_set(const vector<Symbol> &ss, const Symbol &s)
{
    for (size_t i = 0; i < ss.size(); i++) {
        if (ss.at(i).equal(s)) {
            return true;
        }
    }

    return false;
}

/*
 * 合并两个first集合，求并集，结果保存在第一个中
 * @s1 : vector<Symbol>
 * @s2 : vector<Symbol>
 * @return : void
 */
void YaccManager::merge_two_first_set(vector<Symbol> &s1, const vector<Symbol> &s2)
{
    for (size_t i = 0; i < s2.size(); i++) {
        Symbol ri = s2.at(i);
        if (!is_symbol_in_first_set(s1, ri)) {
            s1.push_back(ri);
        }
    }
}

/*
 * 去除epsilon
 * @s : vector<Symbol>
 * @return : vector<Symbol>
 */
vector<Symbol> YaccManager::remove_epsilon(const vector<Symbol> &s)
{
    vector<Symbol> result;
    for (size_t i = 0; i < s.size(); i++) {
        if (s.at(i).type != 0) {
            result.push_back(s.at(i));
        }
    }

    return result;
}

/************ Helper Function End **************/

/*
 * 求单个文法符号的first集合
 * 此方法不处理左递归，针对左递归，假设其不存在epsilon
 * 如存在A->Aa，则不存在A->epsilon，针对A->Aa这样的产生式，直接忽略
 * @symbol : Symbol
 * @return : vector<Symbol>
 */
vector<Symbol> YaccManager::first(Symbol &symbol)
{
    int s_type = symbol.type;
    vector<Symbol> result;
    
    if (s_type == 2) { // nonterminal
        // 遍历产生式
        for (size_t i = 0; i < productions.size(); i++) {
            Production current_production = productions.at(i);
            if (current_production.left.equal(symbol)) { // 产生式左部为symbol
                vector<Symbol> right = current_production.right;
                Symbol right0 = right.at(0);
                if (right0.equal(symbol)) {
                    // 类似A->Aa，忽略
                } else if (right0.type == 0) {
                    // epsilon
                    if (!is_symbol_in_first_set(result, right0)) {
                        result.push_back(right0);
                    }
                } else {
                    vector<Symbol> right_first = first_beta_a(right);  
                    merge_two_first_set(result, right_first);
                }
            }
        }
    } else {
        result.push_back(symbol);
    }

    return result;
}

/*
 * 求一串文法符号的first集合
 * @symbols : vector<Symbol>
 * @return : vector<Symbol>
 */
vector<Symbol> YaccManager::first_beta_a(vector<Symbol> &symbols)
{
    vector<Symbol> result;
    
    // 遍历symbols中的每一个符号
    for (size_t i = 0; i < symbols.size(); i++) {
        Symbol current_symbol = symbols.at(i);
        int type = current_symbol.type;

        if (type == 1) {
            // 检查 0 ~ i-1 是否有epsilon，只要有一个没有就直接返回result
            Symbol epsilon("epsilon", 0);
            for (size_t bs = 0; bs < i; bs++) {
                if (!is_symbol_in_first_set(first(symbols.at(bs)), epsilon)) {
                    return result;
                }
            }
            result.push_back(current_symbol);
            return remove_epsilon(result);
        } else if (type == 2) {
            // 检查 0 ~ i-1 是否有epsilon，只要有一个没有就直接返回result
            Symbol epsilon("epsilon", 0);
            for (size_t bs = 0; bs < i; bs++) {
                if (!is_symbol_in_first_set(first(symbols.at(bs)), epsilon)) {
                    return remove_epsilon(result);
                }
            }
            merge_two_first_set(result, first(current_symbol));
        }
    }

    return remove_epsilon(result);
}


void YaccManager::set_productions(vector<Production> &ps)
{
    this->productions = ps;
}

void YaccManager::set_symbols(vector<Symbol> &ss)
{
    this->symbols = ss;
}

void YaccManager::test_run()
{
    vector<Symbol> rs = first_beta_a(productions.at(0).right);

    for (size_t i = 0; i < rs.size(); i++) {
        Symbol crs = rs.at(i);

        cout << crs.value << " ";
    }

    cout << endl;
}