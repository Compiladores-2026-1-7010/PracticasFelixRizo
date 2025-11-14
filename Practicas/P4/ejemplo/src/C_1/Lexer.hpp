#ifndef __LEXER_HPP__
#define __LEXER_HPP__

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "Parser.hpp" // Necesario para semantic_type

namespace calc { 
  class Lexer : public yyFlexLexer {
  public:
    Lexer(std::istream& arg_yyin, std::ostream& arg_yyout) : yyFlexLexer(arg_yyin, arg_yyout) {}
    Lexer(std::istream* arg_yyin = nullptr, std::ostream* arg_yyout = nullptr) : yyFlexLexer(arg_yyin, arg_yyout) {}
    
    int lex(Parser::semantic_type *yylval);
    int getLine(); // Tu función personalizada
  };
}
#endif