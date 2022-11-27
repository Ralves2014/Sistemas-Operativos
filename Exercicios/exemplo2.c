#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

void main(void){

    pid_t x, y;
    
    x=fork();
    y=fork();

    if ((x+y) == 0) {
        printf(" B ");
    }else{
        printf(" A ");
    }

    /*
        A  A  A  B  B
    */

    
    /*  testes

    x=fork();
    y=fork();

    if ((x+y) == 0) {
        fork();
        printf(" B ");
    }else{
        printf(" A ");
    }
    

    
    A  A  A  B
    */

    
}