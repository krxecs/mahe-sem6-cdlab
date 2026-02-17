#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Grammar 2:
S -> U V W
U -> ( S ) | a S b | d
V -> a V | epsilon
W -> c W | epsilon
*/

char input[100];
int i = 0;

void S(void);
void U(void);
void V(void);
void W(void);

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
  // S -> U V W
  U();
  V();
  W();
}

void U(void) {
  if (input[i] == '(') {
    match('(');
    S();
    match(')');
  } else if (input[i] == 'a') {
    match('a');
    S();
    match('b');
  } else if (input[i] == 'd') {
    match('d');
  } else {
    error();
  }
}

void V(void) {
  // V -> a V | epsilon
  if (input[i] == 'a') {
    match('a');
    V();
  }
  // epsilon: do nothing
}

void W(void) {
  // W -> c W | epsilon
  if (input[i] == 'c') {
    match('c');
    W();
  }
  // epsilon: do nothing
}

int main(void) {
  puts("Grammar: S->UVW; U->(S)|aSb|d; V->aV|e; W->cW|e");
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
