#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Grammar 3 (Left Recursion Removed):
S -> a A c B e
A -> b A'
A' -> b A' | epsilon
B -> d
*/

char input[100];
int i = 0;

void S(void);
void A(void);
void Aprime(void);
void B(void);

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
  // S -> a A c B e
  match('a');
  A();
  match('c');
  B();
  match('e');
}

void A(void) {
  // A -> b A'
  match('b');
  Aprime();
}

void Aprime(void) {
  // A' -> b A' | epsilon
  if (input[i] == 'b') {
    match('b');
    Aprime();
  }
  // epsilon
}

void B(void) {
  // B -> d
  match('d');
}

int main(void) {
  puts("Grammar: S -> aAcBe; A -> Ab|b; B -> d");
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
