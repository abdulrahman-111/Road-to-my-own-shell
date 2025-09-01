#include <stdbool.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int last_status = 0;

struct local_variable {
    char *name;
    char *value;
};

typedef struct local_variable local_variable;
local_variable **lv = NULL;
static int lv_counter = 0;
int cap_var = 10;

local_variable *create(char *name, char *value);
bool check_var(char *string);

void my_cd(char *path);
void my_pwd();
void my_echo(int argc, char *argv[]);
int tokenize(char *input, char ***argv_ptr);
void free_args(char **argv, int argc);
void free_local_variables();
void my_export(char *string);
void add_local_variable(char *string);
char *get_local_variable(const char *name);
char *expand_variable(const char *string);
void redirection(int *argc, char **argv);

int microshell_main(int argc1, char *argv1[]) {
    pid_t pid;
    char input[20050];
    int argc = 0;
    char **argv;

    lv = (local_variable **)malloc(cap_var * sizeof(local_variable *));
    if (!lv) {
        perror("malloc");
        exit(1);
    }

    while (1) {
        printf("Nano Shell Prompt >");
        fflush(stdout);

        if (!fgets(input, sizeof(input), stdin)) break;
        input[strlen(input) - 1] = '\0';

        if (input[0] == '\0') continue;

        argc = tokenize(input, &argv);
        if (argc == 0) continue;

        if (!strcmp(argv[0], "exit")) {
            printf("Good Bye\n");
            fflush(stdout);
            free_args(argv, argc);
            last_status = 0;
            break;
        }
        else if (strchr(argv[0], '=')) {
            if (argc > 1) {
                printf("Invalid command\n");
                last_status = 1;
            } else {
                add_local_variable(argv[0]);
            }
        }
        else if (!strcmp(argv[0], "export")) {
            if (argc < 2) {
                printf("Invalid command\n");
                last_status = 1;
            } else {
                my_export(argv[1]);
            }
        }
        else if (!strcmp(argv[0], "pwd")) {
            my_pwd();
            fflush(stdout);
        }
        else if (!strcmp(argv[0], "cd")) {
            my_cd(argv[1]);
            fflush(stdout);
        }
        else {
            pid = fork();
            if (pid > 0) {
                int status;
                wait(&status);
                if (WIFEXITED(status)) {
                    last_status = WEXITSTATUS(status);
                } else if (WIFSIGNALED(status)) {
                    last_status = 128 + WTERMSIG(status);
                } else {
                    last_status = 1;
                }
            }
            else if (pid == 0) {
                redirection(&argc, argv);
                execvp(argv[0], argv);
                free_local_variables();
                free_args(argv, argc);
                exit(127);
            }
            else {
                perror("fork");
                last_status = 1;
            }
        }

        free_args(argv, argc);
        argv = NULL;
    }

    free_local_variables();
    return last_status;
}

char *get_local_variable(const char *name) {
    for (int i = 0; i < lv_counter; i++) {
        if (strcmp(lv[i]->name, name) == 0) {
            return lv[i]->value;
        }
    }
    return NULL;
}

bool check_var(char *string) {
    char *temp = strchr(string, '=');
    if (!temp || temp == string || *(temp + 1) == '\0') {
        last_status = 1;
        return 0;
    }

    for (char *p = string; *p; p++) {
        if (*p == ' ') {
            last_status = 1;
            return 0;
        }
    }
    return 1;
}

void add_local_variable(char *string) {
    if (!check_var(string)) {
        last_status = 1;
        return;
    }

    char *temp = strchr(string, '=');
    *temp = '\0';
    char *name = string;
    char *value = temp + 1;

    for (int i = 0; i < lv_counter; i++) {
        if (strcmp(lv[i]->name, name) == 0) {
            free(lv[i]->value);
            lv[i]->value = strdup(value);
            last_status = 0;
            return;
        }
    }

    if (lv_counter >= cap_var) {
        cap_var *= 2;
        lv = (local_variable **)realloc(lv, cap_var * sizeof(local_variable *));
        if (!lv) {
            perror("realloc");
            exit(1);
        }
    }

    lv[lv_counter++] = create(name, value);
    last_status = 0;
}

local_variable *create(char *name, char *value) {
    local_variable *temp = (local_variable *)malloc(sizeof(local_variable));
    if (!temp) {
        perror("malloc");
        exit(1);
    }
    temp->name = strdup(name);
    temp->value = strdup(value);
    return temp;
}

void my_export(char *name) {
    char *val = get_local_variable(name);
    if (!val) {
        printf("Invalid command\n");
        last_status = 1;
        return;
    }
    if (setenv(name, val, 1) == 0) {
        last_status = 0;
    } else {
        perror("setenv");
        last_status = 1;
    }
}

void my_cd(char *path) {
    if (chdir(path) != 0) {
        fprintf(stderr, "cd: %s: %s\n", path, strerror(errno));
        last_status = 1;
        return;
    }
    last_status = 0;
}

