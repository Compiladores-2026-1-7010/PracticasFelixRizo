#include "headers/ParserLR.hpp"
#include <iostream>

ParserLR::ParserLR(Lexer *lexer)
{
  this->lexer = lexer;
  loadSyms();
  loadProds();
  loadTable();
}

ParserLR::~ParserLR()
{
}

void ParserLR::loadSyms()
{
}

void ParserLR::loadProds()
{
  // 1. programa -> declaraciones sentencias
  grammar.addProd(1, Production(e_programa, {e_declaraciones, e_sentencias}));
  // 2. declaraciones -> declaraciones declaracion
  grammar.addProd(2, Production(e_declaraciones, {e_declaraciones, e_declaracion}));
  // 3. declaraciones -> declaracion
  grammar.addProd(3, Production(e_declaraciones, {e_declaracion}));
  // 4. declaracion -> tipo lista_var ;
  grammar.addProd(4, Production(e_declaracion, {e_tipo, e_lista_var, t_semicolon}));
  // 5. tipo -> int
  grammar.addProd(5, Production(e_tipo, {t_int}));
  // 6. tipo -> float
  grammar.addProd(6, Production(e_tipo, {t_float}));
  // 7. lista_var -> lista_var , identificador
  grammar.addProd(7, Production(e_lista_var, {e_lista_var, t_comma, t_id}));
  // 8. lista_var -> identificador
  grammar.addProd(8, Production(e_lista_var, {t_id}));
  // 9. sentencias -> sentencias sentencia
  grammar.addProd(9, Production(e_sentencias, {e_sentencias, e_sentencia}));
  // 10. sentencias -> sentencia
  grammar.addProd(10, Production(e_sentencias, {e_sentencia}));
  // 11. sentencia -> identificador = expresion ;
  grammar.addProd(11, Production(e_sentencia, {t_id, t_assign, e_expresion, t_semicolon}));
  // 12. sentencia -> if ( expresion ) sentencias else sentencias
  grammar.addProd(12, Production(e_sentencia, {t_if, t_lparen, e_expresion, t_rparen, e_sentencias, t_else, e_sentencias}));
  // 13. sentencia -> while ( expresión ) sentencias
  grammar.addProd(13, Production(e_sentencia, {t_while, t_lparen, e_expresion, t_rparen, e_sentencias}));
  // 14. expresion -> expresion + expresion
  grammar.addProd(14, Production(e_expresion, {e_expresion, t_plus, e_expresion}));
  // 15-17. Otros operadores
  grammar.addProd(15, Production(e_expresion, {e_expresion, t_minus, e_expresion}));
  grammar.addProd(16, Production(e_expresion, {e_expresion, t_mul, e_expresion}));
  grammar.addProd(17, Production(e_expresion, {e_expresion, t_div, e_expresion}));
  // 18. expresion -> identificador
  grammar.addProd(18, Production(e_expresion, {t_id}));
  // 19. expresion -> numero
  grammar.addProd(19, Production(e_expresion, {t_num}));
  // 20. expresion -> ( expresion )
  grammar.addProd(20, Production(e_expresion, {t_lparen, e_expresion, t_rparen}));
}

