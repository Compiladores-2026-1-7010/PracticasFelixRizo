%{
#include <iostream>
#include <fstream>
#include <string>
#include <cmath> 
#include <FlexLexer.h>
using namespace std;
%}

%language "C++"
%require "3.2"
%defines "Parser.hpp"
%output "Parser.cpp"
 
%define api.parser.class {Parser}
%define api.namespace {calc}
%parse-param {Lexer* lexer}

%code requires
{
    namespace calc {
        class Lexer;
    } 
} 
 
%code
{
 #include "Lexer.hpp"
 #define yylex(x) lexer->lex(x) 
}

/* CAMBIO EJERCICIO 1: Usamos double para soportar decimales */
%union {
    double val;
}

/* CAMBIO EJERCICIO 2: Token para fin de línea */
%token EOL

/* CAMBIO EJERCICIO 1: El token NUM ahora guarda un valor double */
%token<val> NUM

/* CAMBIO EJERCICIO 3 y 4: Definimos precedencia */
%left MAS MENOS      
%left MUL DIV        
%nonassoc PARIZQ PARDER
%right MENOS_UNARIO  /* La precedencia más alta para el negativo (-5) */

%type<val> expresion

%start input

%%

/* CAMBIO EJERCICIO 2: Gramática recursiva para leer múltiples líneas */
input: 
      | input line
      ;

line: EOL
    | expresion EOL { 
        cout << "Resultado: " << $1 << endl; 
      }
    ;

expresion : expresion MAS expresion { $$ = $1 + $3; }
          | expresion MENOS expresion { $$ = $1 - $3; } /* Ejercicio 3 */
          | expresion MUL expresion { $$ = $1 * $3; }
          | expresion DIV expresion { 
                if($3 == 0) {
                    error("Error: Division por cero");
                    $$ = 0;
                } else {
                    $$ = $1 / $3; 
                }
            } /* Ejercicio 3 */
          | MENOS expresion %prec MENOS_UNARIO { $$ = -$2; } /* Ejercicio 4 */
          | PARIZQ expresion PARDER { $$ = $2; }
          | NUM { $$ = $1; }
          ;

%%

void calc::Parser::error(const std::string& msg) {
    std::cerr << "Error: " << msg << '\n';
}

int main(int argc, char *argv[]){
    if(argc < 2){
        cout << "Faltan argumentos (archivo de entrada)" << endl;
        return EXIT_FAILURE;
    }
    filebuf fb;
    fb.open(string(argv[1]), ios::in);
    if (!fb.is_open()) {
        cout << "No se pudo abrir el archivo." << endl;
        return EXIT_FAILURE;
    }
    istream in(&fb);
    calc::Lexer lexer(&in);
    calc::Parser parser(&lexer);
    parser.parse();
    fb.close();
    return 0;
}