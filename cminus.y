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

%token ; , ( ) if while return { } int char bool float double void = += -= *= /= + - * / && || ! true false
%token == <= >= != > < IDENTIFIER NUMBER

%right = += -= *= /=
%left ||
%left &&
%left == !=
%left < > <= >=
%left + -
%left * /
%right !

%%
program  : declarations                  
        ;

declarations    : declaration declarations          
        | declaration                   
        ;

declaration : fun_declaration              
        | var_declaration              
        ;

var_declaration : type identifier ;             
        | type assign_expr ;            
        ;

fun_declaration : type identifier ( parameter_list ) block  
        | type identifier ( ) block         
        ;

parameter_list  : type identifier , parameter_list        
        | type identifier               
        ;

stmt    : if ( logic_expr ) stmt         
        | if ( logic_expr ) stmt else stmt  
        | while ( logic_expr ) stmt     
        | var_declaration               
        | assign_expr ;              
        | function_calling_expr ;         
        | return arithmetic_expr ;      
        | block                   
        ;

stmts       : stmt stmts                   
        | stmt                     
        ;

block       : { stmts }              
        ;

type    : int                    
        | char                   
        | bool                    
        | float                   
        | double                
        | void                   
        ;

expr        : assign_expr                  
        | arithmetic_expr              
        | logic_expr                  
        ;

assign_expr : identifier = expr              
        | identifier += expr             
        | identifier -= expr              
        | identifier *= expr             
        | identifier /= expr             
        ;

arithmetic_expr : arithmetic_expr + arithmetic_expr       
        | arithmetic_expr - arithmetic_expr       
        | arithmetic_expr * arithmetic_expr       
        | arithmetic_expr / arithmetic_expr       
        | ( arithmetic_expr )          
        | identifier                   
        | number                    
        | function_calling_expr             
        ;

logic_expr  : logic_expr && logic_expr           
        | logic_expr || logic_expr           
        | ! logic_expr                
        | ( logic_expr )               
        | arithmetic_expr == arithmetic_expr      
        | arithmetic_expr > arithmetic_expr      
        | arithmetic_expr < arithmetic_expr       
        | arithmetic_expr >= arithmetic_expr     
        | arithmetic_expr <= arithmetic_expr      
        | arithmetic_expr != arithmetic_expr     
        | true                   
        | false               
        ;

function_calling_expr : identifier ( argument_list )  
            | identifier ( )            
            ;

argument_list   : arithmetic_expr , argument_list     
        | arithmetic_expr              
        ;

identifier  : IDENTIFIER                 
        ;

number      : NUMBER                 
        ;

%%
int main()
{
    return ;
}