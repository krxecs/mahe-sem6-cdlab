#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Grammar 4 (Left Recursion Removed):
S -> ( L ) | a
L -> S L'
L' -> , S L' | epsilon
*/

char input[100];
int i = 0;

void S(void);
void L(void);
void Lprime(void);

void error(void) {
  puts("Error");
  exit(0);
}

void match(char c) {
  if (input[i] == c) {
    i++;
  } else {
    error();
  }
}

void S(void) {
  // S -> ( L ) | a
  if (input[i] == '(') {
    match('(');
    L();
    match(')');
  } else if (input[i] == 'a') {
    match('a');
  } else {
    error();
  }
}

void L(void) {
  // L -> S L'
  S();
  Lprime();
}

void Lprime(void) {
  // L' -> , S L' | epsilon
  if (input[i] == ',') {
    match(',');
    S();
    Lprime();
  }
  // epsilon
}

int main(void) {
  puts("Grammar: S -> (L) | a; L -> L,S | S");
  puts("Enter string:");

  if (scanf("%s", input)) {
    S();
    if (input[i] == '\0') {
      puts("Success");
    } else {
      puts("Error: Extra characters at end");
    }
  }
  return 0;
}
