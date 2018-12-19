/* execve.cpp */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char * argv[]) {
  char * newargv[] = {NULL};
  char * newenviron[] = {NULL};

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <file-to-exec>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  newargv[0] = argv[1];
  execve(argv[1], newargv, newenviron);
  perror("execve returned error.\n"); /* execve() returns only on error */
  exit(EXIT_FAILURE);
}
