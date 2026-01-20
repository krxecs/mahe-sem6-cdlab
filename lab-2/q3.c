#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const char *keywords[] = {
  "auto", "break", "case", "char", "const", "continue", "default", "do",
  "double", "else", "enum", "extern", "float", "for", "goto", "if",
  "inline", "int", "long", "register", "restrict", "return", "short",
  "signed", "sizeof", "static", "struct", "switch", "typedef", "union",
  "unsigned", "void", "volatile", "while", NULL
};

int is_keyword(const char *word) {
  for (int i = 0; keywords[i]; i++)
    if (strcmp(word, keywords[i]) == 0)
      return 1;

  return 0;
}

void print_upper(const char *word) {
  for (int i = 0; word[i]; ++i)
    putchar(toupper((unsigned char)word[i]));
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <input_file>\n", argv[0]);
    return 1;
  }

  FILE *fin = fopen(argv[1], "r");
  if (!fin) {
    perror("Error opening input file");
    return 1;
  }

  int c;

  bool in_dq = false;
  while ((c = fgetc(fin)) != EOF) {
    if (c == '\'') {
      putchar(c);
      c = fgetc(fin);
      if (c == EOF) continue;
      else if (c == '\'') {
        putchar(c);
        c = fgetc(fin);
        if (c == EOF) continue;
      }
      putchar(c);
      if (c == EOF) continue;
      c = fgetc(fin);
    }
    if (c == '"') {
      in_dq = !in_dq;
    } else if (in_dq) {
      // skip
    } else if (isalpha(c) || c == '_') {
      int idx = 0;
      char buffer[256];
      buffer[idx++] = c;

      while ((c = fgetc(fin)) != EOF && (isalnum(c) || c == '_')) {
        if (idx < 255)
          buffer[idx++] = c;
      }
      buffer[idx] = '\0';

      if (is_keyword(buffer))
        print_upper(buffer);
      else
        fputs(buffer, stdout);

      if (c == EOF) break;
    }
    putchar(c);
  }

  fclose(fin);
  return 0;
}
