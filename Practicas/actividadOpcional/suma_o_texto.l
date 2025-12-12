%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int suma_actual = 0;
char linea_buffer[1024] = ""; 
%}

%%

[0-9]+ {
   suma_actual += atoi(yytext);
   strcat(linea_buffer, yytext);
}

A {
   printf("%d\n", suma_actual);
   suma_actual = 0;
   linea_buffer[0] = '\0';
}

B {
   printf("%sB\n", linea_buffer);
   suma_actual = 0;
   linea_buffer[0] = '\0';
}

\n { 
   suma_actual = 0;
   linea_buffer[0] = '\0';
}

. { 
   /* Capturamos cualquier otra cosa (letras, espacios) para el buffer */
   strcat(linea_buffer, yytext); 
}

%%

int main(int argc, char **argv) {
    if (argc > 1) yyin = fopen(argv[1], "r");
    yylex();
    return 0;
}
int yywrap() { return 1; }
