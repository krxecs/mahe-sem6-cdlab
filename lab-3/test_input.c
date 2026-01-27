#include <stdio.h>
#define MAX 100

// This function adds two numbers
int main() {
  int a = 10;
  int b = 20;

  /* Multi-line comment:
     Calculating sum
     Preprocessor directives above should be skipped.
  */

  char msg[] = "Sum is: char \"";
  msg[strlen(msg) - 1] = '\'';
  int sum = a + b;

  if (sum >= MAX) {
    return 1;
  }

  return 0;
}
