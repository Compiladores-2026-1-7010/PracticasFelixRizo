%{
#include <iostream>
#include <string>
#include <cstdlib> 
#include "Parser.hpp" /* Bison genera este archivo */
#include "Lexer.hpp"

using namespace std;

/* Adaptamos la firma para que coincida con la llamada desde Parser */
#undef YY_DECL
#define YY_DECL int calc::Lexer::lex(calc::Parser::semantic_type *yylval)

int line = 1; /* Tu contador de líneas */
%}

%option c++
%option noyywrap
%option outfile="Lexer.cpp"
%option yyclass="Lexer"

/* Definiciones Regulares (Las tuyas) */
ID      [a-zA-Z_][a-zA-Z0-9_]*
NUM     [0-9]+(\.[0-9]+)?
BLANK   [ \t\r]+

%%

{BLANK}  { /* Ignorar espacios en blanco */ }
"\n"     { line++; }

"int"    { return Parser::token::INT; }
"float"  { return Parser::token::FLOAT; }
"if"     { return Parser::token::IF; }
"else"   { return Parser::token::ELSE; }
"while"  { return Parser::token::WHILE; }

"+"      { return Parser::token::MAS; }
"-"      { return Parser::token::MENOS; }
"*"      { return Parser::token::MUL; }
"/"      { return Parser::token::DIV; }
"="      { return Parser::token::ASSIGN; }
"("      { return Parser::token::LPAREN; }
")"      { return Parser::token::RPAREN; }
";"      { return Parser::token::PYC; }
","      { return Parser::token::COMA; }

"{"      { return Parser::token::LKEY; } /* NECESARIO para la gramática */
"}"      { return Parser::token::RKEY; } /* NECESARIO para la gramática */

{ID}     { 
           /* Guardamos el texto para que el Parser sepa el nombre */
           yylval->sval = strdup(yytext); 
           return Parser::token::ID; 
         }

{NUM}    { 
           yylval->sval = strdup(yytext);
           return Parser::token::NUM; 
         }

<<EOF>>  { return Parser::token::END; }

.        { cout << "ERROR LEXICO EN LINEA " << line << ": " << yytext << endl; }

%%

/* Esta función permite al Parser preguntar en qué línea estamos */
int calc::Lexer::getLine() {
    return line;
}