/**
 * Recursive Descent Parser for the following grammar:
 *
 *   Program        → main ( ) { declarations assign_stat }
 *   declarations   → data-type identifier-list ; declarations | ε
 *   data-type      → int | char
 *   identifier-list→ id | id , identifier-list
 *   assign_stat    → id = id ; | id = num ;
 *
 * Uses lexer.c / lexer.h for tokenisation.
 * Reports errors with the expected token and its row/column location.
 */

#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ── global parser state ─────────────────────────────────────────── */
static FILE *src;
static Position pos;
static Token cur; /* lookahead token */

/* ── helpers ─────────────────────────────────────────────────────── */

static void advance(void) { cur = getNextToken(src, &pos); }

/* Fatal error: print location of the bad token then exit. */
static void parse_error(const char *expected) {
  fprintf(stderr, "Parse error at row %d, col %d: expected %s but got '%s'\n",
          cur.row, cur.col, expected, cur.lexeme);
  exit(EXIT_FAILURE);
}

static int is_symbol(const char *s) {
  return cur.type == TOKEN_SYMBOL && strcmp(cur.lexeme, s) == 0;
}
static int is_keyword(const char *kw) {
  return cur.type == TOKEN_KEYWORD && strcmp(cur.lexeme, kw) == 0;
}
static int is_operator(const char *op) {
  return cur.type == TOKEN_OPERATOR && strcmp(cur.lexeme, op) == 0;
}

/* Consume current token if it matches the expected symbol, else error. */
static void expect_symbol(const char *s) {
  if (!is_symbol(s)) {
    char msg[64];
    snprintf(msg, sizeof msg, "'%s'", s);
    parse_error(msg);
  }
  advance();
}

/* Consume current token if it is the expected keyword, else error. */
static void expect_keyword(const char *kw) {
  if (!is_keyword(kw)) {
    char msg[64];
    snprintf(msg, sizeof msg, "keyword '%s'", kw);
    parse_error(msg);
  }
  advance();
}

/* Consume current token if it is an identifier, else error. */
static void expect_identifier(void) {
  if (cur.type != TOKEN_IDENTIFIER)
    parse_error("identifier");
  advance();
}

/* Consume current token if it is the expected operator, else error. */
static void expect_operator(const char *op) {
  if (!is_operator(op)) {
    char msg[64];
    snprintf(msg, sizeof msg, "operator '%s'", op);
    parse_error(msg);
  }
  advance();
}

/*
 * data-type → int | char
 */
static void parse_data_type(void) {
  if (is_keyword("int") || is_keyword("char")) {
    advance();
  } else {
    parse_error("data type 'int' or 'char'");
  }
}

/*
 * identifier-list → id | id , identifier-list
 */
static void parse_identifier_list(void) {
  expect_identifier();
  if (is_symbol(",")) {
    advance();               /* consume ',' */
    parse_identifier_list(); /* recurse */
  }
}

/*
 * declarations → data-type identifier-list ; declarations | ε
 *
 * data_type = { int, char }
 */
static void parse_declarations(void) {
  if (is_keyword("int") || is_keyword("char")) {
    parse_data_type();
    parse_identifier_list();
    expect_symbol(";");
    parse_declarations(); /* tail recursion → next declaration */
  }
  /* ε : nothing to do */
}

/*
 * assign_stat → id = id ; | id = num ;
 */
static void parse_assign_stat(void) {
  while (!is_symbol("}")) {
    if (cur.type == TOKEN_EOF)
      parse_error("'}' or assignment statement");

    /* lhs must be an identifier */
    if (cur.type != TOKEN_IDENTIFIER)
      parse_error("identifier (lhs of assignment)");
    advance();

    expect_operator("=");

    /* rhs: id or num */
    if (cur.type == TOKEN_IDENTIFIER || cur.type == TOKEN_NUMBER) {
      advance();
    } else {
      parse_error("identifier or number (rhs of assignment)");
    }

    expect_symbol(";");
  }
}

/*
 * Program → main ( ) { declarations assign_stat }
 */
static void parse_program(void) {
  /* 'main' is lexed as an identifier (not a keyword) */
  if (cur.type != TOKEN_IDENTIFIER || strcmp(cur.lexeme, "main") != 0)
    parse_error("'main'");
  advance();

  expect_symbol("(");
  expect_symbol(")");
  expect_symbol("{");

  parse_declarations();
  parse_assign_stat();

  expect_symbol("}");

  if (cur.type != TOKEN_EOF)
    parse_error("end of input");

  printf("Parsing successful!\n");
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <source-file>\n", argv[0]);
    return 1;
  }

  src = fopen(argv[1], "r");
  if (!src) {
    perror(argv[1]);
    return 1;
  }

  pos.row = 1;
  pos.col = 0;

  advance(); /* prime the lookahead */
  parse_program();

  fclose(src);
  return 0;
}
