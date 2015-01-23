#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

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

void parray (char **a) {
   int i;
   for (i = 0; a[i] != NULL; i++) {
      printf("%s ", a[i]);
   } 
   printf("\n");
}

void myPipe (char **lhs, char **rhs) {
   /* pipe */
} 

void myRedir (char **lhs, char **rhs, int operand) {
   int status;
   pid_t pid;
   pid = fork();

   if (pid == 0) {
      if (operand == 1) {
         int f = open(rhs[0], O_RDWR, S_IRUSR | S_IWUSR);
         if (f < 0) {
            printf("Error: %d is less than 0\n", f);
         }
         close(1);
         dup(f);
         newProc(lhs[0], lhs);
         _exit(0);
      } else {
         /* do stuff for "<" */
         parray(lhs);
         parray(rhs);
      }
   } else {
      waitpid(pid, &status, 0);
   }
} 

void myExec (char **args) {
   int i, toFile = 0, red = 0, lpi = 0;

   for (i = 0; args[i] != NULL; i++) {
      if (strcmp(args[i], ">") == 0) {
         red = i;
         toFile = 1;
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
      char **left = malloc(sizeof(char **));
      left = memcpy(left, args, sizeof(*args) * red);
      myRedir(left, right, toFile);
   } else if (lpi != 0) {
      /* piping stuff */
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
            myExec(args);
           /* newProc(args[0], args); */
         }
      }
   }
}
