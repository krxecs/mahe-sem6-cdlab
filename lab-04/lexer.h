#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>

// Token Types
typedef enum {
  TOKEN_EOF,
  TOKEN_KEYWORD,
  TOKEN_IDENTIFIER,
  TOKEN_NUMBER,
  TOKEN_STRING,
  TOKEN_CHAR,
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

// Function Prototypes
Token getNextToken(FILE *fp, Position *pos);
const char *get_type_name(TokenType type);
void unget_char(int c, FILE *fp, Position *pos); // Exposed for parser pushback

#endif // LEXER_H
