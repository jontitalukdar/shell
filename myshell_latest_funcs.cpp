#include <unordered_map>

#include "function.h"

//Variable Parser
int variable_check(char ** arguments) {
  //commmand names
  std::string set = "set";
  std::string exp = "export";
  std::string inc = "inc";
  if (arguments[0] == set) {
    std::cout << "Setting variable: ";
    return 1;
  }
  if (arguments[0] == exp) {
    std::cout << "Exporting variable: ";
    return 2;
  }
  if (arguments[0] == inc) {
    std::cout << "Incrementing variable: ";
    return 3;
  }
  else {
    return 0;
  }
}

//Set Variable: Sets usr declared variable into umap.
void set_var(char ** arguments,
             char * inputline,
             std::unordered_map<std::string, std::string> & mymap,
             int arg_count) {
  if (arg_count < 3) {
    std::cout << "Usage: set <var_name> <var_value>\n";
    return;
  }

  //Check if arg[1] = var_name is valid  - strcspn(/,.\$!@#$*()'"<>?{})
  char key[] = "/,.<>?:';[]{}\\|)(*&^%$#@!-+=~`\"";
  if (strcspn(arguments[1], key) != strlen(arguments[1])) {
    std::cout << "Syntax Error. Variable Name: " << arguments[1] << " is invalid!\n";
    return;
  }
  std::string value;
  char * start = strstr(inputline, arguments[2]);
  if (start == NULL) {  //If it was a variable
    value = (arguments[2]);
  }
  else {  //If it was any other value
    value = (start);
  }
  std::cout << "Value from arg[2] start: " << value << std::endl;
  mymap[arguments[1]] = value;
}

//Value Parser: Replaces variables declared in commandline with their mapped/set values
void value_parser(char ** arguments,
                  int arg_count,
                  std::unordered_map<std::string, std::string> & mymap) {
  //Scan through all arg[i] [1 : arg_count-1]
  for (int i = 0; i < arg_count; i++) {
    if (arguments[i][0] == '$') {
      std::cout << "$variable found: " << arguments[i] << std::endl;
      char key[MAX_CHARSIZE] = "";
      char token[] = "$";
      char * iterator = arguments[i];
      //Parse the argument

      while (*iterator != '\0') {  //Parse multiple variables in same argument $var1$var2...$vari
        char temp[MAX_CHARSIZE] = "";
        int incr = strcspn(iterator + 1, token);
        strncpy(temp, iterator + 1, incr);
        iterator = iterator + incr + 1;
        std::cout << "Parsed variable: $" << temp << std::endl;
        std::string key_mymap(temp);
        std::unordered_map<std::string, std::string>::const_iterator got_next =
            mymap.find(key_mymap);
        if (got_next != mymap.end()) {
          std::cout << "Mapped Value: " << got_next->second << std::endl;
          strcat(key, got_next->second.c_str());
          std::cout << "Concatenated value: " << key << std::endl;
        }
        else {  //Debugging only
          std::cout << "Undeclared Variable $" << temp << std::endl;
        }
      }
      if (*key == '\0') {  //In case variable not found
        return;
      }
      strcpy(arguments[i], key);
      std::cout << "Replaced  All Variables in argument with Value: " << key << std::endl;
    }
  }
}

//Increment Variable: Increments a variable in base10
void inc_var(char * arg_1, int arg_count, std::unordered_map<std::string, std::string> & mymap) {
  if (arg_count != 2) {
    std::cout << "Usage: inc <var_name>\n";
  }
  std::string key;
  if (arg_1[0] != '$') {
    key = (arg_1);
  }
  else {
    key = (arg_1 + 1);
  }
  std::unordered_map<std::string, std::string>::const_iterator got_next = mymap.find(key);
  if (got_next == mymap.end()) {
    //If variable not found
    std::string init = "1";
    mymap[key] = init;
    std::cout << "//" << key << " is now \"" << init << "\"" << std::endl;
  }
  else {
    int i = atoi(got_next->second.c_str());
    i++;
    mymap[got_next->first] = std::to_string(i);
    std::cout << "//" << key << " is now \"" << got_next->second.c_str() << "\"" << std::endl;
  }
}

