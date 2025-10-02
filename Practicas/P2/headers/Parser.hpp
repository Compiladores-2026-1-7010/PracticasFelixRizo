#ifndef PARSER_H
#define PARSER_H
#include <string>
#include "Lexer.hpp"
#include "Symbols.hpp"
using namespace std;

namespace C_1
{
  class Parser
  {
  private:
    Lexer *lexer;
    int token;
    void match(int expectedToken);
    
  public:
    Parser(Lexer *lexer);
    ~Parser();
    
    // Funciones para cada No-Terminal
    void programa();
    void declaraciones();
    void declaraciones_p();
    void declaracion();
    void tipo();
    void lista_var();
    void lista_var_p();
    void sentencias();
    void sentencias_p();
    void sentencia();
    void expresion();
    void expresion_p();
    void termino();
    void termino_p();
    void factor();
    
    void nextToken();
    void error(string msg);
    void parse();
  };    
}

#endif