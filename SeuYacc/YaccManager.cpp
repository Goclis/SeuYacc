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
 * ���ĳ���ķ������Ƿ�����һ���ķ����ż�����
 * @ss : vector<Symbol> �ķ����ż���
 * @s : Symbol ������ķ�����
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
 * �ϲ�����first���ϣ��󲢼�����������ڵ�һ����
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
 * ȥ��epsilon
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
 * �󵥸��ķ����ŵ�first����
 * �˷�����������ݹ飬�����ݹ飬�����䲻����epsilon
 * �����A->Aa���򲻴���A->epsilon�����A->Aa�����Ĳ���ʽ��ֱ�Ӻ���
 * @symbol : Symbol
 * @return : vector<Symbol>
 */
vector<Symbol> YaccManager::first(Symbol &symbol)
{
    int s_type = symbol.type;
    vector<Symbol> result;
    
    if (s_type == 2) { // nonterminal
        // ��������ʽ
        for (size_t i = 0; i < productions.size(); i++) {
            Production current_production = productions.at(i);
            if (current_production.left.equal(symbol)) { // ����ʽ��Ϊsymbol
                vector<Symbol> right = current_production.right;
                Symbol right0 = right.at(0);
                if (right0.equal(symbol)) {
                    // ����A->Aa������
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
 * ��һ���ķ����ŵ�first����
 * @symbols : vector<Symbol>
 * @return : vector<Symbol>
 */
vector<Symbol> YaccManager::first_beta_a(vector<Symbol> &symbols)
{
    vector<Symbol> result;
    
    // ����symbols�е�ÿһ������
    for (size_t i = 0; i < symbols.size(); i++) {
        Symbol current_symbol = symbols.at(i);
        int type = current_symbol.type;

        if (type == 1) {
            // ��� 0 ~ i-1 �Ƿ���epsilon��ֻҪ��һ��û�о�ֱ�ӷ���result
            Symbol epsilon("epsilon", 0);
            for (size_t bs = 0; bs < i; bs++) {
                if (!is_symbol_in_first_set(first(symbols.at(bs)), epsilon)) {
                    return result;
                }
            }
            result.push_back(current_symbol);
            return remove_epsilon(result);
        } else if (type == 2) {
            // ��� 0 ~ i-1 �Ƿ���epsilon��ֻҪ��һ��û�о�ֱ�ӷ���result
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