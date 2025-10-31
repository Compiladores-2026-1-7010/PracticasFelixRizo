#include "headers/ParserLL.hpp"

static const int BASE_NT = 100; // cualquier offset > #de terminales

static inline int idT(Token t)        { return static_cast<int>(t); }
static inline int idN(NoTerm nt)      { return BASE_NT + static_cast<int>(nt); }

ParserLL::ParserLL(Lexer *lexer)
{
    this->lexer = lexer;
    loadSyms();
    loadProds();
    loadTable();
}

ParserLL::~ParserLL()
{
}

void ParserLL::loadSyms()
{
  /*********************************************
   * 4. Agregar todos los símbolos (N'∪T) de G'*
   *********************************************/

  grammar.setNum(17, 15);

  //Terminales (Σ) - (Modificados para usar t_int, t_id, etc.)
  grammar.addSymbol(idT(t_int), Symbol(t_int));
  grammar.addSymbol(idT(t_float), Symbol(t_float));
  grammar.addSymbol(idT(t_if), Symbol(t_if));
  grammar.addSymbol(idT(t_else), Symbol(t_else));
  grammar.addSymbol(idT(t_while), Symbol(t_while));
  grammar.addSymbol(idT(t_id), Symbol(t_id));
  grammar.addSymbol(idT(t_num), Symbol(t_num));
  grammar.addSymbol(idT(t_assign), Symbol(t_assign));
  grammar.addSymbol(idT(t_pyc), Symbol(t_pyc));
  grammar.addSymbol(idT(t_coma), Symbol(t_coma));
  grammar.addSymbol(idT(t_mas), Symbol(t_mas));
  grammar.addSymbol(idT(t_menos), Symbol(t_menos));
  grammar.addSymbol(idT(t_mul), Symbol(t_mul));
  grammar.addSymbol(idT(t_div), Symbol(t_div));
  grammar.addSymbol(idT(t_lparen), Symbol(t_lparen));
  grammar.addSymbol(idT(t_rparen), Symbol(t_rparen));
  grammar.addSymbol(idT(t_eof), Symbol(t_eof)); // $

  // No terminales (N′)
  grammar.addSymbol(idN(PROGRAMA), Symbol(PROGRAMA));
  grammar.addSymbol(idN(DECLARACIONES), Symbol(DECLARACIONES));
  grammar.addSymbol(idN(DECLARACIONES_P), Symbol(DECLARACIONES_P));
  grammar.addSymbol(idN(DECLARACION), Symbol(DECLARACION));
  grammar.addSymbol(idN(TIPO), Symbol(TIPO));
  grammar.addSymbol(idN(LISTA_VAR), Symbol(LISTA_VAR));
  grammar.addSymbol(idN(LISTA_VAR_P), Symbol(LISTA_VAR_P));
  grammar.addSymbol(idN(SENTENCIAS), Symbol(SENTENCIAS));
  grammar.addSymbol(idN(SENTENCIAS_P), Symbol(SENTENCIAS_P));
  grammar.addSymbol(idN(SENTENCIA), Symbol(SENTENCIA));
  grammar.addSymbol(idN(EXPRESION), Symbol(EXPRESION));
  grammar.addSymbol(idN(EXPRESION_P), Symbol(EXPRESION_P));
  grammar.addSymbol(idN(TERMINO), Symbol(TERMINO));
  grammar.addSymbol(idN(TERMINO_P), Symbol(TERMINO_P));
  grammar.addSymbol(idN(FACTOR), Symbol(FACTOR));

  // Símbolo inicial
  grammar.start = idN(PROGRAMA);
}

