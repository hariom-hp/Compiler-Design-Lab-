%{
#include <stdio.h>
int char_count = 0;
int word_count = 0;
int line_count = 0;
%}

%%
\n              { line_count++; char_count++; }
[ \t]+          { char_count += yyleng; }
[^ \t\n]+       { word_count++; char_count += yyleng; }
%%

int yywrap() {
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc == 2) {
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            fprintf(stderr, "Error: Cannot open file %s\n", argv[1]);
            return 1;
        }
        yyin = file;
    } else {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    yylex();

    printf("Number of characters : %d\n", char_count);
    printf("Number of words      : %d\n", word_count);
    printf("Number of lines      : %d\n", line_count);

    fclose(yyin);
    return 0;
}