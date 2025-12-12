%{
#include <stdio.h>
#include <ctype.h>
%}

%option noyywrap

%%

[a-z]+  {
            /* Recorremos la palabra letra por letra para hacerla mayúscula */
            for (int i = 0; yytext[i] != '\0'; i++) {
                printf("%c", toupper(yytext[i]));
            }
        }

.|\n    { ECHO; /* Lo que no sea minúscula se imprime tal cual */ }

%%

int main(int argc, char **argv) {
    if (argc > 1) yyin = fopen(argv[1], "r");
    
    yylex();
    return 0;
}