%{
#include <stdio.h>
int word_count = 0;
int line_count = 0;
int char_count = 0;
%}

%%
\n          { line_count++; char_count++; }
[ \t]+      { char_count += yyleng; }
[^ \t\n]+   { word_count++; char_count += yyleng; }
%%

int yywrap() {
    return 1;
}

int main() {
    printf("Enter text (Ctrl+D to stop):\n");
    yylex();
    printf("\nWords      : %d\n", word_count);
    printf("Lines      : %d\n", line_count);
    printf("Characters : %d\n", char_count);
    return 0;
}