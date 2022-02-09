#include <stdio.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    puts("ERROR: You need one argument.");
    return 1; // abort with a non-zero error code
  }

  int i = 0;
  int j = 0;
  int position = 0;
  for(i = 1; i < argc; i++) {
    for(j = 0; argv[i][j] != '\0'; j++, position++) {
      char letter = tolower(argv[i][j]);

      switch (letter) {
        case 'a':
          printf("%d: 'a'\n", position);
          break;
        case 'e':
          printf("%d: 'e'\n", position);
          break;
        case 'i':
          printf("%d: 'i'\n", position);
          break;
        case 'o':
          printf("%d: 'o'\n", position);
          break;
        case 'u':
          printf("%d: 'u'\n", position);
          break;
        case 'y':
          if (i > 2) {
            // only sometimes
            printf("%d: 'y'\n", position);
            break;
          }

        default:
          printf("%d: %c is not a vowel\n", position, letter);
      }
    }
  }

  return 0;
}