void ParserLL::loadProds()
{
  /************************************************
   * 5. Agregar todas las producciones (P') de G' *
   ************************************************/
  // Nota: ε se representa con body vacío ({}).

  // 1)  programa       → declaraciones sentencias
  grammar.addProd( 1, Production(idN(PROGRAMA), { idN(DECLARACIONES), idN(SENTENCIAS) }) );

  // 2)  declaraciones  → declaracion declaraciones'
  grammar.addProd( 2, Production(idN(DECLARACIONES), { idN(DECLARACION), idN(DECLARACIONES_P) }) );

  // 3)  declaraciones' → declaracion declaraciones'
  grammar.addProd( 3, Production(idN(DECLARACIONES_P),{ idN(DECLARACION), idN(DECLARACIONES_P) }) );

  // 4)  declaraciones' → ε
  grammar.addProd( 4, Production(idN(DECLARACIONES_P),{ }) );

  // 5)  declaracion    → tipo lista_var ;
  grammar.addProd( 5, Production(idN(DECLARACION), { idN(TIPO), idN(LISTA_VAR), idT(t_pyc) }) );

  // 6)  tipo           → int
  grammar.addProd( 6, Production(idN(TIPO), { idT(t_int) }) );

  // 7)  tipo           → float
  grammar.addProd( 7, Production(idN(TIPO), { idT(t_float) }) );

  // 8)  lista_var      → identificador lista_var'
  grammar.addProd( 8, Production(idN(LISTA_VAR), { idT(t_id), idN(LISTA_VAR_P) }) );

  // 9)  lista_var'     → , identificador lista_var'
  grammar.addProd( 9, Production(idN(LISTA_VAR_P), { idT(t_coma), idT(t_id), idN(LISTA_VAR_P) }) );

  // 10) lista_var'     → ε
  grammar.addProd(10, Production(idN(LISTA_VAR_P), { }) );

  // 11) sentencias     → sentencia sentencias'
  grammar.addProd(11, Production(idN(SENTENCIAS), { idN(SENTENCIA), idN(SENTENCIAS_P) }) );

  // 12) sentencias'    → sentencia sentencias'
  grammar.addProd(12, Production(idN(SENTENCIAS_P), { idN(SENTENCIA), idN(SENTENCIAS_P) }) );

  // 13) sentencias'    → ε
  grammar.addProd(13, Production(idN(SENTENCIAS_P), { }) );

  // 14) sentencia      → identificador = expresion ;
  grammar.addProd(14, Production(idN(SENTENCIA), { idT(t_id), idT(t_assign), idN(EXPRESION), idT(t_pyc) }) );

  // 15) sentencia      → if ( expresion ) sentencias else sentencias
  grammar.addProd(15, Production(idN(SENTENCIA), { idT(t_if), idT(t_lparen), idN(EXPRESION), idT(t_rparen),
                                                        idN(SENTENCIAS), idT(t_else), idN(SENTENCIAS) }) );

  // 16) sentencia      → while ( expresion ) sentencias
  grammar.addProd(16, Production(idN(SENTENCIA), { idT(t_while), idT(t_lparen), idN(EXPRESION), idT(t_rparen),
                                                        idN(SENTENCIAS) }) );

  // 17) expresion      → termino expresion'
  grammar.addProd(17, Production(idN(EXPRESION), { idN(TERMINO), idN(EXPRESION_P) }) );

  // 18) expresion'     → + termino expresion'
  grammar.addProd(18, Production(idN(EXPRESION_P), { idT(t_mas), idN(TERMINO), idN(EXPRESION_P) }) );

  // 19) expresion'     → - termino expresion'
  grammar.addProd(19, Production(idN(EXPRESION_P), { idT(t_menos), idN(TERMINO), idN(EXPRESION_P) }) );

  // 20) expresion'     → ε
  grammar.addProd(20, Production(idN(EXPRESION_P), { }) );

  // 21) termino        → factor termino'
  grammar.addProd(21, Production(idN(TERMINO), { idN(FACTOR), idN(TERMINO_P) }) );

  // 22) termino'       → * factor termino'
  grammar.addProd(22, Production(idN(TERMINO_P), { idT(t_mul), idN(FACTOR), idN(TERMINO_P) }) );

  // 23) termino'       → / factor termino'
  grammar.addProd(23, Production(idN(TERMINO_P), { idT(t_div), idN(FACTOR), idN(TERMINO_P) }) );

  // 24) termino'       → ε
  grammar.addProd(24, Production(idN(TERMINO_P), { }) );

  // 25) factor         → identificador
  grammar.addProd(25, Production(idN(FACTOR), { idT(t_id) }) );

  // 26) factor         → numero
  grammar.addProd(26, Production(idN(FACTOR), { idT(t_num) }) );

  // 27) factor         → ( expresion )
  grammar.addProd(27, Production(idN(FACTOR), { idT(t_lparen), idN(EXPRESION), idT(t_rparen) }) );
}

