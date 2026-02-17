#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <uthash.h>

// Token Types
typedef enum {
  TOKEN_EOF,
  TOKEN_KEYWORD,
  TOKEN_FUNCTION,
  TOKEN_IDENTIFIER,
  TOKEN_NUMBER,
  TOKEN_STRING,
  TOKEN_OPERATOR,
  TOKEN_SYMBOL,
  TOKEN_UNKNOWN
} TokenType;

// Token Structure
typedef struct {
  int row;
  int col;
  TokenType type;
  char lexeme[256];
} Token;

// Position Structure to track cursor
typedef struct {
  int row;
  int col;
} Position;

// Keywords list
static const char *KEYWORDS[] = {"break", "for", "if"};
static const size_t NUM_KEYWORDS = sizeof KEYWORDS / sizeof KEYWORDS[0];

// Helper to check if a string is a keyword
int is_keyword(const char *str) {
  for (size_t i = 0; i < NUM_KEYWORDS; i++) {
    if (strcmp(KEYWORDS[i], str) == 0)
      return 1;
  }
  return 0;
}

const char *get_type_name(TokenType type) {
  switch (type) {
  case TOKEN_KEYWORD:
    return "Keyword";
  case TOKEN_FUNCTION:
    return "Function";
  case TOKEN_IDENTIFIER:
    return "Identifier";
  case TOKEN_NUMBER:
    return "Number";
  case TOKEN_STRING:
    return "String Literal";
  case TOKEN_OPERATOR:
    return "Operator";
  case TOKEN_SYMBOL:
    return "Special Symbol";
  case TOKEN_EOF:
    return "EOF";
  default:
    return "Unknown";
  }
}

// Function to get the next character and update column/row in the struct
int get_char(FILE *fp, Position *pos) {
  int c = fgetc(fp);
  if (c == '\n') {
    pos->row++;
    pos->col = 1;
  } else {
    pos->col++;
  }
  return c;
}

// Function to put back a character and adjust column/row in the struct
void unget_char(int c, FILE *fp, Position *pos) {
  if (c == '\n') {
    pos->row--;
    pos->col = 1;
  } else {
    pos->col--;
  }
  ungetc(c, fp);
}

// Helper function to skip until next line
void skip_until_next_line(FILE *fp, Position *pos) {
  int c;
  while ((c = get_char(fp, pos)) != EOF && c != '\n')
    ;
}

// The core function accepts Position struct pointer
Token getNextToken(FILE *fp, Position *pos) {
  Token token;
  token.lexeme[0] = '\0';
  token.type = TOKEN_UNKNOWN;

  int c;

  while ((c = get_char(fp, pos)) != EOF) {
    token.row = pos->row;
    token.col = pos->col - 1;

    if (isspace(c)) {
      continue;
    }

    if (c == '<') {
      if ((c = get_char(fp, pos)) == '?')
        skip_until_next_line(fp, pos);
      else
        unget_char(c, fp, pos);
    }

    if (c == '?') {
      if ((c = get_char(fp, pos)) == '>')
        skip_until_next_line(fp, pos);
      else
        unget_char(c, fp, pos);
    }

    if (c == '/') {
      int next = get_char(fp, pos);
      if (next == '*') {
        int prev = 0;
        while ((c = get_char(fp, pos)) != EOF) {
          if (prev == '*' && c == '/')
            break;
          prev = c;
        }
        continue;
      } else {
        unget_char(next, fp, pos);
        token.type = TOKEN_OPERATOR;
        strcpy(token.lexeme, "/");
        return token;
      }
    }

    if (c == '"' || c =='\'') {
      int i = 0;
      token.lexeme[i++] = c;
      while ((c = get_char(fp, pos)) != EOF) {
        if (c == '"' || c =='\'')
          break;
        if (c == '\\') {
          if (i < 254)
            token.lexeme[i++] = (char)c;
          c = get_char(fp, pos);
        }
        if (i < 254)
          token.lexeme[i++] = (char)c;
      }
      token.lexeme[i++] = '"';
      token.lexeme[i] = '\0';
      token.type = TOKEN_STRING;
      return token;
    }

    if (isalpha(c) || c == '_') {
      int i = 0;
      token.lexeme[i++] = (char)c;
      while ((c = get_char(fp, pos)) != EOF && (isalnum(c) || c == '_')) {
        if (i < 254)
          token.lexeme[i++] = (char)c;
      }
      token.lexeme[i] = '\0';
      unget_char(c, fp, pos);

      if (is_keyword(token.lexeme)) {
        token.type = TOKEN_KEYWORD;
      } else {
        token.type = TOKEN_FUNCTION;
      }
      return token;
    }

    if (c == '$') {
      int i = 0;
      token.lexeme[i++] = (char)c;
      while ((c = get_char(fp, pos)) != EOF && (isalnum(c) || c == '_')) {
        if (i < 254)
          token.lexeme[i++] = (char)c;
      }
      token.lexeme[i] = '\0';
      unget_char(c, fp, pos);
      token.type = TOKEN_IDENTIFIER;
      return token;
    }

    if (isdigit(c)) {
      int i = 0;
      token.lexeme[i++] = (char)c;
      while ((c = get_char(fp, pos)) != EOF && isdigit(c)) {
        if (i < 254)
          token.lexeme[i++] = (char)c;
      }
      token.lexeme[i] = '\0';
      unget_char(c, fp, pos);
      token.type = TOKEN_NUMBER;
      return token;
    }

    if (strchr("(){}[],;", c)) {
      token.lexeme[0] = (char)c;
      token.lexeme[1] = '\0';
      token.type = TOKEN_SYMBOL;
      return token;
    }

    if (strchr("+-*%=!<>|&", c)) {
      int i = 0;
      token.lexeme[i++] = (char)c;
      int next = get_char(fp, pos);
      int is_compound = 0;
      if (strchr("=+-&|", next)) {
        if ((c == '+' && next == '+') || (c == '-' && next == '-') ||
            (c == '&' && next == '&') || (c == '|' && next == '|') ||
            (c == '=' && next == '=') || (c == '!' && next == '=') ||
            (c == '<' && next == '=') || (c == '>' && next == '=')) {
          token.lexeme[i++] = (char)next;
          is_compound = 1;
        }
      }
      if (!is_compound)
        unget_char(next, fp, pos);
      token.lexeme[i] = '\0';
      token.type = TOKEN_OPERATOR;
      return token;
    }
  }

  token.type = TOKEN_EOF;
  strcpy(token.lexeme, "EOF");
  return token;
}

typedef struct {
  char name[100];
  char scope[100];   // "global" or function name
  UT_hash_handle hh;
} Symbol;

typedef struct {
  Symbol *global_table;
} SymbolContext;



int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <input-file>\n", argv[0]);
    return 1;
  }

  FILE *fp = fopen(argv[1], "r");
  if (!fp) {
    fprintf(stderr, "Error: Could not open %s\n", argv[1]);
    return 1;
  }

  printf("Analyzing '%s'...\n", argv[1]);
  printf("%-10s %-10s %-20s %s\n", "Row", "Col", "Type", "Lexeme");
  puts("---------------------------------------------------------");

  // Initialize Position struct
  Position pos = {1, 1};
  Token t;

  // Pass pointer to struct
  while ((t = getNextToken(fp, &pos)).type != TOKEN_EOF) {
    printf("%-10d %-10d %-20s %s\n", t.row, t.col, get_type_name(t.type),
           t.lexeme);
  }

  fclose(fp);
  return 0;
}
