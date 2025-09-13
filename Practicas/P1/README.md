<p  align="center">
  <img  width="200"  src="https://www.fciencias.unam.mx/sites/default/files/logoFC_2.png"  alt="">  <br>Compiladores  2026-1 <br>
  Práctica 1: Analizadores léxicos con Lex (Flex) <br> Profesora: Ariel Adara Mercado Martínez
</p>

## Análisis léxico con Flex
### Objetivo:
Que el alumno conozca y utilice los principios para generar analizadores léxicos utilizando Lex.

### Introducción
Lex es una herramienta para generar analizadores léxicos, que se deben describir mediante las expresiones regulares de los tokens que serán reconocidas por el analizador léxico (scanner o lexer). Originalmente fue desarrollado para el sistema operativo Unix, pero con la popularidad de Linux se creo una versión para este sistema llamada Flex.

#### Estructura de un archivo Lex
Un programa en LEX consta de tres secciones:
```
Seccion de declaraciones
%%
Sección de expresiones regulares
%%
Sección de código de usuario (código en lenguaje C++)
```

#### Sección de declaraciones
* __Directivas de código c++.__ Se utilizan para incluir los archivos de biblioteca y definir las variables globales es la siguiente:
```lex
%{
  #include <iostream>
  int contador;
%}
```
* __Macros o definiciones.__ Las macros son variables a las que se les asignar una expresión regular. P. ej. ```letra [a-zA-Z]```, la macro
se llama _letra_, separada por un espacio de la definición de su expresión regular.
* __Directivas de Lex__. Las directivas u opciones de escáner le indican a Lex que realice tareas extras al momento de generar el analizador léxico.
    * ```%option yylineno``` genera un contador de lı́neas automáticamente.
    * ```%option noyywrap``` le indica a lex que debe generar de forma automática la función yywrap.
    * ```%x``` o ```%s``` es para declarar estados léxicos.

#### Sección de expresiones regulares
* __Estructura__:
```Expresión Regular (espacio, nunca salto de lı́nea) {Acción Léxica} ```
   * __Expresión Regular__: debe estar escrita con la sintaxis de Lex
   * __Acción Léxica__: se ejecuta cada vez que se encuentra una cadena que coincida con la expresión regular y es escrita
en lenguaje C++, encerrada por llaves.

#### Metacaracteres

| Caracter | Descripción |
|----------|-------------|
|c         |Cualquier carácter representado por c que no sea un operador|
|\c        |El carácter c literalmente|
|"S"       |La cadena s, literalmente|
|.         |Cualquier carácter excepto el salto de lı́nea|
|∧         |Inicio de línea|
|$         |Fin de lı́nea|
|[s]       |Cualquier carácter incluido dentro de la cadena s|
|[^s]      |Cualquier carácter que no esté dentro de s|
|\n        |Salto de lı́nea|
|*         |Cerradura de Kleene|
|+         |Cerradura positiva|
|\|        |Disyunción|
|?         |Cero o una instancia|
|{m, n}    |Entre m y n instancias de una expresión que le antecede



#### Sección de código de usuario
En esta sección se escriben las funciones auxiliares para realizar el análisis léxico, por lo general es donde se agrega a
main.

## Primer programa en Flex++

### Instalación de flex
* Para Debian o Ubuntu: ```sudo apt-get install flex```
* Para Suse u OpenSuse: ```sudo zypper in flex```
### Para comprobar que se ha instalado correctamente:
```flex −−version```
### Primer programa en lex


```C++ 
%{
  #include <iostream>
%}

%option c++
%option noyywrap


digito [0-9]
letra [a-zA-Z]
palabra {letra}+
espacio [ \t\n]

%%

{espacio} {/* La acción léxica puede ir vacía si queremos que el escáner ignore la regla*/}
{digito}+ { std::cout << "Encontré un número: " << yytext << std::endl; }
{palabra} { std::cout << "Encontré una palabra: " << yytext << std::endl; }

%%


int main() {
  FlexLexer* lexer = new yyFlexLexer;
  lexer->yylex();
}

```

