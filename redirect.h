#ifndef __REDIRECT_H__
#define __REDIRECT_H__

#include "function.h"

/*redir_tokenizer function: This function parses the input command line looking
for '>' or '<', IO redirection tokens, and parsing the subsequent file arguments
into separate character arrays.*/
void redir_tokenizer(char * in_direct, char * out_direct, char * input_line) {
  char * temp = input_line;
  char key[] = "<>";
  //Iterate over input line looking for '<' or '>'
  while (*temp != '\0') {
    char space[] = " ";
    int token = strcspn(temp, key);
    temp = temp + token;
    //If '>' OUTPUT redirection found
    if (*temp == '>') {
      *temp = '\0';
      temp = temp + 1;
      if (*temp == '\0') {
        return;
      }
      while (isspace(*temp)) {
        *temp = '\0';
        temp = temp + 1;
      }
      int j = strcspn(temp, space);
      //Store filename in output redirection array
      strncpy(out_direct, temp, j);
    }
    //If '<' INPUT redirection found
    if (*temp == '<') {
      *temp = '\0';
      temp = temp + 1;
      if (*temp == '\0') {
        return;
      }
      while (isspace(*temp)) {
        *temp = '\0';
        temp = temp + 1;
      }
      int j = strcspn(temp, space);
      //Store filename in input redirect array
      strncpy(in_direct, temp, j);
    }
  }
}

#endif
