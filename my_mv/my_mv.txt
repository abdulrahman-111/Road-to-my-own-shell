#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
int main (int argc, char **argv){
	if(argc<3){
		printf("no enough argumens\n");
	}
	int fd1 = open (argv[1] , O_RDONLY );
	if(fd1<0){
	exit(-1);
	}
	int fd2 = open (argv[2], O_WRONLY|O_CREAT|O_TRUNC, 0644 );
		if(fd2<0){
	exit(-2);
	}
  int count = 1000;
  char  str[count];
  long long int rea;
   while((rea=read (fd1,str,count))>0){
       
       if (write (fd2 ,str,rea)!=rea  ){
              exit(-2);
      
   }}
   close (fd1);
   close (fd2);
    if (unlink(argv[1]) < 0) {
        perror("Error deleting source file");
        return 1;
    }
  return 0 ;
}