#### Pasos
a. Transcribir el código anterior a un archivo con extensión .ll, .lex o .flex dentro de la carpeta *src/__Primer programa en Lex__/* <br>
b. Compilar mediante la instrucción: ```flex++ archivo.ll``` <br>
c. Comprobar se genero el archivo _lex.yy.cc_ <br>
d. Compilar mediante: ```g++ lex.yy.cc -o nombreEjecutable``` <br>
e. Ejecutar mediante: ```./nombreEjecutable```

#### Ejercicios 
1. ¿Qué ocurre si en la primera sección se quitan las llaves al nombre de la macro letra? (0.5 pts)
2. ¿Qué ocurre si en la segunda sección se quitan las llaves a las macros? (0.5 pts)
3. ¿Cómo se escribe un comentario en flex? (0.5 pts)
4. ¿Qué se guarda en yytext? (0.5 pts)
5. ¿Qué pasa al ejecutar el programa e introducir cadenas de caracteres y de dígitos por la consola? (0.5 pts)
6. ¿Qué ocurre si introducimos caracteres como "\*" en la consola? (0.5 pts)
7. Modificar al código anterior en un archivo nuevo, de tal manera que reconozca lo siguiente: (2 pts)
    1. La expresión regular para los hexadecimales en lenguaje C++.
    2. 5 palabras reservadas del lenguaje C++.
    3. Los identificadores válidos del lenguaje C++, con longitud máxima de 32 caracteres (**Sugerencia**: use el operador {m,n}).
    4. Los espacios en blanco.


  1. ¿Qué ocurre si en la primera sección se quitan las llaves al nombre de la macro letra?
- Da error de compilación, porque el código C++ debe ir dentro de %{ ... %}.

2. ¿Qué ocurre si en la segunda sección se quitan las llaves a las acciones?
- Da error de sintaxis, porque Flex requiere llaves para delimitar las acciones de las expresiones regulares.

3. ¿Cómo se escribe un comentario en Flex?
- Igual que en C++: // comentario de línea o /* comentario de varias líneas */

4. ¿Qué se guarda en yytext?
- Contiene la cadena que coincide con la expresión regular actualmente reconocida por el lexer.

5. ¿Qué pasa al ejecutar el programa e introducir cadenas de caracteres y de dígitos por la consola?
- Cada token reconocido ejecuta su acción: los números muestran "Encontré un número" y las palabras "Encontré una palabra". Los espacios se ignoran si así está definido.

6. ¿Qué ocurre si introducimos caracteres como "*" en la consola?
- Si no hay regla que los reconozca, se ejecuta la regla por defecto (.) o se ignoran, mostrando posiblemente "Caracter no reconocido".

7. Modificación al código para reconocer nuevos tokens:

Código:
```
%{
#include <iostream>
%}

%option c++
%option noyywrap

digito [0-9]
letra [a-zA-Z]
identificador {letra}({letra}|{digito}|_){0,31}
hexadecimal 0[xX][0-9a-fA-F]+
espacio [ \t\n]
palabra_reservada int|float|if|else|while

%%

{espacio} { /* ignorar */ }
{hexadecimal} { std::cout << "Hexadecimal: " << yytext << std::endl; }
{palabra_reservada} { std::cout << "Palabra reservada: " << yytext << std::endl; }
{identificador} { std::cout << "Identificador: " << yytext << std::endl; }

. { std::cout << "Caracter no reconocido: " << yytext << std::endl; }

%%

int main() {
    FlexLexer* lexer = new yyFlexLexer;
    lexer->yylex();
}
```

---

## Analizador léxico para el lenguaje C_1


### Estructura del directorio
```c++
p2
├── README.md
└── src
    ├── C_1
    │   ├── Lexer.hpp //archivo de cabecera del analizador
    │   ├── lexer.ll //definición del analizador léxico en Flex 
    │   ├── main.cpp //contiene la función principal del programa
    │   ├── prueba //archivo de entrada para el analizador léxico
    │   └── tokens.hpp //definición de tokens
    └── Primer programa en Lex
```

