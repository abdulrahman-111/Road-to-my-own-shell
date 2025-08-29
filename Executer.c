#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Program to execute a given ELF binary using execve
int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <path-to-elf>\n", argv[0]);
        exit(-1);
    }

    // Arguments for the new program: first is program name, followed by NULL
    char *newargv[] = { argv[1], NULL };
    // No environment variables passed
    char *newenvp[] = { NULL };

    // Replace current process image with the new one
    execve(argv[1], newargv, newenvp);

    // If execve returns, an error occurred
    perror("execve failed");
    return 1;
}
