#include <ctype.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Usage: %s <input_file> <output_file>\n", argv[0]);
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

  int c;
  int space_seen = 0;

  while ((c = fgetc(fin)) != EOF) {
    if (isblank(c)) {
      if (!space_seen) {
        fputc(' ', fout);
        space_seen = 1;
      }
    } else {
      fputc(c, fout);
      space_seen = 0;
    }
  }

  fclose(fin);
  fclose(fout);

  return 0;
}
