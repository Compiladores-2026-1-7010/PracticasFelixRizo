#ifndef __PARSER_LR_HPP__
#define __PARSER_LR_HPP__
#include "Grammar.h"
#include "Symbols.hpp"
#include "Lexer.hpp"

#include <string>
#include <vector>
#include <map>
#include <stack>
using namespace std;

enum ActionType {reduce, shift, acc}; //reducción, desplazamiento, aceptación

/**
 * Estructura para ACCIÓN (de la tabla de an. sintáctico LR)
 */
struct Action{
  ActionType type; //si la acción es reducir, desplazar o aceptar
  int num; //número de estado en caso  de desplazamiento, o número de transición en caso de reducción
  Action(){}; 
  Action(ActionType type, int num)
  {
    this->type = type;
    this->num = num;
  };
};

/**
 * Clase para analizadores sintácticos LR
 */
class ParserLR
{
private:
  Grammar grammar; //la gramática que debe reconocer nuestro analizador sintáctico
  Token token; //token actual 
  Lexer *lexer; //analizador léxico para cadenas de G
  map<int, map<Token, Action>> ACTION; //parte de la tabla LR para ACCIÓN (edo, (term, acción))
  map<int,map<NoTerm,int>> GOTO; //parte de la tabla LR para GOTO (edo, (no-term, edo))
  
public:
  ParserLR(Lexer *lexer);  //constructor
  ~ParserLR();
  
  void loadSyms();//función para agregar todos los símbolos a la gramática (en grammar)
  void loadProds(); //función para cargar todas las producciones (en grammar)
  void loadTable(); //función para definir la tabla de análisis sintáctico LR
  int parse(); //función con la implementación del algoritmo de análisis sintáctico LR
  
  void error(string msg); //función para impresión de errores sintácticos
  Token eat(); //consumo de entrada
  void aceptar(string msg); //función para impresión de mensaje de aceptación
};




#endif // !__PARSER_LR_HPP__
