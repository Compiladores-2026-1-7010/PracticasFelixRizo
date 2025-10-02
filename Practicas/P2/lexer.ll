%{
#include <iostream>
#include <string>
using namespace std;

#include "headers/Symbols.hpp"
#include "headers/Lexer.hpp"

// Variable para llevar el conteo de la línea actual
int line=1;
%}

%option c++
%option outfile="Lexer.cpp"
%option yyclass="Lexer"

/* Definiciones Regulares */
ID      [a-zA-Z_][a-zA-Z0-9_]*
NUM     [0-9]+(\.[0-9]+)?
BLANK   [ \t\r]+

%%

{BLANK}  { /* Ignorar espacios en blanco */ }
"\n" 	 { line++; }

"int"    { return t_int; }
"float"  { return t_float; }
"if"     { return t_if; }
"else"   { return t_else; }
"while"  { return t_while; }

{ID}     { return t_id; }
{NUM}    { return t_num; }

"+"      { return t_mas; }
"-"      { return t_menos; }
"*"      { return t_mul; }
"/"      { return t_div; }
"="      { return t_assign; }
"("      { return t_lparen; }
")"      { return t_rparen; }
";"      { return t_pyc; }
","      { return t_coma; }

<<EOF>>  { return t_eof; }   // <-- ¡ESTA ES LA LÍNEA NUEVA!

.        { cout << "ERROR LEXICO EN LÍNEA " << line << ": " << yytext << endl; }

%%

// Flex necesita esta función
int yyFlexLexer::yywrap(){
    return 1;
}

// Función para obtener la línea actual desde el Parser
int Lexer::getLine()
{
    return line;
}