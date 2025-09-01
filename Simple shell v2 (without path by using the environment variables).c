#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

void tokenize(char input[], int *argc, char **argv) {
    char *token = strtok(input, " ");
    while (token) {
        argv[*argc] = token;
        (*argc)++;
        token = strtok(NULL, " ");
    }
    argv[*argc] = NULL;
}

int main() {
    pid_t pid;
    char input[1000];
    int argc = 0;
    char *argv[10];

    while (1) {
        printf("simple shell $$   ");
        fgets(input, 1000, stdin);

        input[strlen(input) - 1] = '\0';  // remove newline

        if (!strcmp(input, "NO")) {       // exit command
            exit(0);
        }

        if (strlen(input) == 0) {         // empty command
            continue;
        }

        pid = fork();

        if (pid > 0) {
            int status;
            wait(&status);  // parent waits for child
        } else if (pid == 0) {
            tokenize(input, &argc, argv);
            char *newenvp[] = { NULL };

            execvp(argv[0], argv);  // execute command
            exit(0);  // exit if execvp fails
        } else {
            printf("No child created\n");
            perror("fork failed");
        }
    }

    return 0;
}
