%{
/*  myyacc.txt
*/
#define _CRT_SECURE_NO_WARNINGS //这个宏定义是因为某些VC版本会对strtok报unsafe的错，加了就忽略了这个问题

#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string>
using namespace std;

%}

%token a

%%
S : A
  ;

A : a A
  |
  ;
%%
int main()
{
    return ;
}