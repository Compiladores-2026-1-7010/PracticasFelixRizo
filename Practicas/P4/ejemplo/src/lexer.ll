%{
#include <sstream>
#include <cstdlib>
#include <iostream>
#include "Parser.hpp"
#include "Lexer.hpp"

using namespace std;

#undef YY_DECL
#define YY_DECL int calc::Lexer::lex(calc::Parser::semantic_type *yylval)
%}

%option c++
%option noyywrap
%option outfile="Lexer.cpp"
%option yyclass="Lexer"

DIG [0-9]
WSPC [ \t]+

%%

"+"	{ return Parser::token::MAS; }
"-"	{ return Parser::token::MENOS; } /* Ejercicio 3 */
"*"	{ return Parser::token::MUL; }
"/"	{ return Parser::token::DIV; }   /* Ejercicio 3 */

"("	{ return Parser::token::PARIZQ; }
")"	{ return Parser::token::PARDER; }

"\n" { return Parser::token::EOL; }   /* Ejercicio 2: Detectar Enter */

{DIG}+(\.{DIG}+)? { /* Ejercicio 1: Regex para decimales */
	 yylval->val = atof(yytext); 
	 return Parser::token::NUM;
	}

{WSPC}  { /* Ignoramos espacios */ }

. {
	  cout << "Error lexico: " << yytext << endl;
}

<<EOF>> { yyterminate(); }

%%