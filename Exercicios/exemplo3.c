#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

void main(void){
    pid_t f;
    int i;

    for (i = 1; i <= 2; i++){
        f = fork();
        if (f == 0) {printf("\n A ");} else {printf("\n B ");}
        printf("\n C");
    }
}