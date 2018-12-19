#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_ARGSIZE 128
#define MAX_CHARSIZE 1024
#include <errno.h>

#include <iostream>
#include <string>
#include <vector>

//init_arg_array function: Initialize argument array
char ** init_arg_array() {
  char ** array_of_arguments = new char *[MAX_ARGSIZE];
  for (int i = 0; i < MAX_ARGSIZE; i++) {
    array_of_arguments[i] = NULL;
  }
  return array_of_arguments;
}
//init_redir_array function: Initialize character array (used for IO Redirection, environment vars etc.)
char * init_redir_array() {
  char * redir_string = new char[MAX_CHARSIZE];
  for (int i = 0; i < MAX_CHARSIZE; i++) {
    redir_string[i] = '\0';
  }
  return redir_string;
}

//shell_exit function: Check for shell exit condition or EOF
int shell_exit(char * input_line) {
  std::string exit_sequence = "exit";
  if (input_line == exit_sequence) {  //exit
    //Print prompt while exiting
    std::cout << "exiting...\n";
    return 1;
  }
  if (feof(stdin)) {  //EOF
    //Print prompt while exiting
    std::cout << "exiting...\n";
    return 1;
  }
  return 0;
}

//free_memory function: Remove allocd memory in overall shell including char * arrays, char ** arrays
void free_memory(char ** arguments, char * in_direct, char * out_direct, int arg_count) {
  for (int i = 0; i < arg_count; i++) {
    delete[] arguments[i];
  }
  delete[] in_direct;
  delete[] out_direct;
  delete[] arguments;
}

#endif
