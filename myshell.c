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
   int ind;
   for (ind = 0; a[ind] != NULL; ind++) {
      printf("%s ", a[ind]);
   } 
   printf("\n");
}

void mypipe (char **args, int end) {
   int n;
   int lpi = 0;

   for (n = 0; n < end; n++) {
      if (strcmp(args[n], "|") == 0) {
         lpi = n;
      }
   } 

   if (lpi == 0) {
      newProc(args[0], args);
      printf("poop\n");
   } else {
      char **last = args+lpi+1;
      /*
      char **init = memcpy(init, &args, sizeof(*args) * lpi);
      parray(init);
      */
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
