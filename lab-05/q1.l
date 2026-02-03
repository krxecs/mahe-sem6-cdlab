%{
#include <stdio.h>

int vowels = 0;
int consonants = 0;
%}

%%
[aeiouAEIOU]    { vowels++; }
[a-zA-Z]        { consonants++; }
[^a-zA-Z]       { /* Ignore other characters */ }
%%

int yywrap() {
  return 1;
}

int main() {
  puts("Enter string (Ctrl+D to end):");
  yylex();
  printf("Number of Vowels: %d\n", vowels);
  printf("Number of Consonants: %d\n", consonants);
  return 0;
}
