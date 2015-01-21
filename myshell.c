#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

extern char **getline();

pid_t pid;

void commands (char *c, char **a) {
   int status;
   if (strcmp(c, "cd") == 0) {
   } 
   else {
      if (pid == 0) {
         execvp(c, a);
      }
      else {
         waitpid(pid, &status,0);
      }
   }
}

int main (void) {
   int i;
   char **args;
   
   while(1) {
      printf("swagshell> ");
      args = getline();
      pid = fork();
	  

      if (args[0] != NULL) {
         if (strcmp(args[0], "exit") == 0) {
            _exit(0);
         }
         else {
            commands(args[0], args);
         }
      }
   }
}