void my_pwd() {
    char path[20050];
    getcwd(path, 20050);
    strncat(path, "\n", 1);
    if (write(1, path, strlen(path)) < 0) {
        exit(-1);
    }
    last_status = 0;
}

void my_echo(int argc, char *argv[]) {
    for (int x = 1; x < argc; x++) {
        if (write(1, argv[x], strlen(argv[x])) < 0) {
            last_status = 1;
            exit(1);
        }
        if (x < argc - 1) write(1, " ", 1);
    }
    last_status = 0;
    write(1, "\n", 1);
}

int tokenize(char *input, char ***argv_ptr) {
    int argc = 0, capacity = 10;
    char **argv = (char **)malloc(capacity * sizeof(char *));
    if (!argv) {
        perror("malloc");
        exit(1);
    }

    char *token = strtok(input, " ");
    while (token) {
        if (argc == capacity) {
            capacity *= 2;
            char **tmp = (char **)realloc(argv, capacity * sizeof(char *));
            if (!tmp) {
                perror("realloc");
                exit(1);
            }
            argv = tmp;
        }

        char *expanded = expand_variable(token);
        argv[argc] = expanded;
        argc++;
        token = strtok(NULL, " ");
    }

    argv[argc] = NULL;
    *argv_ptr = argv;
    return argc;
}

char *expand_variable(const char *string) {
    size_t len = strlen(string);
    char *result = (char *)malloc(1);
    if (!result) {
        perror("malloc");
        exit(1);
    }
    result[0] = '\0';

    for (size_t i = 0; i < len;) {
        if (string[i] == '$') {
            i++;
            size_t start = i;
            while (i < len && (isalnum(string[i]) || string[i] == '_')) i++;
            size_t var_len = i - start;

            char var_name[256];
            strncpy(var_name, string + start, var_len);
            var_name[var_len] = '\0';

            char *val = get_local_variable(var_name);
            if (!val) val = getenv(var_name);
            if (!val) val = "";

            result = (char *)realloc(result, strlen(result) + strlen(val) + 1);
            if (!result) {
                perror("realloc");
                exit(1);
            }
            strcat(result, val);
        }
        else {
            size_t start = i;
            while (i < len && string[i] != '$') i++;
            size_t seg_len = i - start;

            result = (char *)realloc(result, strlen(result) + seg_len + 1);
            if (!result) {
                perror("realloc");
                exit(1);
            }
            strncat(result, string + start, seg_len);
        }
    }
    return result;
}

void free_args(char **argv, int argc) {
    for (int i = 0; i < argc; i++) {
        free(argv[i]);
    }
    free(argv);
}

void free_local_variables() {
    for (int i = 0; i < lv_counter; i++) {
        if (lv[i]) {
            free(lv[i]->name);
            free(lv[i]->value);
            free(lv[i]);
            lv[i] = NULL;
        }
    }
    free(lv);
    lv_counter = 0;
}

void redirection(int *argc, char **argv) {
    for (int x = 1; x < (*argc); x++) {
        if (!argv[x]) continue;

        if (argv[x][0] == '>' && argv[x][1] == '\0') {
            if (x + 1 >= *argc || !argv[x + 1]) {
                fprintf(stderr, "syntax: > FILE\n");
                last_status = 1;
                exit(1);
            }
            int fd = open(argv[x + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                fprintf(stderr, "%s: %s\n", argv[x + 1], strerror(errno));
                last_status = 1;
                exit(1);
            }
            if (dup2(fd, 1) == -1) {
                perror("dup2");
                last_status = 1;
                exit(1);
            }
            argv[x] = NULL;
            argv[x + 1] = NULL;
            x++;
            close(fd);
        }
        else if (argv[x][0] == '<' && argv[x][1] == '\0') {
            if (x + 1 >= *argc || !argv[x + 1]) {
                fprintf(stderr, "syntax: < FILE\n");
                last_status = 1;
                exit(1);
            }
            int fd = open(argv[x + 1], O_RDONLY);
            if (fd < 0) {
                fprintf(stderr, "cannot access %s: %s\n", argv[x + 1], strerror(errno));
                last_status = 1;
                exit(1);
            }
            if (dup2(fd, 0) == -1) {
                perror("dup2");
                last_status = 1;
                exit(1);
            }
            argv[x] = NULL;
            argv[x + 1] = NULL;
            x++;
            close(fd);
        }
        else if (argv[x][0] == '2' && argv[x][1] == '>' && argv[x][2] == '\0') {
            if (x + 1 >= *argc || !argv[x + 1]) {
                fprintf(stderr, "syntax: 2> FILE\n");
                last_status = 1;
                exit(1);
            }

            int fd = open(argv[x + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                fprintf(stderr, "cannot access %s: %s\n", argv[x + 1], strerror(errno));
                last_status = 1;
                exit(1);
            }
            if (dup2(fd, 2) == -1) {
                fprintf(stderr, "cannot access %s: %s\n", argv[x + 1], strerror(errno));
                last_status = 1;
                exit(1);
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
