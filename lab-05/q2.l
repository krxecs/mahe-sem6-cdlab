%{
#include <stdio.h>

int lines = 0;
int words = 0;
int chars = 0;
int blanks = 0;
%}

%%
[a-zA-Z0-9]+    { words++; chars += yyleng; }
[ \t]           { blanks++; chars++; }
\n              { lines++; chars++; }
.               { chars++; }
%%

int yywrap() {
  return 1;
}

int main() {
  puts("Enter text (Ctrl+D to end):");
  yylex();

  // Check if line count needs adjustment for last line without newline
  if (chars > 0 && lines == 0) lines = 1;

  printf("Lines: %d\n", lines);
  printf("Words: %d\n", words);
  printf("Characters: %d\n", chars);
  printf("Blanks: %d\n", blanks);
  return 0;
}
