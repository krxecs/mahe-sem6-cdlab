#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 4096

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Usage: %s <input_file> <output_file>", argv[0]);
    return 1;
  }

  FILE *fin = fopen(argv[1], "r");
  if (!fin) {
    perror("Error opening input file");
    return 1;
  }

  FILE *fout = fopen(argv[2], "w");
  if (!fout) {
    perror("Error opening output file");
    fclose(fin);
    return 1;
  }

  char buffer[MAX_LINE];
  int inside_directive = 0;

  while (fgets(buffer, sizeof(buffer), fin)) {
    size_t len = strlen(buffer);

    int has_continuation = 0;
    if (len >= 2 && buffer[len - 1] == '\n' && buffer[len - 2] == '\\') {
      has_continuation = 1;
    } else if (len >= 1 && buffer[len - 1] == '\\') {
      has_continuation = 1;
    }

    if (inside_directive) {
      if (!has_continuation)
        inside_directive = 0;

      continue;
    }

    char *p = buffer;
    while (*p && isblank((unsigned char)*p))
      p++;

    if (*p == '#') {
      if (has_continuation)
        inside_directive = 1;

      continue;
    }

    fputs(buffer, fout);
  }

  fclose(fin);
  fclose(fout);

  return 0;
}
