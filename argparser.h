#ifndef __ARGPARSER_H__
#define __ARGPARSER_H__

#include <stdio.h>
#include <string.h>

#include <iostream>

#include "function.h"

/*arg_parser function: Parses input command line by user into different arguments
storing them in an array. It parses arguments separated by spaces and also takes
the escape sequence '\' to include additional space.*/
int arg_parser(char ** array, char * input_line) {
  int arg_count = 0;
  int j = 0;
  char key[] = " ";
  char * temp = input_line;
  //Start Parsing from beginning of input string, iterating over line charcterwise
  while (*temp != '\0') {
    //Error Check for Size Overflow.
    if (arg_count >= MAX_ARGSIZE) {
      fprintf(stderr, "No. of args overflowed from MAX_ARGSIZE of 128.\n");
      exit(EXIT_FAILURE);
    }
    //Variable to store individual argument
    char * argument = init_redir_array();
    while (isspace(*temp)) {
      temp = temp + 1;
    }
    //Finds the first space and uses it as delimiter for parsing
    j = strcspn(temp, key);
    //Code for checking if escape sequence is present.
    if (*(temp + j - 1) == '\\') {
      do {
        //Local char array for storing intermediate characters
        char intermediate[MAX_CHARSIZE] = "";
        *(temp + j - 1) = ' ';
        strncpy(intermediate, temp, j);
        strcat(argument, intermediate);
        //Move temp to "AFTER the SPACE" prior to backslash and check for previous backslash
        temp = temp + j + 1;
        if (*temp == '\0') {
          break;
        }
        j = strcspn(temp, key);
        if (j == 0) {
          break;
        }
      } while (*(temp + j - 1) == '\\');
      char intermediate[MAX_CHARSIZE] = "";
      strncpy(intermediate, temp, j);
      strcat(argument, intermediate);
      array[arg_count] = argument;
      temp = temp + j + 1;
      arg_count++;
    }
    //In case of standard input without escape sequence
    else {
      strncpy(argument, temp, j);
      array[arg_count] = argument;
      temp = temp + j + 1;
      arg_count++;
    }
  }
  return arg_count;
}
#endif
