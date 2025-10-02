#ifndef __SYMBOLS_HPP__
#define __SYMBOLS_HPP__

// Enumeración para el conjunto T (Tokens)
enum Token {
    t_int = 1, t_float, t_if, t_else, t_while, t_id, t_num,
    t_mas, t_menos, t_mul, t_div, t_assign, t_lparen,
    t_rparen, t_pyc, t_coma, t_eof
};

#endif // !__SYMBOLS_HPP__