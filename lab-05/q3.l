%{
#include <stdio.h>

int pos_int = 0;
int neg_int = 0;
int pos_float = 0;
int neg_float = 0;
%}

%%
\+?[0-9]+\.[0-9]+  { pos_float++; }
-[0-9]+\.[0-9]+    { neg_float++; }
\+?[0-9]+          { pos_int++; }
-[0-9]+            { neg_int++; }
.|\n               { /* Ignore other characters */ }
%%

int yywrap() {
  return 1;
}

int main() {
  puts("Enter numbers (Ctrl+D to end):");
  yylex();
  printf("Positive Integers: %d\n", pos_int);
  printf("Negative Integers: %d\n", neg_int);
  printf("Positive Floats: %d\n", pos_float);
  printf("Negative Floats: %d\n", neg_float);
  return 0;
}
