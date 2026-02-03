%{
#include <stdio.h>
#include <stdlib.h>
%}

%%
[0-9]+      {
                int num = atoi(yytext);
                printf("%X", num);
            }
.|\n        { ECHO; }
%%

int yywrap() {
  return 1;
}

int main() {
  puts("Enter decimal numbers mixed with text (Ctrl+D to end):");
  yylex();
  return 0;
}
