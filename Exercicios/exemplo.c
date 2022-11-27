#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

void main(void){
    pid_t f;

    f=fork();
    if (f == 0)
    {
        fork();
        printf(" B ");
    } else
    {
        printf(" A ");
    }

    /*
        A  B  B
    */
}