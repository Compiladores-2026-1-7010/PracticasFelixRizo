%{
#include <iostream>
#include <string>
using namespace std;

/* Incluir archivos de cabecera del proyecto */
#include "tokens.hpp"  // Definición de los tokens (IDs)
#include "Lexer.hpp"   // Definición de la clase Lexer
%}

/* Opciones de Flex++ */
%option c++                 // Genera código en C++
%option outfile="Lexer.cpp" // Nombre del archivo de salida
%option yyclass="C_1::Lexer" // Nombre de la clase Lexer generada
%option case-insensitive    // Ignorar mayúsculas/minúsculas

/* Definición de patrones reutilizables */
DIG [0-9]                   // Dígito del 0 al 9
ID_RE [a-zA-Z_][a-zA-Z0-9_]* // Identificador válido (letra o _ seguido de letras, dígitos o _)

%%

/* Palabras reservadas */
"int"       { return C_1::INT; }   // Token INT
"float"     { return C_1::FLOAT; } // Token FLOAT
"if"        { return C_1::IF; }    // Token IF
"else"      { return C_1::ELSE; }  // Token ELSE
"while"     { return C_1::WHILE; } // Token WHILE

/* Números (enteros y flotantes con notación científica) */
{DIG}+("."{DIG}*)?([eE][+-]?{DIG}+)?  { return C_1::NUMERO; }

/* Identificadores */
{ID_RE}     { return C_1::ID; }

/* Símbolos simples */
";"         { return C_1::PYC; }   // Punto y coma
","         { return C_1::COMA; }  // Coma
"("         { return C_1::LPAR; }  // Paréntesis izquierdo
")"         { return C_1::RPAR; }  // Paréntesis derecho

/* Ignorar espacios, tabs y saltos de línea */
[ \t\n]+    { /* nada */ }

/* Cualquier otro carácter no reconocido */
.           { 
                cout << "ERROR LEXICO " << yytext << endl; 
                return 0; 
            }

%%

/* Función de cierre de Flex */
int yyFlexLexer::yywrap(){
    return 1;
}
