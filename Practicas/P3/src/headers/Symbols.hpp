#ifndef __SYMBOLS_HPP__
#define __SYMBOLS_HPP__

/**
 * 1. Gramática G′ = ( N′, Σ, P′, S )
 * N′ = {
 *   programa,
 *   declaraciones, declaraciones',
 *   declaracion, tipo,
 *   lista_var, lista_var',
 *   sentencias, sentencias',
 *   sentencia,
 *   expresion, expresion',
 *   termino, termino',
 *   factor
 * }
 *
 * Σ = {
 *   int, float, identificador, numero,
 *   if, else, while,
 *   ',', ';', '=', '(', ')', '+', '-', '*', '/',
 *   $
 * }
 *
 * P′:
 *   1)  programa       → declaraciones sentencias
 *   2)  declaraciones  → declaracion declaraciones'
 *   3)  declaraciones' → declaracion declaraciones' | ε
 *   4)  declaracion    → tipo lista_var ;
 *   5)  tipo           → int | float
 *   6)  lista_var      → identificador lista_var'
 *   7)  lista_var'     → , identificador lista_var' | ε
 *   8)  sentencias     → sentencia sentencias'
 *   9)  sentencias'    → sentencia sentencias' | ε
 *   10) sentencia      → identificador = expresion ;
 *                      | if ( expresion ) sentencias else sentencias
 *                      | while ( expresion ) sentencias
 *   11) expresion      → termino expresion'
 *   12) expresion'     → + termino expresion' | - termino expresion' | ε
 *   13) termino        → factor termino'
 *   14) termino'       → * factor termino' | / factor termino' | ε
 *   15) factor         → identificador | numero | ( expresion )
 *
 * Símbolo inicial:
 *   S = programa
 */

/**
 * 2. Determinar la enumeración para el conjunto T
 * (Modificado para coincidir con lexer.ll)
 */
enum Token {
// Nombres del lexer.ll
t_int,
t_float,
t_if,
t_else,
t_while,
t_id,
t_num,
t_assign,
t_pyc,  
t_coma, 
t_mas,  
t_menos, 
t_mul,   
t_div,   
t_lparen,
t_rparen,
t_eof // Corresponde a <<EOF>>
};


enum NoTerm {
PROGRAMA,
DECLARACIONES,
DECLARACIONES_P,
DECLARACION,
TIPO,
LISTA_VAR,
LISTA_VAR_P,
SENTENCIAS,
SENTENCIAS_P,
SENTENCIA,
EXPRESION,
EXPRESION_P,
TERMINO,
TERMINO_P,
FACTOR
};

#endif // !__SYMBOLS_HPP__