//Export Variable: Exports usr declared variable to environment variables
int export_var(char ** env_array,
               int env_count,
               char * arg_1,
               int arg_count,
               std::unordered_map<std::string, std::string> & mymap) {
  if (arg_count != 2) {
    std::cout << "Usage: export <var_name>\n";
  }
  if (arg_1[0] != '$') {
    std::cout << arg_1 << std::endl;
    std::cout << "Usage: variable name should start with $, try: $<var_name>\n";
  }
  std::string key(arg_1 + 1);
  std::unordered_map<std::string, std::string>::const_iterator got_next = mymap.find(key);
  if (got_next == mymap.end()) {
    //    std::cout << "Undeclared Variable " << temp << std::endl;
    std::cout << "Environment Variable not set\n";
  }
  else {
    std::string env_val = key + "=" + got_next->second;
    int pre_set = 0;
    for (int i = 0; i < env_count; i++) {  //Check if env_var already set
      if (strstr(env_array[i], arg_1 + 1) != NULL) {
        strcpy(env_array[i], env_val.c_str());
        pre_set = 1;
        return env_count;
      }
    }
    if (pre_set == 0) {
      char * var = new char[MAX_CHARSIZE];
      for (int i = 0; i < MAX_CHARSIZE; i++) {
        var[i] = '\0';
      }
      strcpy(var, env_val.c_str());
      env_array[env_count] = var;
      env_count++;
    }
    std::cout << "Setting: " << env_val << std::endl;
  }
  return env_count;
}
//Delete environment array
void free_environment(char ** env_array, int env_count) {
  for (int i = 0; i < env_count; i++) {
    delete[] env_array[i];
  }
  delete[] env_array;
}

//IO redirection parser:
void redir_tokenizer(char * in_direct, char * out_direct, char * input_line) {
  char * temp = input_line;
  char key[] = "<>";

  while (*temp != '\0') {
    char space[] = " ";
    int token = strcspn(temp, key);
    temp = temp + token;
    if (*temp == '>' && *(temp + 1) == ' ') {
      std::cout << "Found output redirection token!\n";
      *temp = '\0';
      temp = temp + 1;
      while (isspace(*temp)) {
        *temp = '\0';
        temp = temp + 1;
      }
      int j = strcspn(temp, space);
      strncpy(out_direct, temp, j);
    }
    if (*temp == '<' && *(temp + 1) == ' ') {
      std::cout << "Found input redirection token!\n";
      *temp = '\0';
      temp = temp + 1;
      while (isspace(*temp)) {
        *temp = '\0';
        temp = temp + 1;
      }
      int j = strcspn(temp, space);
      strncpy(in_direct, temp, j);
    }
  }
  std::cout << "AFTER:" << temp << std::endl;
  std::cout << "AFTER '>':" << out_direct << std::endl;
  std::cout << "AFTER '<':" << in_direct << std::endl;
}

//Initializations
//Initialize argument array
char ** init_arg_array() {
  char ** array_of_arguments = new char *[MAX_ARGSIZE];
  for (int i = 0; i < MAX_ARGSIZE; i++) {
    array_of_arguments[i] = NULL;
  }
  return array_of_arguments;
}
//Initialize IO Redirection array
char * init_redir_array() {
  char * redir_string = new char[MAX_CHARSIZE];
  for (int i = 0; i < MAX_CHARSIZE; i++) {
    redir_string[i] = '\0';
  }
  return redir_string;
}

