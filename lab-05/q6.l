%{
#include <stdio.h>
#include <ctype.h>
%}

/* Define a state for multi-line comments */
%x COMMENT

%%
"//".* { ECHO; /* Match single line comments and print as-is */ }

"/*"            { ECHO; BEGIN(COMMENT); /* Enter comment state */ }
<COMMENT>"*/"   { ECHO; BEGIN(INITIAL); /* Exit comment state */ }
<COMMENT>.|\n   { ECHO; /* Print everything inside comments as-is */ }

[A-Z]           { putchar(tolower(yytext[0])); /* Convert upper to lower */ }
.|\n            { ECHO; /* Print everything else as-is */ }
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
  return 0;
}
