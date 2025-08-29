#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main (){
        char path[10000];
        getcwd(path,10000);
        strncat(path,"\n ",1);
        if( write (1, path,strlen(path)) <0) {
        exit(-1);
        }
        return 0;
}