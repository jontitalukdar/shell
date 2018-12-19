#include "argparser.h"
#include "env.h"
#include "function.h"
#include "process.h"
#include "redirect.h"
#include "variables.h"

int main() {
  //Store user input in character array
  char input_line[MAX_CHARSIZE];

  //Fetch the environment variables in a string and store them in a vector of strings
  std::vector<std::string> paths;
  path_parser(paths);

  //Initialize array for environment variables
  char ** env_array = init_arg_array();
  int env_count = 0;
  env_count = set_env(env_array, env_count);

  //Declare unordered map for storing environment variables and define change directory string
  std::unordered_map<std::string, std::string> mymap;
  std::string cd = "cd";

  //Execute shell as a while loop unless user exits program
  while (1) {
    char buf[MAX_CHARSIZE] = "";
    std::cout << "myShell:" << getcwd(buf, MAX_CHARSIZE) << " $ ";
    fgets(input_line, MAX_CHARSIZE, stdin);
    input_line[strlen(input_line) - 1] = '\0';

    //Initialize strings for Input/Output redirection by parsing and identifying'<' or '>'
    char * in_direct = init_redir_array();
    char * out_direct = init_redir_array();
    redir_tokenizer(in_direct, out_direct, input_line);

    //Initialize command line argument array
    char ** array_of_arguments = init_arg_array();

    //Parse input argument and store each element as part of the argument array
    int arg_count = 0;
    arg_count = arg_parser(array_of_arguments, input_line);
    char arg_1[MAX_CHARSIZE] = "";
    if (arg_count > 1) {
      strcpy(arg_1, array_of_arguments[1]);
    }

    //Replace user declared variables in command line with their value
    value_parser(array_of_arguments, arg_count, mymap);

    //Check User command input Exit Status, Enter key or EOF or change directory & free allocd memory
    if (shell_exit(input_line)) {  //When user types exit or EOF
      free_memory(array_of_arguments, in_direct, out_direct, arg_count);
      break;
    }
    if (array_of_arguments[0] == NULL) {  //When user presses only enter
      free_memory(array_of_arguments, in_direct, out_direct, arg_count);
      continue;
    }
    if (array_of_arguments[0] == cd) {  //When change directory invoked
      change_dir(array_of_arguments, arg_count);
      free_memory(array_of_arguments, in_direct, out_direct, arg_count);
      continue;
    }

    //Check user input for variable based commands
    int var_check = variable_check(array_of_arguments);
    if (var_check > 0) {
      //if 1 : call set_var : User invoked set operation
      if (var_check == 1) {
        set_var(array_of_arguments, input_line, mymap, arg_count);
        free_memory(array_of_arguments, in_direct, out_direct, arg_count);
        continue;
      }
      //if 2 : call export_var : User invoked export operation
      if (var_check == 2) {
        env_count = export_var(env_array, env_count, arg_1, arg_count, mymap);
        free_memory(array_of_arguments, in_direct, out_direct, arg_count);
        continue;
      }
      //if 3 : call inc_var : User invoked inc operation
      if (var_check == 3) {
        inc_var(arg_1, arg_count, mymap);
        free_memory(array_of_arguments, in_direct, out_direct, arg_count);
        continue;
      }
    }

    //Once all parsing and settings done, run user specified command
    invoke_process(array_of_arguments, env_array, in_direct, out_direct, paths);

    //Free allocd memory once commands executed as prescribed
    free_memory(array_of_arguments, in_direct, out_direct, arg_count);
  }
  //Delete environment array
  free_environment(env_array, env_count);
  return 0;
}
