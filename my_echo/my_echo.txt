#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main (int argc, char * argv[]){
        for (int x=1; x <argc; x++ ){
        if (write(1,argv[x],strlen(argv[x]) )<0 ){
        exit (0);
        }
         if (x < argc - 1)
          write(1, " ", 1);
        }
        printf ("\n");

    return 0;
}