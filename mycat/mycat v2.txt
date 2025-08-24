#include <stdio.h>
#include<stdlib.h>
#include <fcntl.h>
#include <string.h>
 #include <unistd.h>


int main (int argc, char* argv[]){

        for (int x=1 ; x < argc; x++){
                char*path =argv[x];
                printf ("we used this path %s \n ",path);
                int fd = open (path , O_RDONLY );
                if (fd<0){
                        exit(-1);
                }
                int count = 1000;
                char  str[count];
                while(! (read (fd,str,count)<=0)){
                        int size = strlen(str);
                        if (write (1,str,size)<0  ){
                                exit(-2);
                        }
                }
                close (fd);

        }

        printf("hi man , we do it \n");

        return 0;
}