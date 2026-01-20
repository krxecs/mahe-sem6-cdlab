#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <filename>\n", argv[0]);
    return 1;
  }

  FILE *fp = fopen(argv[1], "r");
  if (!fp) {
    perror("Error opening file");
    return 1;
  }

  long char_count = 0, line_count = 0;
  int ch;

  while ((ch = fgetc(fp)) != EOF) {
    char_count++;
    if (ch == '\n')
      line_count++;
  }

  fclose(fp);

  printf("Lines: %ld\n", line_count);
  printf("Characters: %ld\n", char_count);

  return 0;
}

