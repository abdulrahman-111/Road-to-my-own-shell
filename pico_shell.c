#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
int last_status = 0;

void my_cd(char * path){
	if(chdir(path)!=0){
 fprintf(stderr, "cd: %s: %s\n", path, strerror(errno));
	last_status = 1; 
	return;
	}
	last_status = 0;
}

void my_pwd(){
    char path[20050];
    getcwd(path,20050);
    strncat(path,"\n ",1);
    if( write (1, path,strlen(path)) <0) {
        exit(-1);
     }
}

void my_echo(int argc , char * argv[]){
 for (int x=1; x <argc; x++ ){
        if (write(1,argv[x],strlen(argv[x]) )<0 ){
        last_status = 1;
        exit (1);
        }
        if (x < argc - 1)  write(1, " ", 1);   
  }
  last_status = 0; 
  write(1,"\n",1);
}

int tokenize(char *input, char ***argv_ptr) {
    int argc = 0, capacity = 10;
    char **argv = (char **) malloc(capacity * sizeof(char *));
    if (!argv) { perror("malloc"); exit(1); }

    char *token = strtok(input, " ");
    while (token) {
        if (argc == capacity) {
            char **tmp = (char **) realloc(argv, 2 * capacity * sizeof(char *));
            if (!tmp) { perror("realloc"); exit(1); }
            argv = tmp;
            capacity *= 2;
        }
        argv[argc] = strdup(token);
        if (!argv[argc]) { perror("strdup"); exit(1); }
        argc++;
        token = strtok(NULL, " ");
    }
    argv[argc] = NULL;
    *argv_ptr = argv;
    return argc;
}


void free_args(char **argv, int argc) {
    for (int i = 0; i < argc; i++) {
        free(argv[i]);
    }
    free(argv);
}

int main (){
		 pid_t pid;
		 
    char input[20050];
  int argc=0;
    char **argv;
		while (1) {
        
        printf("Pico shell prompt >");
			  fflush(stdout);
        
        if (!fgets(input, sizeof(input), stdin)) break;
        
        input[strlen(input) - 1] = '\0';        // 0  == NULL
        
        if (input[0] == '\0') continue;

         argc= tokenize(input, &argv);
        
        if (argc == 0) continue; // empty command, skip
        
        if (!strcmp(argv[0], "exit")) {     // like ^z or ^c
            printf("Good Bye\n");
            fflush(stdout);
            last_status = 0;
            break;
        }
        else if(!strcmp(argv[0], "echo")){
	        my_echo(argc,argv);
	        fflush(stdout);
	        continue;
        }
        else if(!strcmp(argv[0], "pwd")){
	        my_pwd();
	        fflush(stdout);
	        continue;
        }
         else if(!strcmp(argv[0], "cd")){
	        my_cd(argv[1]);
	        fflush(stdout);
	        continue;
        }
        else{
          pid = fork();

            if (pid > 0) {
                int status;
                wait(&status);
                 if (WIFEXITED(status))
                last_status = WEXITSTATUS(status);
            else
                last_status = 1; // abnormal termination
            }

            else if (pid == 0) {
                execvp(argv[0],argv); //use caller (the child of parent)env list
                 fprintf(stderr, "%s: command not found\n", argv[0]);
    exit(127);  // 127 means command not found
            } else {
                perror("fork");
	            last_status = 1;    
            }
        }
        
    }
  free_args(argv, argc); // free memory after each command

return  last_status ;
}
