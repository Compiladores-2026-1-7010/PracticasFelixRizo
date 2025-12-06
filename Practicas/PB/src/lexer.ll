%{
#include <iostream>
#include <string>
#include <cstdlib> // 
using namespace std;

#include "headers/Symbols.hpp"
#include "headers/Lexer.hpp"

int line=1;
%}

%option c++
%option outfile="Lexer.cpp"
%option yyclass="Lexer"

/* Definiciones regulares */
DIGITO [0-9]
ID     [a-zA-Z_][a-zA-Z0-9_]*
NUM    {DIGITO}+(\.{DIGITO}+)?

%%

[ \t\r]+   { /* Ignorar espacios en blanco */ }
"\n"	   { line++; }

"int"      { return t_int; }
"float"    { return t_float; }
"if"       { return t_if; }
"else"     { return t_else; }
"while"    { return t_while; }

";"        { return t_semicolon; }
","        { return t_comma; }
"="        { return t_assign; }
"+"        { return t_plus; }
"-"        { return t_minus; }
"*"        { return t_mul; }
"/"        { return t_div; }
"("        { return t_lparen; }
")"        { return t_rparen; }

{ID}       { return t_id; }
{NUM}      { return t_num; }

.   	   { 
             cout << "ERROR LEXICO " << yytext << " en linea " << line << endl; 
             exit(EXIT_FAILURE); // 
           }

%%

int yyFlexLexer::yywrap(){
    return 1;
}

int Lexer::getLine()
{
    return line;
}