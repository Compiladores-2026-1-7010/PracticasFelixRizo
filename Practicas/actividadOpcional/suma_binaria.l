%{
#include <stdio.h>
#include <stdlib.h>

int suma_total = 0;
%}

%option noyywrap

%%

0b[01]+   { 
            /* Saltamos los dos primeros caracteres (0b) y convertimos base 2 */
            suma_total += strtol(yytext + 2, NULL, 2); 
          }

.|\n      { /* Ignoramos cualquier otra cosa que no sea binario */ }

%%

int main(int argc, char **argv) {
    if (argc > 1) yyin = fopen(argv[1], "r");
    
    yylex();
    
    /* Imprimimos el resultado final */
    printf("La suma total es: %d\n", suma_total);
    return 0;
}