#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Usage: %s <source> <destination>\n", argv[0]);
    return 1;
  }

  FILE *src = fopen(argv[1], "rb");
  if (!src) {
    perror("Error opening source file");
    return 1;
  }

  FILE *dest = fopen(argv[2], "wb");
  if (!dest) {
    perror("Error opening destination file");
    fclose(src);
    return 1;
  }

  if (fseek(src, 0, SEEK_END) != 0) {
    perror("Seek error");
    fclose(src);
    fclose(dest);
    return 1;
  }

  long file_size = ftell(src);
  printf("File size: %ld bytes\n", file_size);

  long pos = file_size - 1;
  while (pos >= 0) {
    fseek(src, pos, SEEK_SET);

    int ch = fgetc(src);
    if (ch != EOF)
      fputc(ch, dest);

    pos--;
  }

  fclose(src);
  fclose(dest);

  puts("File contents reversed and stored successfully.");

  return 0;
}