void ParserLL::loadTable()
{
  /*************************************************
   ** 6.Cargar la tabla de AS predictivo      **
   * (Modificada para usar t_int, t_id, etc.)
   *************************************************/
  table.clear();

  // PROGRAMA
  table[PROGRAMA][t_int]   = 1;
  table[PROGRAMA][t_float] = 1;

  // DECLARACIONES
  table[DECLARACIONES][t_int]   = 2;
  table[DECLARACIONES][t_float] = 2;

  // DECLARACIONES'
  table[DECLARACIONES_P][t_int]   = 3;
  table[DECLARACIONES_P][t_float] = 3;
  table[DECLARACIONES_P][t_id]    = 4;
  table[DECLARACIONES_P][t_if]    = 4;
  table[DECLARACIONES_P][t_while] = 4;

  // DECLARACION
  table[DECLARACION][t_int]   = 5;
  table[DECLARACION][t_float] = 5;

  // TIPO
  table[TIPO][t_int]   = 6;
  table[TIPO][t_float] = 7;

  // LISTA_VAR
  table[LISTA_VAR][t_id] = 8;

  // LISTA_VAR'
  table[LISTA_VAR_P][t_coma] = 9;
  table[LISTA_VAR_P][t_pyc]  = 10;

  // SENTENCIAS
  table[SENTENCIAS][t_id]    = 11;
  table[SENTENCIAS][t_if]    = 11;
  table[SENTENCIAS][t_while] = 11;

  // SENTENCIAS'
  table[SENTENCIAS_P][t_id]    = 12;
  table[SENTENCIAS_P][t_if]    = 12;
  table[SENTENCIAS_P][t_while] = 12;
  table[SENTENCIAS_P][t_else]  = 13;
  table[SENTENCIAS_P][t_eof]   = 13;

  // SENTENCIA
  table[SENTENCIA][t_id]    = 14;
  table[SENTENCIA][t_if]    = 15;
  table[SENTENCIA][t_while] = 16;

  // EXPRESION
  table[EXPRESION][t_id]     = 17;
  table[EXPRESION][t_num]    = 17;
  table[EXPRESION][t_lparen] = 17;

  // EXPRESION'
  table[EXPRESION_P][t_mas]    = 18;
  table[EXPRESION_P][t_menos]  = 19;
  table[EXPRESION_P][t_rparen] = 20;
  table[EXPRESION_P][t_pyc]    = 20;

  // TERMINO
  table[TERMINO][t_id]     = 21;
  table[TERMINO][t_num]    = 21;
  table[TERMINO][t_lparen] = 21;

  // TERMINO'
  table[TERMINO_P][t_mul]    = 22;
  table[TERMINO_P][t_div]    = 23;
  table[TERMINO_P][t_mas]    = 24;
  table[TERMINO_P][t_menos]  = 24;
  table[TERMINO_P][t_rparen] = 24;
  table[TERMINO_P][t_pyc]    = 24;

  // FACTOR
  table[FACTOR][t_id]    = 25;
  table[FACTOR][t_num]   = 26;
  table[FACTOR][t_lparen]= 27;
}

int ParserLL::parse()
{
  /**************************************************
   ** 7. Implementar el algoritmo de AS predictivo **
   **************************************************/

  // Pila de símbolos de gramática
  stack<Symbol> pila;

  // Inicialización: $, S
  pila.push(Symbol(t_eof)); // Modificado
  pila.push(grammar.getSym(grammar.start));

  // Primer token
  token = eat();

  // Bucle principal
  while(true){
    Symbol X = pila.top(); pila.pop();

    if (X.getType() == terminal) {
      // Coincidencia terminal vs token actual
      if (X.getToken() == token) {
        // Si ambos son END (t_eof), éxito
        if (token == t_eof) { // Modificado
          aceptar("Cadena aceptada por el analizador LL(1).");
          return 0;
        }
        // Consumir y seguir
        token = eat();
      } else {
        error("Token inesperado durante el análisis.");
        return 1;
      }
    } else {
      // No terminal
      NoTerm A = X.getNoTerm();
      map<Token,int>::iterator it = table[A].find(token);
      if (it == table[A].end()) {
        error("Entrada inválida: no hay regla para el par (NoTerm, Token).");
        return 1;
      }
      int pid = it->second;

      // Expandir con la producción pid
      Production prod = grammar.getProd(pid);
      vector<int> cuerpo = prod.getBody();

      // Empujar RHS en orden inverso 
      for (int i = static_cast<int>(cuerpo.size()) - 1; i >= 0; --i) {
        pila.push(grammar.getSym(cuerpo[i]));
      }
    }
  }

  // No debería alcanzarse
  return 0;
}

void ParserLL::error(string msg)
{
    cout << msg << endl;
    exit(EXIT_FAILURE);
}

Token ParserLL::eat()
{
    return (Token) lexer->yylex();
}

void ParserLL::aceptar(string msg)
{
    cout << msg << endl;
}