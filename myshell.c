#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>

extern char **getline(void);
int status;
pid_t pid;

/*
 * Takes in an error return value and prints it out.
*/
void procResolution(int r) {
   if (errno != 0) {
      printf("Not enough swag: %s\n",strerror(errno));
   }
}

/*
 * Takes in the first comand in args and the rest
 * of args checks to see if the command is "cd".
 * If so, then do not fork() a new process, but 
 * call chdir() instead. Otherwise, fork() a
 * new process.
*/
void exeCommand (char *command, char **args){
   if (strcmp(command, "cd") == 0) {
      if (args[0] == NULL){
         procResolution(chdir("/root"));
      } else { 
         procResolution(chdir(args[0]));
      }
   } else {
      procResolution(execvp(command, args));
   }
}

/*
 * Takes the first command in args and the rest of
 * args then forks a new process to execute the
 * command in the child process as the parent waits
*/
void newProc (char *command, char **args) {
   pid = fork();
   if (pid == 0) {
      exeCommand(command, args);
   } else {
      waitpid(pid, &status, 0);
   }
}

/*
 * Takes the left and right hand sides of a pipe
 * operand and pipes the output of the left side
 * as input to the right side
*/
void myPipe (char **lhs, char **rhs) {
   int pfd[2];
   pipe(pfd);
   pid = fork();

   if (pid == 0) {
      dup2(pfd[1], 1);
      exeCommand(lhs[0], lhs);
   } else {
      dup2(pfd[0], 0);
      close(pfd[1]);
      exeCommand(rhs[0], rhs);
   }
} 

/*
 * Takes in the left and right hand sides of a redirect
 * operand as well as a flag that indicates which operand
 * was used ("<" or ">"). Then performs the proper redirection
 * procedure based on the given operand
*/
void myRedir (char **lhs, char **rhs, int operand) {
   pid = fork();
   if (pid == 0) {
      if (operand == 1) {
         /* command ">" file */
         int f = open(rhs[0], O_WRONLY | O_CREAT | O_TRUNC);
         if (f < 0) {
            printf("Error: %d is less than 0\n", f);
         } else {
            int c;
	    dup2(f, 1);
            exeCommand(lhs[0], lhs);
            close(f);
	 }
      } else {
         /* command "<" file */
         int f = open(rhs[0], O_RDONLY);
         if (f < 0) {
            printf("Error: %d is less than 0\n", f);
         } else {
            dup2(f, 0);
            exeCommand(lhs[0], lhs);
            close(f);
	 }
      }
   } else {
      waitpid(pid, &status, 0);
   }
} 

/*
 * Takes in args and checks if there is a redirection
 * or pipe operand. If so, either call myRedir() or
 * myPipe on the corresponding scenario. Otherwise,
 * just call newProc() to fork() a new process and 
 * execute the command
*/
void myExec (char **args) {
   int i, operand = 0, red = 0, lpi = 0;

   for (i = 0; args[i] != NULL; i++) {
      if (strcmp(args[i], ">") == 0) {
         red = i;
         operand = 1;
      } else if (strcmp(args[i], "<") == 0) {
         red = i;
      } else if (strcmp(args[i], "|") == 0) {
         lpi = i;
      }
   } 

   if (red == 0 && lpi == 0) {
      newProc(args[0], args);
   } else if (red != 0) {
      /* redirecting stuff */
      char **right = args+red+1;
      char **left = malloc(sizeof(*args) * red);
      left = memcpy(left, args, sizeof(*args) * red);
      myRedir(left, right, operand);
   } else if (lpi != 0) {
      /* piping stuff */
      char **right = args+lpi+1;
      char **left = malloc(sizeof(*args) * lpi);
      left = memcpy(left, args, sizeof(*args) * lpi);
      myPipe(left, right);
   }
} 

/*
 * Main function checks if the command given in args[0] is 
 * "exit". If so then exit the shell. If command is "cd" then 
 * call exeCommand(). Otherwise, call myExec()
*/
int main (void) {
   int i;
   char **args;

   printf("You are now running SwagShell. The swaggiest shell around.\n");
   printf("Created by David Carranza and Justin Barros.\n");

   while(1) {
      printf("SwagShell> ");
      args = getline();
      if (args[0] != NULL) {
         if (strcmp(args[0], "exit") == 0) {
            _exit(0);
         } else if (strcmp(args[0], "cd") == 0) {
            exeCommand(args[0], args+1); 
         } else {
            myExec(args);
           /* newProc(args[0], args); */
         }
      }
   }
}
