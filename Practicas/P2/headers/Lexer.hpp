#ifndef __LEXER_HPP__
#define __LEXER_HPP__

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

class Lexer : public yyFlexLexer
{
public:
    // Constructor que recibe el stream de entrada (el archivo de texto)
    Lexer(std::istream *in) : yyFlexLexer(in){};

    // La función principal que el parser llamará para obtener el siguiente token
    virtual int yylex();
    
    // Función de utilidad para obtener el número de línea actual para los reportes de error
    virtual int getLine();

private:
    const int ERROR = -1;
};

#endif /* __LEXER_HPP__ */