#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Grammar 1 (Left Recursion Removed):
S -> a | > | ( T )
T -> S T'
T' -> , S T' | epsilon
*/

char input[100];
int i = 0;

void S(void);
void T(void);
void Tprime(void);

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
  if (input[i] == 'a') {
    match('a');
  } else if (input[i] == '>') {
    match('>');
  } else if (input[i] == '(') {
    match('(');
    T();
    match(')');
  } else {
    error();
  }
}

void T(void) {
  // T -> S T'
  S();
  Tprime();
}

void Tprime(void) {
  // T' -> , S T' | epsilon
  if (input[i] == ',') {
    match(',');
    S();
    Tprime();
  }
  // epsilon: do nothing
}

int main(void) {
  puts("Grammar: S -> a | > | (T) ; T -> T,S | S");
  puts("Enter string:");

  // Using scanf for simple whitespace-delimited input as per typical lab inputs
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
