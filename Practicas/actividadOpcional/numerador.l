%{
#include <stdio.h>
int numero_linea = 1;
%}

%option noyywrap

%%

\n      { printf("\n%d: ", ++numero_linea); }
.       { ECHO; }

%%

int main(int argc, char **argv) {
    /* Si se pasa un archivo como argumento, lo abre */
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror("Error abriendo el archivo");
            return 1;
        }
    }

    /* Imprime el número de la primera línea antes de comenzar a escanear */
    printf("%d: ", numero_linea);
    
    yylex();
    
    printf("\n");
    
    return 0;
}