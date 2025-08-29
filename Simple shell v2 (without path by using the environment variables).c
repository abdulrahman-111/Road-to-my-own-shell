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
    int argc=0 ;
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
                wait(&status);  // parent call to wait for change state of its child
            }

            else if (pid == 0) {
                tokenize(input, &argc, argv);

                char *newenvp[] = { NULL };
                // this is a custom env list that i can pass to the new process
                execvp(argv[0],argv);
                // no need for typing the full path, just the command enough
                // this passes the current env list
                exit(0);
                // to terminate after the process terminated  (ig no need for as is we replace this by the new prcoess so it will terminate after the process (commnad))
                // this exit just in case of failure of execve
            } else {
                printf("No child created\n");
                perror("fork failed");
            }
    }

         return 0;
    }

