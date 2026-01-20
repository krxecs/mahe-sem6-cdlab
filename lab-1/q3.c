#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE 2048

int main(int argc, char *argv[]) {
  if (argc != 4) {
    printf("Usage: %s <file1> <file2> <dest>", argv[0]);
    return 1;
  }

  FILE *f1 = fopen(argv[1], "r");
  if (!f1) {
    perror("Error opening file 1");
    return 1;
  }

  FILE *f2 = fopen(argv[2], "r");
  if (!f2) {
    perror("Error opening file 2");
    fclose(f1);
    return 1;
  }

  FILE *dest = fopen(argv[3], "w");
  if (!dest) {
    perror("Error opening destination file");
    fclose(f1);
    fclose(f2);
    return 1;
  }

  char buffer[MAX_LINE];
  int has_more_data = 1;

  while (has_more_data) {
    has_more_data = 0;

    if (fgets(buffer, sizeof(buffer), f1)) {
      fputs(buffer, dest);
      has_more_data = 1;
    }

    if (fgets(buffer, sizeof(buffer), f2)) {
      fputs(buffer, dest);
      has_more_data = 1;
    }
  }

  fclose(f1);
  fclose(f2);
  fclose(dest);

  puts("Files merged successfully.");

  return 0;
}