### Uso

#### Compilación

```bash
$ cd src/
$ flex++ lexer.ll
$ g++ Lexer.cpp main.cpp -o compiler
```

#### Ejecución

```bash
$ ./compiler prueba
```


#### Salida esperada
```
12, int
15, float
11, if
14, else
13, while
12, int
16, 12345
16, 1.2e6
10, a1
10, a_23
10, ___
10, id2
10, if3
10, while4
10, _b
9, ;
8, ,
6, (
7, )
12, int
7, )
10, a
10, _qbc
```


#### Ejercicios

8. Describir el conjunto de terminales y la expresión regular que reconoce a cada uno  en _lexer.ll_. (2 pts)

Conjunto de terminales y expresiones regulares en `lexer.ll`

| Terminal        | Expresión regular en `lexer.ll`                     | Descripción                                      |
|-----------------|--------------------------------------------------|-------------------------------------------------|
| `int`           | `"int"`                                          | Palabra reservada `int`                         |
| `float`         | `"float"`                                        | Palabra reservada `float`                       |
| `if`            | `"if"`                                           | Palabra reservada `if`                          |
| `else`          | `"else"`                                         | Palabra reservada `else`                        |
| `while`         | `"while"`                                        | Palabra reservada `while`                       |
| Identificador   | `[a-zA-Z_][a-zA-Z0-9_]*`                        | Nombres de variables o funciones válidos       |
| Número entero   | `[0-9]+`                                         | Secuencia de dígitos                            |
| Número flotante | `[0-9]+(\.[0-9]*)?([eE][+-]?[0-9]+)?`          | Números con decimal o notación científica      |
| `;`             | `";"`                                            | Punto y coma                                   |
| `,`             | `","`                                            | Coma                                           |
| `(`             | `"("`                                            | Paréntesis izquierdo                           |
| `)`             | `")"`                                            | Paréntesis derecho                              |
| Otros           | `.`                                              | Cualquier otro carácter (genera error léxico) |

**Notas:**

- Las palabras reservadas se escriben entre comillas `"..."` en `lexer.ll`.  
- Los identificadores y números usan clases de caracteres y patrones (`[a-zA-Z_][a-zA-Z0-9_]*`, `[0-9]+`).  
- Los espacios y saltos de línea se ignoran con `[ \t\n]+`.  
- Cualquier otro carácter no reconocido se captura con `.` y genera un error léxico.




9. Generar acciones léxicas para cada terminal de nuestro lenguaje en _Lexer.cpp_, de modo que se muestre en pantalla la slida esperada con el archivo _prueba_. (2 pts)
10. Crear un _Makefile_. (1 pt)


Se implementaron todas las acciones léxicas en `lexer.ll` para cada terminal del lenguaje. Esto permite que al ejecutar el compilador con el archivo de prueba `prueba.txt` se genere la salida esperada, mostrando el token y su lexema correspondiente.

Además, se creó un `Makefile` que facilita la compilación y ejecución del proyecto:

- `make` → compila el lexer y el programa principal.
- `make run` → ejecuta el compilador con `prueba.txt`.
- `make clean` → elimina archivos generados (`Lexer.cpp` y `compiler`).

### Salida final al correr `make run`:

```
12, int
15, float
11, if
14, else
13, while
12, int
16, 12345
16, 1.2e6
10, a1
10, a_23
10, ___
10, id2
10, if3
10, while4
10, _b
9, ;
8, ,
6, (
7, )
12, int
7, )
10, a
10, _qbc
```


---
#### Extras

11. Incluir lo necesario para producir la misma salida que en la práctica 1 (2 pts.)
12. Documentar el código. (0.25pts)
13. Proponer 4 archivos de prueba nuevos, 2 válidos y 2 inválidos. (0.25pts)