//Invoke run_process by checking env variables and preparing file names
void invoke_process(char ** array_of_arguments,
                    char ** env_array,
                    char * in_direct,
                    char * out_direct,
                    std::vector<std::string> & paths) {
  if (check_path_type(array_of_arguments)) {
    run_process(array_of_arguments[0], array_of_arguments, env_array, in_direct, out_direct);
  }
  else {
    int found = 0;
    for (std::vector<std::string>::const_iterator it = paths.begin(); it != paths.end(); ++it) {
      std::string path = *it + "/" + array_of_arguments[0];
      if (access(path.c_str(), F_OK) == 0) {
        //Run Command
        char path_c[MAX_CHARSIZE];
        strcpy(path_c, path.c_str());
        run_process(path_c, array_of_arguments, env_array, in_direct, out_direct);
        found = 1;
        break;
      }
    }
    if (found != 1) {
      std::cout << "Command " << array_of_arguments[0] << " not found.\n";
    }
  }
}

int main() {
  char input_line[MAX_CHARSIZE];
  std::string cd = "cd";
  // This code fetches the environment variables in a string and stores them in an array
  std::vector<std::string> paths;
  path_parser(paths);

  //Set environment variables
  char ** env_array = init_arg_array();
  int env_count = 0;
  env_count = set_env(env_array, env_count);

  //Declare unordered map
  std::unordered_map<std::string, std::string> mymap;

  while (1) {
    //getcwd
    char buf[MAX_CHARSIZE] = "";
    std::cout << "myShell:" << getcwd(buf, MAX_CHARSIZE) << " $ ";
    fgets(input_line, MAX_CHARSIZE, stdin);
    input_line[strlen(input_line) - 1] = '\0';

    std::cout << " BEFORE: " << input_line << std::endl;

    //Parse io direction array initialization '<' or '>'
    char * in_direct = init_redir_array();
    char * out_direct = init_redir_array();
    redir_tokenizer(in_direct, out_direct, input_line);

    //Set argument array
    char ** array_of_arguments = init_arg_array();

    //Argument Parsing
    int arg_count = 0;
    arg_count = arg_parser(array_of_arguments, input_line);
    char arg_1[MAX_CHARSIZE] = "";
    if (arg_count > 1) {
      strcpy(arg_1, array_of_arguments[1]);
    }

    //Value Parse
    value_parser(array_of_arguments, arg_count, mymap);

    //Check Shell Exit Status, Enter key or EOF or change directory
    if (shell_exit(input_line)) {  //When user types exit or EOF
      free_memory(array_of_arguments, in_direct, out_direct, arg_count);  //, env);
      break;
    }
    if (array_of_arguments[0] == NULL) {  //When user presses only enter
      free_memory(array_of_arguments, in_direct, out_direct, arg_count);  //, env);
      continue;
    }
    if (array_of_arguments[0] == cd) {  //When change directory invoked
      change_dir(array_of_arguments, arg_count);
      free_memory(array_of_arguments, in_direct, out_direct, arg_count);  //, env);
      continue;
    }

    //Variable checker
    int var_check = variable_check(array_of_arguments);
    if (var_check > 0) {
      //if 1 : call set_var
      if (var_check == 1) {
        set_var(array_of_arguments, input_line, mymap, arg_count);
        free_memory(array_of_arguments, in_direct, out_direct, arg_count);  //, env);
        continue;
      }
      //if 2 : call export_var
      if (var_check == 2) {
        env_count = export_var(env_array, env_count, arg_1, arg_count, mymap);
        free_memory(array_of_arguments, in_direct, out_direct, arg_count);  //, env);
        continue;
      }
      //if 3 : call inc_var
      if (var_check == 3) {
        inc_var(arg_1, arg_count, mymap);
        free_memory(array_of_arguments, in_direct, out_direct, arg_count);  //, env);
        continue;
      }
    }

    //Search all paths in envi var for binary file specified in input
    invoke_process(array_of_arguments, env_array, in_direct, out_direct, paths);
    //Delete argument array
    free_memory(array_of_arguments, in_direct, out_direct, arg_count);  //, env);
  }
  //Delete environment array
  free_environment(env_array, env_count);
  return 0;
}
