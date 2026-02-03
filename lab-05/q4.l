%{
#include <stdio.h>

int scanf_count = 0;
int printf_count = 0;
%}

%%
"scanf"     { fprintf(yyout, "READ"); scanf_count++; }
"printf"    { fprintf(yyout, "WRITE"); printf_count++; }
.|\n        { fprintf(yyout, "%s", yytext); }
%%

int yywrap() {
  return 1;
}

int main(int argc, char **argv) {
  if (argc > 1) {
    FILE *file = fopen(argv[1], "r");
    if (!file) {
      fprintf(stderr, "Could not open %s\n", argv[1]);
      return 1;
    }
    yyin = file;
  } else {
    puts("Enter C code (Ctrl+D to end):");
  }

  yylex();

  fprintf(stderr, "\n--- Statistics ---\n");
  fprintf(stderr, "scanf replaced: %d\n", scanf_count);
  fprintf(stderr, "printf replaced: %d\n", printf_count);
  return 0;
}
