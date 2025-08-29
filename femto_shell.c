#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

void my_echo(int argc , char ** argv){
 for (int x=1; x <argc; x++ ){
        if (write(1,argv[x],strlen(argv[x]) )<0 ){
        exit (1);
        }
        if (x < argc - 1)  write(1, " ", 1);
  }
  write(1,"\n",1);
}

int tokenize(char input[] ,char **argv)
{
		int argc=0;
    char *token = strtok(input, " ");
    while (token  && argc < 1100 - 1) {
        argv[argc] = token;
        (argc)++;
        token = strtok(NULL, " ");
    }
    argv[argc]=NULL;
    return argc;
}


int main (){
    char input[20050];
  
    char *argv[1100];
		while (1) {
        
        printf("Femto shell prompt >");
			  fflush(stdout);
        
        if (!fgets(input, sizeof(input), stdin)) break;
        
        input[strlen(input) - 1] = '\0';        // 0  == NULL
        
        if (input[0] == '\0') continue;

        int argc= tokenize(input, argv);
        
        if (argc == 0) continue; // empty command, skip
        
        if (!strcmp(argv[0], "exit")) {     // like ^z or ^c
            printf("Good Bye\n");
            fflush(stdout);
            exit(0);
        }
        else if(!strcmp(argv[0], "echo")){
	        my_echo(argc,argv);
	        fflush(stdout);
	        continue;
        }
        else{
             printf("Invalid command\n");
             printf("Femto shell prompt >");
             fflush(stdout);
             return 1;
        }
        
    }
return 0;
}
