#ifndef __ENV_H__
#define __ENV_H__
#include <string.h>

#include <iostream>
#include <vector>

#include "function.h"

/*set_env function: Sets the environment variables in PATH to local ECE551PATH
and initializes the environment variable array. */
int set_env(char ** env_array, int env_count) {
  std::string path_var = "ECE551PATH=";
  char const * path = "PATH";
  char * env_name = getenv(path);
  char * env_variable = init_redir_array();
  //Sets ECE551PATH to env_variables in PATH
  strcpy(env_variable, path_var.c_str());
  strcat(env_variable, env_name);
  env_array[env_count] = env_variable;
  env_count++;

  return env_count;
}

/*path_parser function: Parses environment variables from PATH and stores them
in a vector templated to string type. */
std::vector<std::string> & path_parser(std::vector<std::string> & paths) {
  char const * path_var = "PATH";
  char * env_name = getenv(path_var);
  char * token_ptr;
  //The : is used as token delimiter
  token_ptr = strtok(env_name, ":");
  while (token_ptr != NULL) {
    paths.push_back(token_ptr);
    token_ptr = strtok(NULL, ":");
  }
  return paths;
}

/*change_dir function: Changes current working directory based on input path
given as arguments.*/
void change_dir(char ** arguments, int arg_count) {
  std::string cd = "cd";
  if (arg_count > 2) {
    fprintf(stderr, "cd has more than two arguments. Error.\n");
  }
  //Just typing 'cd' without arguments directs to my HOME directory as default
  else if (arg_count == 1) {
    chdir("/home/jt292/");
  }
  else {
    int result = chdir(arguments[1]);
    if (result < 0) {
      fprintf(stderr, "'%s' : No such file or directory.\n", arguments[1]);
    }
  }
}

/*free_environment function: Frees memory allocd for environment variable array.*/
void free_environment(char ** env_array, int env_count) {
  for (int i = 0; i < env_count; i++) {
    delete[] env_array[i];
  }
  delete[] env_array;
}

#endif
