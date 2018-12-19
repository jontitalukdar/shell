#ifndef __PROCESS_H__
#define __PROCESS_H__
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <iostream>
#include <vector>

#include "function.h"

/* run_process function: This function takes all parsed command line arguments,
environment variables and input/output redirections to create child process for
the user input commands and executes them using fork(), execve() and waitpit() 
system calls.
*/
void run_process(char * filename,
                 char ** arguments,
                 char ** env_name,
                 char * in_direct,
                 char * out_direct) {
  int status;
  pid_t wait_pid;
  int fd_in, fd_out;
  int exec_out, exec_in;

  //Create child process with fork and using the duplicated calling process below.
  pid_t child_pid = fork();

  //Error checking: If fork() failed, return value is -1.
  if (child_pid < 0) {
    fprintf(stderr, "Fork Creation Failed.\n");
    exit(EXIT_FAILURE);
  }

  /*On Success, pid of 0 is returned in child process. Hence, this code is executed 
    in the child thread.*/
  if (child_pid == 0) {
    //Input Redirection: Check if any argument contains input redirection
    if (*in_direct != '\0') {
      //Input filedescriptor opens file in read only mode.
      fd_in = open(in_direct, O_RDONLY);
      if (fd_in < 0) {
        perror("Input Redirection Error");
        exit(EXIT_FAILURE);
      }
      //Create a copy of fd_in as stdin
      exec_in = dup2(fd_in, 0);
      if (exec_in < 0) {
        perror("File Opening Failure ");
        exit(EXIT_FAILURE);
      }
      close(fd_in);
    }

    //Output Redirection: Check if any argument contains output redirection
    if (*out_direct != '\0') {
      /*Output filedescriptor opens an already declared file or creates one if required.
       This is done by setting the corresponding flags and modes. Here the user opens or
      creates file with read write permissions.*/
      mode_t permissions = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;
      fd_out = open(out_direct, O_WRONLY | O_CREAT | O_TRUNC, permissions);
      if (fd_out < 0) {
        perror("Output Redirection Error");
        exit(EXIT_FAILURE);
      }
      //Create a copy of fd_out as stdout
      exec_out = dup2(fd_out, 1);
      if (exec_out < 0) {
        perror("File Opening Error ");
        exit(EXIT_FAILURE);
      }
      close(fd_out);
    }

    //Execve: Execution system for executing with given argument and env array.
    execve(filename, arguments, env_name);
    perror(arguments[0]);

    _exit(1);
  }

  //Parent process belo. Waits until child is complete. Execution flow maintained.
  else {
    wait_pid = waitpid(child_pid, &status, WUNTRACED);
    //Exit status of executed child process returned.
    if (wait_pid < 0) {
      perror("Wait Pid Failed.\n");
      exit(EXIT_FAILURE);
    }
    if (WIFSIGNALED(status)) {
      std::cout << "Program was killed by signal " << WTERMSIG(status) << std::endl;
    }
    if (WIFEXITED(status)) {
      std::cout << "Program exited with status " << WEXITSTATUS(status) << std::endl;
    }
  }
}

//Checks if pathname is absolute or not
int check_path_type(char ** arguments) {
  if (strchr(arguments[0], '/') == NULL) {
    return 0;
  }
  return 1;
}

/*Invoke Process Function: Performs checks in the stored environment variables for the given 
 filename or binary is present or not and modifies filename/executable accordingly. Also invokes
 run_process function for deploying child processes.*/
void invoke_process(char ** array_of_arguments,
                    char ** env_array,
                    char * in_direct,
                    char * out_direct,
                    std::vector<std::string> & paths) {
  //Check for absolute path
  if (check_path_type(array_of_arguments)) {
    run_process(array_of_arguments[0], array_of_arguments, env_array, in_direct, out_direct);
  }
  else {
    int found = 0;
    //Search for stored environment variables if path exists there.
    for (std::vector<std::string>::const_iterator it = paths.begin(); it != paths.end(); ++it) {
      std::string path = *it + "/" + array_of_arguments[0];
      //If path exists, append full path to filename
      if (access(path.c_str(), F_OK) == 0) {
        char path_c[MAX_CHARSIZE];
        strcpy(path_c, path.c_str());
        run_process(path_c, array_of_arguments, env_array, in_direct, out_direct);
        found = 1;
        break;
      }
    }
    //If path not found, generate unrecognized command error.
    if (found != 1) {
      std::cout << "Command " << array_of_arguments[0] << " not found.\n";
    }
  }
}

#endif
