#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

extern char **getline(void);

void procResolution(int r) {
   printf("errno is %d\n",errno);
}

void exeCommand (char *command, char **args){
   if (strcmp(command, "cd") == 0) {
      if (args[0] == NULL){
         procResolution(chdir("/root"));
      } else { 
         procResolution(chdir(args[0]));
      }
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
      waitpid(pid, &status, 0);
   }
}
int length (char **a) {
   int len = 0;
   while (a[len] != NULL) {
      len++;
   }
   return len;
}

void parray (char **a) {
   int i;
   for (i = 0; a[i] != NULL; i++) {
      printf("%s ", a[i]);
   } 
   printf("\n");
}

void mypipe (char **args, int end) {
   int i, j;
   int r = 0, lpi = 0;

   for (i = 0; i < end; i++) {
      if (strcmp(args[i], "|") == 0) {
         lpi = i;
      } else if (strcmp(args[i], ">") == 0) {
         r = i;
      } else if (strcmp(args[i], "<") == 0) {
         r = i;
      }
   } 

   if (r == 0) {
   } else {
      char **rhs = args+r+1;
      /*
      char **lhs;
      for (j = 0; j < r; j++) {
         lhs[j] = args[j];
      }
      */
   }

   if (lpi == 0) {
      newProc(args[0], args);
   } else {
       char **last = args+lpi+1;
       char **init = args;
       parray(last);
       for (j = 0; init[j] != NULL; j++) {
          if (j >= lpi) {
             init[j] = NULL;
          }
       } 
       parray(init);
       parray(last);
   }
} 

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
            mypipe(args, length(args));
           /* newProc(args[0], args); */
         }
      }
   }
}
