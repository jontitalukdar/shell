#ifndef __VARIABLES_H__
#define __VARIABLES_H__
#include <unordered_map>

#include "function.h"

/*variable_check function: Checks if the input for build in Variable commands.*/
int variable_check(char ** arguments) {
  //commmand names
  std::string set = "set";
  std::string exp = "export";
  std::string inc = "inc";
  if (arguments[0] == set) {  //if set invoked
    return 1;
  }
  if (arguments[0] == exp) {  //if export invoked
    return 2;
  }
  if (arguments[0] == inc) {  //if inc invoked
    return 3;
  }
  else {
    return 0;  //if no command invoked.
  }
}

/*Value Parser: Replaces variables declared in commandline with their mapped
values set by the user through command line arguments including set or inc. */
void value_parser(char ** arguments,
                  int arg_count,
                  std::unordered_map<std::string, std::string> & mymap) {
  //Scan through all parsed arguments from [1 : arg_count-1] because arg[0] is filename
  for (int i = 0; i < arg_count; i++) {
    if (arguments[i][0] == '$') {
      char key[MAX_CHARSIZE] = "";
      char token[] = "$";
      char * iterator = arguments[i];

      //Parse multiple variables in same argument like: $var1$var2...$vari
      while (*iterator != '\0') {
        char temp[MAX_CHARSIZE] = "";
        //Find index of subsequent declared $var
        int incr = strcspn(iterator + 1, token);
        strncpy(temp, iterator + 1, incr);
        iterator = iterator + incr + 1;
        //Check mapping of variable with stored env variables.
        std::string key_mymap(temp);
        std::unordered_map<std::string, std::string>::const_iterator got_next =
            mymap.find(key_mymap);
        if (got_next != mymap.end()) {
          strcat(key, got_next->second.c_str());
        }
        //If variable in $ format not found, shell returns prompt of undeclared variable
        else {
          std::cout << "Undeclared Variable $" << temp << std::endl;
        }
      }
      if (*key == '\0') {  //In case variable not found
        return;
      }
      strcpy(arguments[i], key);
    }
  }
}

/*Set Variable: Set the variable var to the string on the rest of the
 command line (even if it contains spaces, etc.) The shell remembers this
 value, and makes use of it in future $ evaluations by storing it in a hash
 table (unordered_map). */
void set_var(char ** arguments,
             char * inputline,
             std::unordered_map<std::string, std::string> & mymap,
             int arg_count) {
  if (arg_count < 3) {
    std::cout << "Usage: set <var_name> <var_value>\n";
    return;
  }

  //A valid Variable declaration only includes letters, numbers and underscores.
  //Hence, Check if arg[1] = var_name is valid  - strcspn(/,.\$!@#$*()'"<>?{})
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
  mymap[arguments[1]] = value;
  std::cout << "//" << arguments[1] << " is now \"" << value << "\"" << std::endl;
}

/* inc_var function:Interprets the value of var as a number (base10), and 
   increments it. If not a number, treated as 0 and incremented*/
void inc_var(char * arg_1, int arg_count, std::unordered_map<std::string, std::string> & mymap) {
  if (arg_count != 2) {
    std::cout << "Usage: inc <var_name>\n";
    return;
  }
  std::string key;
  //Check if input argument is already a variable
  if (arg_1[0] != '$') {
    key = (arg_1);
  }
  else {
    key = (arg_1 + 1);
  }
  std::unordered_map<std::string, std::string>::const_iterator got_next = mymap.find(key);
  //If variable is not found in hash lookup, it is added to map and incremented.
  if (got_next == mymap.end()) {
    std::string init = "1";
    mymap[key] = init;
    std::cout << "//" << key << " is now \"" << init << "\"" << std::endl;
  }
  //If variable is found in lookup, it is converted to int and incremented.
  else {
    int i = atoi(got_next->second.c_str());
    i++;
    mymap[got_next->first] = std::to_string(i);
    std::cout << "//" << key << " is now \"" << got_next->second.c_str() << "\"" << std::endl;
  }
}

/*export_var function: Exports user declared variable to environment variables
for other programs. */
int export_var(char ** env_array,
               int env_count,
               char * arg_1,
               int arg_count,
               std::unordered_map<std::string, std::string> & mymap) {
  if (arg_count != 2) {
    std::cout << "Usage: export <var_name>\n";
  }
  //Raise prompt if variable is undeclared or not specified by user properly
  if (arg_1[0] != '$') {
    std::cout << "Usage: variable name should start with $, try: $<var_name>\n";
  }
  std::string key(arg_1 + 1);
  std::unordered_map<std::string, std::string>::const_iterator got_next = mymap.find(key);
  //Look for env variable in mapped table
  if (got_next != mymap.end()) {
    std::string env_val = key + "=" + got_next->second;
    //Check if env_var already set, if so reset it
    int pre_set = 0;
    for (int i = 0; i < env_count; i++) {
      if (strstr(env_array[i], arg_1 + 1) != NULL) {
        strcpy(env_array[i], env_val.c_str());
        pre_set = 1;
        return env_count;
      }
    }
    //Otherwise add variable to environment array
    if (pre_set == 0) {
      char * var = new char[MAX_CHARSIZE];
      for (int i = 0; i < MAX_CHARSIZE; i++) {
        var[i] = '\0';
      }
      strcpy(var, env_val.c_str());
      env_array[env_count] = var;
      env_count++;
    }
  }
  return env_count;
}

#endif
