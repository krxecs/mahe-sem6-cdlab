#include "lexer.h"
#include "uthash.h" // Ensure uthash.h is in the same directory
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- SYMBOL TABLE DEFINITIONS ---

// Symbol Structure
typedef struct {
  char name[100];    // Key
  char type[50];     // Data type (e.g., "int", "char", "func")
  int size;          // Size in bytes
  char scope[100];   // "global" or function name
  UT_hash_handle hh; // Handle for uthash
} Symbol;

// Structure to manage Local Symbol Tables (One table per function)
typedef struct {
  char func_name[100]; // Key: Function Name
  Symbol *symbols;     // Head of the local symbol hash table
  UT_hash_handle hh;   // Handle for uthash
} LocalTableRegistry;

// Context to hold state instead of global variables
typedef struct {
  Symbol *global_table;
  LocalTableRegistry *local_registries;
} SymbolContext;

// --- HELPERS ---

// Helper to determine base size of a type
int get_type_size(const char *type) {
  if (strcmp(type, "int") == 0)
    return 4;
  if (strcmp(type, "char") == 0)
    return 1;
  if (strcmp(type, "float") == 0)
    return 4;
  if (strcmp(type, "double") == 0)
    return 8;
  if (strcmp(type, "short") == 0)
    return 2;
  if (strcmp(type, "long") == 0)
    return 8;
  if (strcmp(type, "bool") == 0)
    return 1;
  if (strcmp(type, "void") == 0)
    return 0;
  return 4; // Default/Pointers
}

// Add symbol to Global Table
void add_global_symbol(SymbolContext *ctx, const char *name, const char *type,
                       int size) {
  Symbol *s;
  HASH_FIND_STR(ctx->global_table, name, s);
  if (!s) {
    s = malloc(sizeof *s);
    strcpy(s->name, name);
    strcpy(s->type, type);
    s->size = size;
    strcpy(s->scope, "Global");
    HASH_ADD_STR(ctx->global_table, name, s);
  }
}

// Add symbol to Local Table (creates table if needed)
void add_local_symbol(SymbolContext *ctx, const char *func_name,
                      const char *name, const char *type, int size) {
  LocalTableRegistry *reg;
  HASH_FIND_STR(ctx->local_registries, func_name, reg);

  // If local table for this function doesn't exist, create it
  if (!reg) {
    reg = malloc(sizeof *reg);
    strcpy(reg->func_name, func_name);
    reg->symbols = NULL;
    HASH_ADD_STR(ctx->local_registries, func_name, reg);
  }

  Symbol *s;
  HASH_FIND_STR(reg->symbols, name, s);
  if (!s) {
    s = malloc(sizeof *s);
    strcpy(s->name, name);
    strcpy(s->type, type);
    s->size = size;
    strcpy(s->scope, func_name);
    HASH_ADD_STR(reg->symbols, name, s);
  }
}

int is_data_type(const char *str) {
  static const char *types[] = {"int",  "char", "float", "double",
                                "void", "long", "short", "bool"};
  for (int i = 0; i < 8; i++) {
    if (strcmp(types[i], str) == 0)
      return 1;
  }
  return 0;
}

void print_tables(SymbolContext *ctx) {
  Symbol *s, *tmp;
  LocalTableRegistry *reg, *tmp_reg;

  puts("\n--- GLOBAL SYMBOL TABLE ---");
  printf("%-20s %-10s %-10s %-10s\n", "Name", "Type", "Size", "Scope");
  puts("-------------------------------------------------------");
  HASH_ITER(hh, ctx->global_table, s, tmp) {
    printf("%-20s %-10s %-10d %-10s\n", s->name, s->type, s->size, s->scope);
  }

  puts("\n--- LOCAL SYMBOL TABLES ---");
  HASH_ITER(hh, ctx->local_registries, reg, tmp_reg) {
    printf("\nFunction: %s\n", reg->func_name);
    printf("%-20s %-10s %-10s %-10s\n", "Name", "Type", "Size", "Scope");
    puts("-------------------------------------------------------");
    Symbol *ls, *tmp_ls;
    HASH_ITER(hh, reg->symbols, ls, tmp_ls) {
      printf("%-20s %-10s %-10d %-10s\n", ls->name, ls->type, ls->size,
             ls->scope);
    }
  }
}

