#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

extern char **getline(void);

void exeCommand (char *command, char **args){
   if (strcmp(command, "cd") == 0) {
      chdir(args[0]);
   } else {
      execvp(command, args);
   }
}

void newProc (char *command, char **args) {
   pid_t pid;
   int status;
   int i = 0;
   
   pid = fork();

   if (pid == 0) {
      exeCommand(command, args);
   } else {
      waitpid(pid, &status,0);
      /*
      printf("Command: %s Arguments:",c); 
      while(a[i] != NULL){
         printf("%s ",a[i]);
         i++;
      } 
      printf("\n");
      */
   }
}

int main (void) {
   int i;
   char **args;

   while(1) {
      printf("SwagShell> ");
      args = getline();

      if (args[0] != NULL) {
         if (strcmp(args[0], "exit") == 0) {
            _exit(0);
         } else {
            if(args[1] == NULL){
               newProc(args[0], args);
            } else {
               newProc(args[0], args+1);
            }
         }
      }
   }
}
