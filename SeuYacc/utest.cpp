#include "YaccManager.h"
#include "Production.h"
#include "Symbol.h"
#include <map>
#include <vector>

using namespace std;

int main(int argc, char **argv)
{
    YaccManager ym;
    vector<Symbol> ss;
    vector<Production> ps;
    
//     Symbol nt_A("A", 2);
//     Symbol nt_B("B", 2);
//     Symbol t_a("a", 1);
//     Symbol t_c("c", 1);
//     Symbol epsilon("epsilon", 0);
//     ss.push_back(nt_A);
//     ss.push_back(nt_B);
//     ss.push_back(t_a);
//     ss.push_back(t_c);
//     ss.push_back(epsilon);
// 
//     vector<Symbol> r0;
//     r0.push_back(t_a);
//     r0.push_back(nt_A);
//     Production p0(0, nt_A, r0);
// 
//     vector<Symbol> r1;
//     r1.push_back(nt_B);
//     r1.push_back(t_c);
//     Production p1(1, nt_A, r1);
// 
//     vector<Symbol> r2;
//     r2.push_back(epsilon);
//     Production p2(2, nt_B, r2);
// 
//     ps.push_back(p0);
//     ps.push_back(p1);
//     ps.push_back(p2);

    //Symbol nt_Ep("E'", 2);
    //Symbol nt_E("E", 2);
    //Symbol t_plus("+", 1);
    //Symbol t_mul("*", 1);
    //Symbol t_left("(", 1);
    //Symbol t_right(")", 1);
    //Symbol t_id("id", 1);
    //ss.push_back(nt_Ep);
    //ss.push_back(nt_E);
    //ss.push_back(t_plus);
    //ss.push_back(t_mul);
    //ss.push_back(t_left);
    //ss.push_back(t_right);
    //ss.push_back(t_id);

    //vector<Symbol> r0;
    //r0.push_back(nt_E);
    //Production p0(0, nt_Ep, r0);

    //vector<Symbol> r1;
    //r1.push_back(nt_E);
    //r1.push_back(t_plus);
    //r1.push_back(nt_E);
    //Production p1(1, nt_E, r1);

    //vector<Symbol> r2;
    //r2.push_back(nt_E);
    //r2.push_back(t_mul);
    //r2.push_back(nt_E);
    //Production p2(2, nt_E, r2);
    //
    //vector<Symbol> radd;
    //radd.push_back(t_left);
    //radd.push_back(nt_E);
    //radd.push_back(t_right);
    //Production padd(3, nt_E, radd);

    //vector<Symbol> r3;
    //r3.push_back(t_id);
    //Production p3(4, nt_E, r3);

    //ps.push_back(p0);
    //ps.push_back(p1);
    //ps.push_back(p2);
    //ps.push_back(padd);
    //ps.push_back(p3);

    //map<string, Priority> priorities;
    //priorities["+"] = Priority(1, 0);
    //priorities["*"] = Priority(2, 0);
    
//     ym.set_symbols(ss);
//     ym.set_productions(ps);
//     ym.set_priorities(priorities);
    
    
    //ym.run();
	 ym.run();

    return 0;
}