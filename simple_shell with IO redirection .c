#include <fcntl.h>
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

// after redirection we need to handle argv correctly
void redirection(int *argc, char **argv) {
    for (int x = 1; x < (*argc); x++) {
        if (!argv[x]) continue;

        if (argv[x][0] == '>' && argv[x][1] == '\0') {
            if (x + 1 >= *argc || !argv[x + 1]) {
                fprintf(stderr, "syntax: > FILE\n");
                continue;
            }
            int fd = open(argv[x + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                printf("err in redirecting output to %s", argv[x + 1]);
                continue;
            }
            if (dup2(fd, 1) == -1) {
                printf("err in dup2 output ");
            }
            argv[x] = NULL;
            argv[x + 1] = NULL;
            x++;
            close(fd);
        }
        else if (argv[x][0] == '<' && argv[x][1] == '\0') {
            if (x + 1 >= *argc || !argv[x + 1]) {
                fprintf(stderr, "syntax: < FILE\n");
                continue;
            }
            int fd = open(argv[x + 1], O_RDONLY);
            if (fd < 0) {
                printf("err in redirecting input to %s", argv[x + 1]);
                continue;
            }
            if (dup2(fd, 0) == -1) {
                printf("err in dup2 input ");
            }
            argv[x] = NULL;
            argv[x + 1] = NULL;
            x++;
            close(fd);
        }
        else if (argv[x][0] == '2' && argv[x][1] == '>' && argv[x][2] == '\0') {
            if (x + 1 >= *argc || !argv[x + 1]) {
                fprintf(stderr, "syntax: 2> FILE\n");
                continue;
            }
            int fd = open(argv[x + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                fprintf(stderr, "err in redirecting output to %s", argv[x + 1]);
                continue;
            }
            if (dup2(fd, 2) == -1) {
                printf("err in dup2 output ");
            }
            argv[x] = NULL;
            argv[x + 1] = NULL;
            x++;
            close(fd);
        }
    }

    int j = 0;
    for (int i = 0; i < (*argc); i++) {
        if (argv[i]) {
            argv[j++] = argv[i];
        }
    }
    argv[j] = NULL;
    *argc = j;
}

int main() {
    pid_t pid;
    char input[1000];
    int argc;
    char *argv[30];

    while (1) {
        argc = 0;
        printf("simple shell $$   ");
        fflush(stdout);
        fgets(input, 1000, stdin);

        input[strlen(input) - 1] = '\0';

        if (!strcmp(input, "NO")) {
            exit(0);
        }
        if (strlen(input) == 0) {
            continue;
        }

        pid = fork();

        if (pid > 0) {
            int status;
            wait(&status);
        }
        else if (pid == 0) {
            tokenize(input, &argc, argv);
            redirection(&argc, argv);
            execvp(argv[0], argv);
            perror("execvp");
            exit(-1);
        }
        else {
            printf("No child created\n");
            perror("fork failed");
        }
    }

    return 0;
}
