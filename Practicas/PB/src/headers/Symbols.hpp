#ifndef __SYMBOLS_HPP__
#define __SYMBOLS_HPP__

/** * 1. Definir en un comentario la gramática G'(N, T, P', S) aumentada de G(N, T, P, S) 
 */

/**
 * 2. Determinar la enumeración para el conjunto T
 */
enum Token { 
    t_eof = 0,    // Flex devuelve 0 en EOF por defecto
    t_int, 
    t_float, 
    t_id, 
    t_num, 
    t_semicolon, // ;
    t_comma,     // ,
    t_assign,    // =
    t_if, 
    t_else, 
    t_while, 
    t_plus,      // +
    t_minus,     // -
    t_mul,       // *
    t_div,       // /
    t_lparen,    // (
    t_rparen     // )
};

/**
 * 3. Determinar la enumeración para el conjunto N
 */
enum NoTerm { 
    e_programa = 100, 
    e_declaraciones, 
    e_declaracion, 
    e_tipo, 
    e_lista_var, 
    e_sentencias, 
    e_sentencia, 
    e_expresion 
};
 
#endif // !__SYMBOLS_HPP__