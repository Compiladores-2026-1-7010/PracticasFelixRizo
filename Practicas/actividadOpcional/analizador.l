%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Variable para contar líneas */
int line_num = 1;
/* Puntero al archivo de salida */
FILE *out_file;

/* Función auxiliar para imprimir tokens en el formato solicitado */
void write_token(const char *clase, const char *valor) {
    fprintf(out_file, "<%s, %s>\n", clase, valor);
}

/* Función para reportar errores */
void report_error(const char *msg, char c) {
    fprintf(out_file, "Error: %s '%c' en la linea %d\n", msg, c, line_num);
    printf("Error léxico encontrado en línea %d. Ver archivo .tokens\n", line_num);
}
%}

/* Estado exclusivo para manejar comentarios multilínea correctamente */
%x MULTI_COMMENT

/* Definiciones de expresiones regulares comunes */
DIGIT       [0-9]
LETTER      [a-zA-Z_]
ID          {LETTER}({LETTER}|{DIGIT})*

%%

    /* --- e. PALABRAS RESERVADAS --- */
"if"        { write_token("KEYWORD", yytext); }
"then"      { write_token("KEYWORD", yytext); }
"else"      { write_token("KEYWORD", yytext); }
"while"     { write_token("KEYWORD", yytext); }
"do"        { write_token("KEYWORD", yytext); }
"case"      { write_token("KEYWORD", yytext); }
"is"        { write_token("KEYWORD", yytext); }
"void"      { write_token("KEYWORD", yytext); }
"true"      { write_token("KEYWORD", yytext); }
"false"     { write_token("KEYWORD", yytext); }
"begin"     { write_token("KEYWORD", yytext); }
"end"       { write_token("KEYWORD", yytext); }
"not"       { write_token("KEYWORD", yytext); }

    /* --- b. NÚMEROS REALES (Debe ir antes de enteros por prioridad) --- */
{DIGIT}+\.{DIGIT}+  { write_token("REAL", yytext); }

    /* --- a. NÚMEROS ENTEROS --- */
{DIGIT}+            { write_token("INTEGER", yytext); }

    /* --- c. IDENTIFICADORES ESTILO C --- */
{ID}                { write_token("ID", yytext); }

    /* --- d. CADENAS DE CARACTERES --- */
\"[^\"\n]*\"        { write_token("STRING", yytext); }

    /* --- g. COMENTARIOS UNA LÍNEA (-- hasta el final) --- */
"--".* { write_token("COMMENT_LINE", yytext); }

    /* --- h. COMENTARIOS MULTILÍNEA (<* ... *>) --- */
"<*"                { BEGIN(MULTI_COMMENT); /* Entramos al estado comentario */ }
<MULTI_COMMENT>"*>" { 
                      write_token("COMMENT_BLOCK", "bloque_comentario"); 
                      BEGIN(INITIAL); /* Volvemos al estado normal */ 
                    }
<MULTI_COMMENT>\n   { line_num++; /* Contamos líneas dentro del comentario */ }
<MULTI_COMMENT>.    { /* Ignoramos el contenido del comentario */ }

    /* --- k. OPERADOR DE ASIGNACIÓN --- */
":="                { write_token("ASSIGN_OP", yytext); }

    /* --- j. OPERADORES RELACIONALES --- */
"<>"                { write_token("REL_OP", yytext); }
"<"                 { write_token("REL_OP", yytext); }
">"                 { write_token("REL_OP", yytext); }
"="                 { write_token("REL_OP", yytext); }

    /* --- i. OPERADORES ARITMÉTICOS --- */
"+"                 { write_token("ARITH_OP", yytext); }
"-"                 { write_token("ARITH_OP", yytext); }
"*"                 { write_token("ARITH_OP", yytext); }
"/"                 { write_token("ARITH_OP", yytext); }
"%"                 { write_token("ARITH_OP", yytext); }

    /* --- l. SÍMBOLOS ESPECIALES --- */
"("                 { write_token("SYMBOL", yytext); }
")"                 { write_token("SYMBOL", yytext); }
"{"                 { write_token("SYMBOL", yytext); }
"}"                 { write_token("SYMBOL", yytext); }
";"                 { write_token("SYMBOL", yytext); }
","                 { write_token("SYMBOL", yytext); }

    /* --- f. ESPACIOS EN BLANCO --- */
[ \t]+              { write_token("WHITESPACE", " "); }
\n                  { line_num++; write_token("WHITESPACE", "\\n"); }

    /* --- m. MANEJO DE ERRORES --- */
.                   { report_error("Caracter desconocido", yytext[0]); }

%%

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Uso: ./analizador archivo.art\n");
        return 1;
    }

    char *filename = argv[1];
    
    /* Validación simple de extensión .art */
    char *ext = strrchr(filename, '.');
    if (!ext || strcmp(ext, ".art") != 0) {
        printf("Error: El archivo de entrada debe tener extension .art\n");
        return 1;
    }

    /* Preparar nombre de archivo de salida (.tokens) */
    char out_filename[256];
    strncpy(out_filename, filename, ext - filename);
    out_filename[ext - filename] = '\0';
    strcat(out_filename, ".tokens");

    /* Abrir archivos */
    yyin = fopen(filename, "r");
    if (!yyin) {
        perror("Error abriendo archivo de entrada");
        return 1;
    }

    out_file = fopen(out_filename, "w");
    if (!out_file) {
        perror("Error creando archivo de salida");
        return 1;
    }

    /* Ejecutar análisis */
    yylex();

    /* Cerrar y limpiar */
    fclose(yyin);
    fclose(out_file);

    printf("Analisis completado. Resultado guardado en: %s\n", out_filename);

    return 0;
}

int yywrap() {
    return 1;
}