%{
/*  myyacc.txt
*/
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
using namespace std;

%}

%token , + - * / > < ; { } ( )
%token IDENTIFIER NUMBER PROT
%token INT VOID UNION STATIC STRUCT AUTO ENUM BREAK CONST CONTINUE
%token DO FOR WHILE IF ELSE EXTERN REGISTER RETURN SIZEOF TYPEDEF
%token SWITCH CASE DEFAULT CHAR SHORT
%token NE_OP EQ_OP 

%left ';'
%left '>' '<' NE_OP EQ_OP
%left '+' '-'
%left '*' '/'

%%
program : declarations
      ;
declarations : var_declaration declarations
      | declaration
      ;
declaration  : fun_declaration declaration
      |
      ;
fun_declaration : type IDENTIFIER ( parameters ) block
      ;
type : VOID
     | INT
     ;
var_declaration : type identifiers ;
     ;
identifiers : IDENTIFIER , identifiers
     | assign_statement , identifiers
     | identifier
     ;
identifier : IDENTIFIER 
     | assign_statement
     ;
parameters : parameterlist
     |
     ;
parameterlist : parameter , parameterlist
     | parameter
     ;
parameter : type IDENTIFIER
     ;
block : { local_declarations statements }
     ;
local_declarations : var_declaration local_declarations
     |
     ;
statements : statement statements
     | ;
     |
     ;
statement : if_statement
     | while_statement
     | return_statement
     | assign_statements
     | break_statement
     ;
if_statement : IF ( exp ) block
     | IF ( exp ) block  ELSE block
     ;
while_statement : WHILE ( exp ) block
     ;
return_statement : RETURN
     | RETURN exp
     ;
break_statement : BREAK
     ;
assign_statements : assign_statement assign_statements
     | var_declaration assign_statements
     |
     ;
assign_statement : IDENTIFIER = exp
     ;
exp : exp + exp
    | exp - exp
    | exp / exp
    | exp * exp
    | ( exp )
    | IDENTIFIER
    | IDENTIFIER ( args )
    | NUMBER   
    | exp EQ_OP  exp 
    | exp > exp
    | exp < exp
    | exp NE_OP  exp
    ;
args : exp , args
     |
     ;
%%
int main()
{
    return ;
}