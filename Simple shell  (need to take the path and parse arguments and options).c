#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

void tokenize(char input[], int *argc, char **argv)
{
    char *token = strtok(input, " ");
    while (token) {
        argv[*argc] = token;
        (*argc)++;
        token = strtok(NULL, " ");
    }
    argv[*argc]=NULL;
}

int main()
{
    pid_t pid;
    char input[1000];
    int argc=0 ; // should be initialized to prevent segemntaiton fault
    char *argv[10];
    while (1) {
        printf("simple shell $$   ");

        fgets(input, 1000, stdin);

        input[strlen(input) - 1] = '\0';        // 0  == NULL

        if (!strcmp(input, "NO")) {     // like ^z or ^c
            exit(0);
        }
        if (strlen(input) == 0) {       // no command
            continue;
        }
	      pid = fork();
        if (pid > 0) {
		                int status;
		                printf("parent invoked and wait for child to terminate\n");
		                wait(&status);  // parent call to wait for change state of its child
		                printf("parent return, and status is %d\n",WEXITSTATUS(status));
			  }
        else if (pid == 0) {
                tokenize(input, &argc, argv);
                char *newenvp[] = { NULL };
                execve(argv[0] , argv , newenvp);//need the whole path 
                exit(0);        
                // to terminate after the process terminated  (ig no need for as is we replace this by the new prcoess so it will terminate after the process (commnad))
                // this exit just in case of failure of execve
        }
        else {
                printf("No child created\n");
                perror("fork failed");
        }
    }

         return 0;
    }

