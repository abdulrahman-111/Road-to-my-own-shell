#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// we wil take the pathname for the program we want to execute
// using this program 
int main (int argc, char ** argv ){
		if (argc<2){
				printf(" Usage: %s path to elf \n",argv[0]);
				exit(-1);
		}
		char * newargv[] = { argv[1],NULL };
		char * newenvp[] = { NULL }; // we didnot pass any env varibles to the process
		execve (argv[1],newargv,newenvp);
		// retrun to this lines means there is an error 
		printf("ERORRROEasIHFK ");
		
return 0;
}