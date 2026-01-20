#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

static const char *KEYWORDS[] = {
  "auto", "break", "case", "char", "const", "continue", "default", "do",
  "double", "else", "enum", "extern", "float", "for", "goto", "if",
  "int", "long", "register", "return", "short", "signed", "sizeof", "static",
  "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
};
static const size_t NUM_KEYWORDS = sizeof KEYWORDS / sizeof KEYWORDS[0];

bool is_keyword(const char *str) {
  for (size_t i = 0; i < NUM_KEYWORDS; i++) {
    if (strcmp(KEYWORDS[i], str) == 0)
      return true;
  }
  return false;
}

void identify_operator(const char *token) {
  if (strcmp(token, "+") == 0 || strcmp(token, "-") == 0 ||
    strcmp(token, "*") == 0 || strcmp(token, "/") == 0 ||
    strcmp(token, "%") == 0) {
    printf("Arithmetic Operator: %s\n", token);
    return;
  }

  if (strcmp(token, "<") == 0 || strcmp(token, ">") == 0 ||
    strcmp(token, "<=") == 0 || strcmp(token, ">=") == 0 ||
    strcmp(token, "==") == 0 || strcmp(token, "!=") == 0) {
    printf("Relational Operator: %s\n", token);
    return;
  }

  if (strcmp(token, "&&") == 0 || strcmp(token, "||") == 0 ||
    strcmp(token, "!") == 0) {
    printf("Logical Operator: %s\n", token);
    return;
  }

  if (strcmp(token, "=") == 0) {
    puts("Assignment Operator: =");
    return;
  }
}

void analyze_file(FILE *fp) {
  char ch;
  while ((ch = fgetc(fp)) != EOF) {

    if (isspace(ch)) continue;

    if (ch == '/') {
      char next = fgetc(fp);
      if (next == '/') {
        while ((ch = fgetc(fp)) != EOF && ch != '\n');
        continue;
      } else if (next == '*') {
        while ((ch = fgetc(fp)) != EOF) {
          if (ch == '*') {
            char end_check = fgetc(fp);
            if (end_check == '/') break;
            ungetc(end_check, fp);
          }
        }
        continue;
      } else {
        ungetc(next, fp);
        identify_operator("/");
        continue;
      }
    }

    if (ch == '"') {
      char buffer[256];
      int i = 0;
      buffer[i++] = ch;
      while ((ch = fgetc(fp)) != EOF && ch != '"') {
        buffer[i++] = ch;
      }
      buffer[i++] = '"';
      buffer[i] = '\0';
      printf("String Literal: %s\n", buffer);
      continue;
    }

    if (isalpha(ch) || ch == '_') {
      char buffer[256];
      int i = 0;
      buffer[i++] = ch;
      while ((ch = fgetc(fp)) != EOF && (isalnum(ch) || ch == '_')) {
        buffer[i++] = ch;
      }
      buffer[i] = '\0';
      ungetc(ch, fp);

      if (is_keyword(buffer)) {
        printf("Keyword: %s\n", buffer);
      } else {
        printf("Identifier: %s\n", buffer);
      }
      continue;
    }

    if (isdigit(ch)) {
      char buffer[256];
      int i = 0;
      buffer[i++] = ch;
      while ((ch = fgetc(fp)) != EOF && isdigit(ch)) {
        buffer[i++] = ch;
      }
      buffer[i] = '\0';
      ungetc(ch, fp);
      printf("Numerical Constant: %s\n", buffer);
      continue;
    }

    if (strchr("+-*%=!<>|&", ch)) {
      char buffer[4] = {ch, '\0', '\0', '\0'};
      char next = fgetc(fp);

      int combined = 0;
      if (strchr("=+-&|", next)) {
        if ((ch == '+' && next == '+') || (ch == '-' && next == '-') ||
          (ch == '&' && next == '&') || (ch == '|' && next == '|') ||
          (ch == '=' && next == '=') || (ch == '!' && next == '=') ||
          (ch == '<' && next == '=') || (ch == '>' && next == '=')) {
            buffer[1] = next;
            combined = 1;
          }
      }

      if (!combined) {
        ungetc(next, fp);
      }

      identify_operator(buffer);
      continue;
    }

    if (strchr("(){}[],;", ch)) {
      printf("Special Symbol: %c\n", ch);
      continue;
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <input-file>\n", argv[0]);
    return 1;
  }

  const char *filename = argv[1];
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    puts("Error: Could not open file.");
    return 1;
  }

  printf("Analyzing '%s'...\n", argv[1]);
  analyze_file(fp);

  fclose(fp);
  return 0;
}