// --- MAIN PARSER ---

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

  SymbolContext ctx = {NULL, NULL};

  Position pos = {1, 1};
  Token t;
  char current_scope[100] = "Global";
  int brace_level = 0;

  // Parsing Loop
  while ((t = getNextToken(fp, &pos)).type != TOKEN_EOF) {

    // Check for variable or function declaration
    if (t.type == TOKEN_KEYWORD && is_data_type(t.lexeme)) {
      char type_name[50];
      strcpy(type_name, t.lexeme);

      // Process list of identifiers (e.g., int a, b, c;)
      while (1) {
        Token next = getNextToken(fp, &pos);
        int is_ptr = 0;

        // Handle pointers (int *ptr)
        if (next.type == TOKEN_OPERATOR && strcmp(next.lexeme, "*") == 0) {
          is_ptr = 1;
          next = getNextToken(fp, &pos);
        }

        if (next.type == TOKEN_IDENTIFIER) {
          char id_name[100];
          strcpy(id_name, next.lexeme);

          Token sep = getNextToken(fp, &pos);

          // Case 1: Function Definition/Declaration 'int func('
          if (strcmp(sep.lexeme, "(") == 0) {
            // It's a function
            if (brace_level == 0) {
              add_global_symbol(&ctx, id_name, "func",
                                -1); // Size -1 or 0 for func
              strcpy(current_scope, id_name);

              // Parse Parameters
              while (1) {
                Token pT = getNextToken(fp, &pos);
                if (strcmp(pT.lexeme, ")") == 0)
                  break;
                if (is_data_type(pT.lexeme)) {
                  char pType[50];
                  strcpy(pType, pT.lexeme);
                  Token pName = getNextToken(fp, &pos);
                  int pPtr = 0;
                  if (strcmp(pName.lexeme, "*") == 0) {
                    pPtr = 1;
                    pName = getNextToken(fp, &pos);
                  }
                  if (pName.type == TOKEN_IDENTIFIER) {
                    int sz = pPtr ? 4 : get_type_size(pType);
                    add_local_symbol(&ctx, current_scope, pName.lexeme, pType,
                                     sz);
                  }
                  // Handle array param int a[]
                  Token afterParam = getNextToken(fp, &pos);
                  if (strcmp(afterParam.lexeme, "[") == 0) {
                    while (strcmp(afterParam.lexeme, "]") != 0 &&
                           strcmp(afterParam.lexeme, ",") != 0 &&
                           strcmp(afterParam.lexeme, ")") != 0) {
                      afterParam = getNextToken(fp, &pos);
                    }
                    if (strcmp(afterParam.lexeme, "]") == 0)
                      afterParam = getNextToken(fp, &pos);
                  }
                  if (strcmp(afterParam.lexeme, ")") == 0)
                    break;
                }
              }
            }
            // Loop continues to check if next is { or ;
            // If it is {, the next iteration of while(1) will hit the else
            // block below
          }
          // Case 2: Array 'int arr['
          else if (strcmp(sep.lexeme, "[") == 0) {
            Token sizeTok = getNextToken(fp, &pos);
            int arr_len = 0;
            if (sizeTok.type == TOKEN_NUMBER) {
              arr_len = atoi(sizeTok.lexeme);
              getNextToken(fp, &pos); // consume ']'
            } else if (strcmp(sizeTok.lexeme, "]") == 0) {
              arr_len = 1; // Unspecified or pointer decay
            }

            int total_size = arr_len * get_type_size(type_name);
            sep = getNextToken(fp, &pos); // Get delimiter after ]

            if (brace_level == 0) {
              add_global_symbol(&ctx, id_name, type_name, total_size);
            } else {
              add_local_symbol(&ctx, current_scope, id_name, type_name,
                               total_size);
            }
          }
          // Case 3: Simple Variable
          else {
            int size = is_ptr ? 4 : get_type_size(type_name);
            if (brace_level == 0) {
              add_global_symbol(&ctx, id_name, type_name, size);
            } else {
              add_local_symbol(&ctx, current_scope, id_name, type_name, size);
            }
          }

          // Handle delimiter
          if (strcmp(sep.lexeme, ";") == 0)
            break; // End of decl
          if (strcmp(sep.lexeme, "=") == 0) {
            // Skip initialization until , or ;
            int paren_depth = 0;
            while (1) {
              sep = getNextToken(fp, &pos);
              if (strcmp(sep.lexeme, "(") == 0)
                paren_depth++;
              if (strcmp(sep.lexeme, ")") == 0)
                paren_depth--;
              if (paren_depth == 0 && (strcmp(sep.lexeme, ",") == 0 ||
                                       strcmp(sep.lexeme, ";") == 0))
                break;
              if (sep.type == TOKEN_EOF)
                break;
            }
          }
          if (strcmp(sep.lexeme, ";") == 0)
            break;
        } else {
          // Not an identifier (e.g. '{' starting a function body)
          // Put it back so the main loop can handle it (like incrementing
          // brace_level)
          unget_char(next.lexeme[0], fp, &pos);
          break;
        }
      }
    }
    // Track Scope
    else if (strcmp(t.lexeme, "{") == 0) {
      brace_level++;
    } else if (strcmp(t.lexeme, "}") == 0) {
      brace_level--;
      if (brace_level == 0) {
        strcpy(current_scope, "Global");
      }
    }
  }

  print_tables(&ctx);
  fclose(fp);
  return 0;
}
