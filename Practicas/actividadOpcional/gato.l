%{
#include <stdio.h>
#include <stdbool.h>

char board[9]; 
int count = 0; 

bool checkWin(char p) {
    if (board[0] == p && board[1] == p && board[2] == p) return true;
    if (board[3] == p && board[4] == p && board[5] == p) return true;
    if (board[6] == p && board[7] == p && board[8] == p) return true;
    if (board[0] == p && board[3] == p && board[6] == p) return true;
    if (board[1] == p && board[4] == p && board[7] == p) return true;
    if (board[2] == p && board[5] == p && board[8] == p) return true;
    if (board[0] == p && board[4] == p && board[8] == p) return true;
    if (board[2] == p && board[4] == p && board[6] == p) return true;
    return false;
}

void analyzeBoard() {
    bool xWins = checkWin('X');
    bool oWins = checkWin('O');
    
    int emptyCells = 0;
    for (int i = 0; i < 9; i++) {
        if (board[i] == '_') emptyCells++;
    }

    if (xWins && !oWins) printf("gana X\n");
    else if (oWins && !xWins) printf("gana O\n");
    else {
        if (emptyCells == 0) printf("empate\n");
        else printf("sin ganador\n");
    }
    count = 0;
}
%}

%%

[XO0_] { 
   /* TRUCO: Si viene un 0, lo guardamos como O */
   char c = yytext[0];
   if (c == '0') c = 'O';
   
   board[count++] = c;
   if (count == 9) analyzeBoard();
}

\n { ; }
.  { ; }

%%

int main(int argc, char **argv) {
    if (argc > 1) yyin = fopen(argv[1], "r");
    yylex();
    return 0;
}
int yywrap() { return 1; }
