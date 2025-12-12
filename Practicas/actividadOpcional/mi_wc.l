%{
#include <stdio.h>

int lineas = 0;
int palabras = 0;
int caracteres = 0;
%}

%option noyywrap

%%

\n          { lineas++; caracteres++; }
[^ \t\n]+   { palabras++; caracteres += yyleng; }
.           { caracteres++; }

%%

int main(int argc, char **argv) {
    /* Si se pasa un argumento, intenta abrir el archivo */
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror("Error al abrir el archivo");
            return 1;
        }
    }

    yylex();

    /* Imprime el resultado en formato: lineas palabras caracteres */
    printf("%d %d %d\n", lineas, palabras, caracteres);

    return 0;
}