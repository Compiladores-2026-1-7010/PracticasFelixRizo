#include "headers/Parser.hpp"
#include <iostream>

namespace C_1
{
  Parser::Parser(Lexer *lexer)
  {
    this->lexer = lexer;
  }
    
  Parser::~Parser()
  {
  }

  void Parser::nextToken(){
    token = lexer->yylex();
  }

  void Parser::match(int expectedToken) {
    if (token == expectedToken) {
        nextToken();
    } else {
        error("Token inesperado.");
    }
  }

  void Parser::error(string msg){
    cout << "ERROR DE SINTAXIS: " << msg << " en la línea "<< lexer->getLine() <<endl;
    exit(EXIT_FAILURE);
  }   

  void Parser::parse(){
    nextToken();
    programa();
    if(token == t_eof)
      cout << "La cadena es aceptada" << endl;
    else 
      error("La cadena no pertenece al lenguaje.");
  }

  /************************************************************************/
  /** Funciones para cada No-Terminal (NT)                 **/
  /************************************************************************/

  void Parser::programa(){
    declaraciones();
    sentencias();
  }

  void Parser::declaraciones() {
    declaracion();
    declaraciones_p();
  }

  void Parser::declaraciones_p() {
    if (token == t_int || token == t_float) {
        declaracion();
        declaraciones_p();
    }
  }

  void Parser::declaracion() {
    tipo();
    lista_var();
    match(t_pyc);
  }

  void Parser::tipo() {
    if (token == t_int) match(t_int);
    else if (token == t_float) match(t_float);
    else error("Se esperaba un tipo (int o float)");
  }
  
  void Parser::lista_var() {
      match(t_id);
      lista_var_p();
  }

  void Parser::lista_var_p() {
    if (token == t_coma) {
        match(t_coma);
        match(t_id);
        lista_var_p();
    }
  }
  
  void Parser::sentencias() {
    sentencia();
    sentencias_p();
  }

  void Parser::sentencias_p() {
    if (token == t_id || token == t_if || token == t_while) {
        sentencia();
        sentencias_p();
    }
  }

  void Parser::sentencia() {
    switch (token) {
        case t_id:
            match(t_id); match(t_assign); expresion(); match(t_pyc);
            break;
        case t_if:
            match(t_if); match(t_lparen); expresion(); match(t_rparen);
            sentencias(); match(t_else); sentencias();
            break;
        case t_while:
            match(t_while); match(t_lparen); expresion(); match(t_rparen); sentencias();
            break;
        default:
            error("Se esperaba una sentencia (asignación, if o while)");
    }
  }

  void Parser::expresion() {
    termino();
    expresion_p();
  }
  
  void Parser::expresion_p() {
    if (token == t_mas) {
        match(t_mas); termino(); expresion_p();
    } else if (token == t_menos) {
        match(t_menos); termino(); expresion_p();
    }
  }

  void Parser::termino() {
    factor();
    termino_p();
  }

  void Parser::termino_p() {
    if (token == t_mul) {
        match(t_mul); factor(); termino_p();
    } else if (token == t_div) {
        match(t_div); factor(); termino_p();
    }
  }

  void Parser::factor() {
    switch (token) {
        case t_lparen:
            match(t_lparen); expresion(); match(t_rparen);
            break;
        case t_id:
            match(t_id);
            break;
        case t_num:
            match(t_num);
            break;
        default:
            error("Se esperaba '(', un identificador o un número");
    }
  }
}