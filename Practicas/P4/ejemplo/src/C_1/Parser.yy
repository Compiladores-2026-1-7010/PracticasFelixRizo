%{
#include <iostream>
#include <string>
#include "Lexer.hpp"

using namespace std;

/* Declaración adelantada para el error */
void yyerror(const char *s);
%}

%language "C++"
%defines "Parser.hpp"
%output "Parser.cpp"
 
%define api.parser.class {Parser}
%define api.namespace {calc}
%parse-param {Lexer* lexer}

%code requires {
    namespace calc { class Lexer; }
}
 
%code {
 #include "Lexer.hpp"
 #define yylex(x) lexer->lex(x)
}

%union {
    char* sval; /* Para guardar el texto de ID y NUM */
}

/* Definición de Tokens (Deben coincidir con el Lexer) */
%token<sval> ID NUM
%token INT FLOAT IF ELSE WHILE
%token ASSIGN PYC COMA
%token MAS MENOS MUL DIV
%token LPAREN RPAREN LKEY RKEY
%token END 0 "end of file"

/* Precedencia (Menor a Mayor) */
%left MAS MENOS
%left MUL DIV
%nonassoc LPAREN RPAREN
%nonassoc IFX
%nonassoc ELSE

%start programa

%%

/* Gramática definida en la práctica */

programa: declaraciones sentencias { cout << ">>> Programa analizado correctamente." << endl; }
        ;

declaraciones: declaraciones declaracion
             | declaracion
             ;

declaracion: tipo lista_var PYC { cout << "   [Declaracion detectada]" << endl; }
           ;

tipo: INT 
    | FLOAT 
    ;

lista_var: lista_var COMA ID { cout << "   Variable extra: " << $3 << endl; }
         | ID { cout << "   Nueva variable: " << $1 << endl; }
         ;

sentencias: sentencias sentencia
          | sentencia
          ;

sentencia: ID ASSIGN expresion PYC { cout << "   [Asignacion] " << $1 << endl; }
         | IF LPAREN expresion RPAREN LKEY sentencias RKEY %prec IFX { cout << "   [Bloque IF]" << endl; }
         | IF LPAREN expresion RPAREN LKEY sentencias RKEY ELSE LKEY sentencias RKEY { cout << "   [Bloque IF-ELSE]" << endl; }
         | WHILE LPAREN expresion RPAREN LKEY sentencias RKEY { cout << "   [Bloque WHILE]" << endl; }
         ;

expresion: expresion MAS expresion
         | expresion MENOS expresion
         | expresion MUL expresion
         | expresion DIV expresion
         | ID
         | NUM
         | LPAREN expresion RPAREN
         ;

%%

/* Implementación de función de error explícito */
void calc::Parser::error(const std::string& msg) {
    std::cerr << "Error de Sintaxis: " << msg << " en la linea " << lexer->getLine() << endl;
}