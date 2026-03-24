#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Rules representation
struct Rule {
  int lhs;
  int len;
  const char *str;
} rules[] = {
  {0, 0, "E' -> E"},
  {0, 3, "E -> E + T"},
  {0, 1, "E -> T"},
  {1, 3, "T -> T * F"},
  {1, 1, "T -> F"},
  {2, 3, "F -> ( E )"},
  {2, 1, "F -> id"}
};

// Action table (12 states x 6 terminals)
// Terminals: 0:id(i), 1:+, 2:*, 3:(, 4:), 5:$
// type: 0=Error, 1=Shift, 2=Reduce, 3=Accept
struct Action {
  int type;
  int val;
} action[12][6] = {
  /* 0  */ {{1,5}, {0,0}, {0,0}, {1,4}, {0,0}, {0,0}},
  /* 1  */ {{0,0}, {1,6}, {0,0}, {0,0}, {0,0}, {3,0}},
  /* 2  */ {{0,0}, {2,2}, {1,7}, {0,0}, {2,2}, {2,2}},
  /* 3  */ {{0,0}, {2,4}, {2,4}, {0,0}, {2,4}, {2,4}},
  /* 4  */ {{1,5}, {0,0}, {0,0}, {1,4}, {0,0}, {0,0}},
  /* 5  */ {{0,0}, {2,6}, {2,6}, {0,0}, {2,6}, {2,6}},
  /* 6  */ {{1,5}, {0,0}, {0,0}, {1,4}, {0,0}, {0,0}},
  /* 7  */ {{1,5}, {0,0}, {0,0}, {1,4}, {0,0}, {0,0}},
  /* 8  */ {{0,0}, {1,6}, {0,0}, {0,0}, {1,11},{0,0}},
  /* 9  */ {{0,0}, {2,1}, {1,7}, {0,0}, {2,1}, {2,1}},
  /* 10 */ {{0,0}, {2,3}, {2,3}, {0,0}, {2,3}, {2,3}},
  /* 11 */ {{0,0}, {2,5}, {2,5}, {0,0}, {2,5}, {2,5}}
};

// Goto table (12 states x 3 non-terminals: E=0, T=1, F=2)
int gototbl[12][3] = {
  /* 0  */ {1, 2, 3},
  /* 1  */ {-1, -1, -1},
  /* 2  */ {-1, -1, -1},
  /* 3  */ {-1, -1, -1},
  /* 4  */ {8, 2, 3},
  /* 5  */ {-1, -1, -1},
  /* 6  */ {-1, 9, 3},
  /* 7  */ {-1, -1, 10},
  /* 8  */ {-1, -1, -1},
  /* 9  */ {-1, -1, -1},
  /* 10 */ {-1, -1, -1},
  /* 11 */ {-1, -1, -1}
};

int get_term_idx(char c) {
  if (c == 'i') return 0;
  if (c == '+') return 1;
  if (c == '*') return 2;
  if (c == '(') return 3;
  if (c == ')') return 4;
  if (c == '$') return 5;
  return -1;
}

int main(void) {
  int stack[100];
  int top = 0;
  stack[0] = 0;

  // 'i' represents 'id'
  char input[100];
  printf("Enter input string (end with $): ");
  scanf("%s", input);
  int ip = 0;

  puts("Stack\t\t\tInput\t\tAction");
  puts("------------------------------------------------------");

  while (1) {
    char stack_str[100] = "";
    for (int k = 0; k <= top; k++) {
      char temp[10];
      sprintf(temp, "%d ", stack[k]);
      strcat(stack_str, temp);
    }

    printf("%-24s%-16s", stack_str, input + ip);

    int state = stack[top];
    char sym = input[ip];
    int term_idx = get_term_idx(sym);

    if (term_idx == -1) {
      puts("Error: Invalid character");
      break;
    }

    struct Action act = action[state][term_idx];

    if (act.type == 1) { // Shift
      stack[++top] = act.val;
      ip++;
      printf("Shift %d\n", act.val);
    } else if (act.type == 2) { // Reduce
      struct Rule r = rules[act.val];
      top -= r.len;
      int next_state = stack[top];
      stack[++top] = gototbl[next_state][r.lhs];
      printf("Reduce %s\n", r.str);
    } else if (act.type == 3) { // Accept
      puts("Accept");
      break;
    } else { // Error
      puts("Error: Syntax error");
      break;
    }
  }
  return 0;
}