void ParserLR::loadTable()
{
  // =========================================================
  // ESTADOS PRINCIPALES Y DECLARACIONES
  // =========================================================
  
  // ESTADO 0: Inicial
  ACTION[0][t_int]   = Action(shift, 5);
  ACTION[0][t_float] = Action(shift, 6);
  GOTO[0][e_programa]      = 1;
  GOTO[0][e_declaraciones] = 2; 
  GOTO[0][e_declaracion]   = 3;
  GOTO[0][e_tipo]          = 4;

  // ESTADO 1: Aceptación
  ACTION[1][t_eof] = Action(acc, 0);

  // ESTADO 2: Fin de declaraciones, esperando Sentencias
  ACTION[2][t_int]    = Action(shift, 5);
  ACTION[2][t_float]  = Action(shift, 6);
  // Inicio de sentencias: ID, IF o WHILE
  ACTION[2][t_id]     = Action(shift, 30); 
  ACTION[2][t_if]     = Action(shift, 40); // Nuevo: IF
  ACTION[2][t_while]  = Action(shift, 50); // Nuevo: WHILE

  GOTO[2][e_sentencias]  = 90;
  GOTO[2][e_sentencia]   = 91;
  GOTO[2][e_declaracion] = 92; 
  GOTO[2][e_tipo]        = 4;

  // ESTADO 3: Reducciones Declaraciones
  ACTION[3][t_int] = Action(reduce, 3);
  ACTION[3][t_float] = Action(reduce, 3);
  ACTION[3][t_id] = Action(reduce, 3); 
  ACTION[3][t_if] = Action(reduce, 3);
  ACTION[3][t_while] = Action(reduce, 3);

  // ESTADO 4-6, 21-25: Lógica de Declaraciones (Intacta)
  ACTION[4][t_id] = Action(shift, 21);
  GOTO[4][e_lista_var] = 22;
  ACTION[5][t_id] = Action(reduce, 5);
  ACTION[6][t_id] = Action(reduce, 6);
  ACTION[21][t_semicolon] = Action(reduce, 8);
  ACTION[21][t_comma]     = Action(reduce, 8);
  ACTION[22][t_semicolon] = Action(shift, 25);
  ACTION[22][t_comma]     = Action(shift, 23);
  ACTION[23][t_id] = Action(shift, 24);
  ACTION[24][t_semicolon] = Action(reduce, 7);
  ACTION[24][t_comma]     = Action(reduce, 7);
  ACTION[25][t_int]   = Action(reduce, 4);
  ACTION[25][t_float] = Action(reduce, 4);
  ACTION[25][t_id]    = Action(reduce, 4);
  ACTION[25][t_if]    = Action(reduce, 4);
  ACTION[25][t_while] = Action(reduce, 4);
  ACTION[25][t_eof]   = Action(reduce, 4);

  // =========================================================
  // LÓGICA DE ASIGNACIÓN (SENTENCIAS)
  // =========================================================
  
  // ESTADO 30: sentencia -> id . = expresion ;
  ACTION[30][t_assign] = Action(shift, 31);

  // ESTADO 31: sentencia -> id = . expresion ;
  ACTION[31][t_id] = Action(shift, 37);
  ACTION[31][t_num] = Action(shift, 32);
  GOTO[31][e_expresion] = 33;

  // ESTADO 32: expresion -> numero .
  ACTION[32][t_semicolon] = Action(reduce, 19);
  ACTION[32][t_plus]      = Action(reduce, 19);
  ACTION[32][t_minus]     = Action(reduce, 19);
  ACTION[32][t_mul]       = Action(reduce, 19);
  ACTION[32][t_div]       = Action(reduce, 19);
  ACTION[32][t_rparen]    = Action(reduce, 19); // Para cierres de paréntesis

  // ESTADO 37: expresion -> id .
  ACTION[37][t_semicolon] = Action(reduce, 18);
  ACTION[37][t_plus]      = Action(reduce, 18);
  ACTION[37][t_minus]     = Action(reduce, 18);
  ACTION[37][t_mul]       = Action(reduce, 18);
  ACTION[37][t_div]       = Action(reduce, 18);
  ACTION[37][t_rparen]    = Action(reduce, 18);

  // ESTADO 33: sentencia -> id = expresion . ;  Y Operaciones
  ACTION[33][t_semicolon] = Action(shift, 36);
  ACTION[33][t_plus]      = Action(shift, 34);
  ACTION[33][t_minus]     = Action(shift, 34); // Simplificamos operadores al mismo estado
  ACTION[33][t_mul]       = Action(shift, 34);
  ACTION[33][t_div]       = Action(shift, 34);

  // ESTADO 34: expresion -> expresion op . expresion
  ACTION[34][t_id]  = Action(shift, 37);
  ACTION[34][t_num] = Action(shift, 32);
  GOTO[34][e_expresion] = 35;

  // ESTADO 35: expresion -> expresion op expresion .
  ACTION[35][t_semicolon] = Action(reduce, 14); // Reducción genérica para operadores
  ACTION[35][t_plus]      = Action(reduce, 14);
  ACTION[35][t_minus]     = Action(reduce, 14);
  ACTION[35][t_mul]       = Action(reduce, 14);
  ACTION[35][t_div]       = Action(reduce, 14);
  ACTION[35][t_rparen]    = Action(reduce, 14);

  // ESTADO 36: sentencia -> id = expresion ; .
  // Aquí reducimos sentencia (Prod 11). Follow: eof, id, if, while, else
  ACTION[36][t_id]    = Action(reduce, 11);
  ACTION[36][t_if]    = Action(reduce, 11);
  ACTION[36][t_while] = Action(reduce, 11);
  ACTION[36][t_else]  = Action(reduce, 11);
  ACTION[36][t_eof]   = Action(reduce, 11);

  // =========================================================
  // LÓGICA DE IF (NUEVO)
  // =========================================================
  
  // ESTADO 40: sentencia -> if . ( expresion ) sentencias else sentencias
  ACTION[40][t_lparen] = Action(shift, 41);

  // ESTADO 41: sentencia -> if ( . expresion ) ...
  ACTION[41][t_id]  = Action(shift, 37);
  ACTION[41][t_num] = Action(shift, 32);
  GOTO[41][e_expresion] = 42;

  // ESTADO 42: sentencia -> if ( expresion . ) ...
  ACTION[42][t_rparen] = Action(shift, 43);
  ACTION[42][t_plus]   = Action(shift, 34); // Soporte operadores en el if

  // ESTADO 43: if ( expresion ) . sentencias else sentencias
  // Esperamos inicio de sentencias
  ACTION[43][t_id]    = Action(shift, 30);
  ACTION[43][t_if]    = Action(shift, 40);
  ACTION[43][t_while] = Action(shift, 50);
  GOTO[43][e_sentencias] = 45;
  GOTO[43][e_sentencia]  = 44;

  // ESTADO 44: sentencias -> sentencia . (dentro del if)
  ACTION[44][t_else] = Action(reduce, 10);
  ACTION[44][t_id]   = Action(reduce, 10); // Recursion sentencias

  // ESTADO 45: if (...) sentencias . else sentencias
  ACTION[45][t_else] = Action(shift, 46);
  ACTION[45][t_id]   = Action(shift, 30); // Recursión sentencias -> sentencias sentencia
  ACTION[45][t_if]   = Action(shift, 40);
  ACTION[45][t_while]= Action(shift, 50);
  GOTO[45][e_sentencia] = 49; // Estado especial reducción recursiva

  // ESTADO 46: else . sentencias
  ACTION[46][t_id]    = Action(shift, 30);
  ACTION[46][t_if]    = Action(shift, 40);
  ACTION[46][t_while] = Action(shift, 50);
  GOTO[46][e_sentencias] = 48;
  GOTO[46][e_sentencia]  = 47;

  // ESTADO 47: sentencias -> sentencia . (dentro del else)
  ACTION[47][t_eof]   = Action(reduce, 10);
  ACTION[47][t_id]    = Action(reduce, 10);
  ACTION[47][t_if]    = Action(reduce, 10);
  ACTION[47][t_while] = Action(reduce, 10);

  // ESTADO 48: if (...) sentencias else sentencias .
  ACTION[48][t_eof]   = Action(reduce, 12);
  ACTION[48][t_id]    = Action(reduce, 12); 
  ACTION[48][t_if]    = Action(reduce, 12);
  ACTION[48][t_while] = Action(reduce, 12);
  ACTION[48][t_else]  = Action(reduce, 12); // Para ifs anidados

  // ESTADO 49: sentencias -> sentencias sentencia . (
  ACTION[49][t_else] = Action(reduce, 9);
  ACTION[49][t_id]   = Action(reduce, 9);

  // =========================================================
  // LÓGICA DE WHILE (NUEVO)
  // =========================================================

  // ESTADO 50: sentencia -> while . ( expresion ) sentencias
  ACTION[50][t_lparen] = Action(shift, 51);

  // ESTADO 51: sentencia -> while ( . expresion ) ...
  ACTION[51][t_id]  = Action(shift, 37);
  ACTION[51][t_num] = Action(shift, 32);
  GOTO[51][e_expresion] = 52;

  // ESTADO 52: sentencia -> while ( expresion . ) ...
  ACTION[52][t_rparen] = Action(shift, 53);
  ACTION[52][t_plus]   = Action(shift, 34);

  // ESTADO 53: while ( expresion ) . sentencias
  ACTION[53][t_id]    = Action(shift, 30);
  ACTION[53][t_if]    = Action(shift, 40);
  ACTION[53][t_while] = Action(shift, 50);
  GOTO[53][e_sentencias] = 55;
  GOTO[53][e_sentencia]  = 54;

  // ESTADO 54: sentencias -> sentencia . (dentro del while)
  ACTION[54][t_eof]   = Action(reduce, 10);
  ACTION[54][t_id]    = Action(reduce, 10);
  ACTION[54][t_if]    = Action(reduce, 10);
  ACTION[54][t_while] = Action(reduce, 10);
  ACTION[54][t_else]  = Action(reduce, 10);

  // ESTADO 55: while (...) sentencias .
  ACTION[55][t_eof]   = Action(reduce, 13);
  ACTION[55][t_id]    = Action(shift, 30); // Recursión
  ACTION[55][t_if]    = Action(reduce, 13);
  ACTION[55][t_while] = Action(reduce, 13);
  ACTION[55][t_else]  = Action(reduce, 13);
  GOTO[55][e_sentencia] = 56;

  // ESTADO 56: sentencias -> sentencias sentencia . (dentro while)
  ACTION[56][t_eof]   = Action(reduce, 9);
  ACTION[56][t_id]    = Action(reduce, 9);
  ACTION[56][t_while] = Action(reduce, 9);

  // =========================================================
  // ESTADOS FINALES
  // =========================================================

  // ESTADO 90: programa -> declaraciones sentencias .
  ACTION[90][t_eof] = Action(reduce, 1);
  // Soporte para múltiples sentencias
  ACTION[90][t_id]    = Action(shift, 30); 
  ACTION[90][t_if]    = Action(shift, 40);
  ACTION[90][t_while] = Action(shift, 50);
  GOTO[90][e_sentencia] = 93; 

  // ESTADO 91: sentencias -> sentencia .
  ACTION[91][t_eof]   = Action(reduce, 10);
  ACTION[91][t_id]    = Action(reduce, 10); 
  ACTION[91][t_if]    = Action(reduce, 10);
  ACTION[91][t_while] = Action(reduce, 10);

  // ESTADO 92: declaraciones -> declaraciones declaracion .
  ACTION[92][t_int]   = Action(reduce, 2);
  ACTION[92][t_float] = Action(reduce, 2);
  ACTION[92][t_id]    = Action(reduce, 2);
  ACTION[92][t_if]    = Action(reduce, 2);
  ACTION[92][t_while] = Action(reduce, 2);
  
  // ESTADO 93: sentencias -> sentencias sentencia .
  ACTION[93][t_eof]   = Action(reduce, 9);
  ACTION[93][t_id]    = Action(reduce, 9); 
  ACTION[93][t_if]    = Action(reduce, 9);
  ACTION[93][t_while] = Action(reduce, 9);
}

int ParserLR::parse()
{
  stack<int> pila;
  pila.push(0); 
  Token a = (Token)lexer->yylex(); 

  while(true) 
  {
      int s = pila.top();
      if (ACTION[s].find(a) == ACTION[s].end()) {
          error("Error Sintactico en linea " + to_string(lexer->getLine()) + " Token: " + to_string(a));
      }

      Action accion = ACTION[s][a];

      if (accion.type == shift) 
      {
          pila.push(accion.num);
          a = (Token)lexer->yylex();
      }
      else if (accion.type == reduce) 
      {
          Production prod = grammar.getProd(accion.num);
          int tam = prod.getBody().size();
          for(int i=0; i<tam; i++) pila.pop();
          
          int t = pila.top();
          pila.push(GOTO[t][(NoTerm)prod.getHead()]);
      }
      else if (accion.type == acc) 
      {
          aceptar("Cadena aceptada correctamente.");
          return 0;
      }
  }
  return -1; 
}

void ParserLR::error(string msg)
{
  cout << msg << endl;
  exit(EXIT_FAILURE);
}

Token ParserLR::eat()
{
  return (Token)lexer->yylex();
}

void ParserLR::aceptar(string msg)
{
  cout << msg << endl;
}