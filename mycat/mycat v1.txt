#include <stdio.h>
#include<stdlib.h>
#include <fcntl.h>
#include <string.h>
 #include <unistd.h>


int main (){
        int fd = open ("/home/abdo/Desktop/test.txt" , O_RDONLY );

        if (fd<0){
                exit(-1);
        }
        int count = 1000;
        char  str[count];

        read (fd,str,count);
        int size = strlen(str);
        if (write (1,str,size)<0  ){
                exit(-2);
        }
        printf("hi man , we do it ");
        close(fd);
        return 0;
}