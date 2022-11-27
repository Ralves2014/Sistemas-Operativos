#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

void forkTest() {
    puts("TESTE FORK:\n");
    
    pid_t pid;
    
    pid = fork(); //cria processo
    
    if(pid > 0) {
        printf(" Processo Pai PID: %d \n", pid);        // vai sendo alterado cada vez que o programa é corrido
    }
    else {
        printf(" Processo Filho PID: %d \n", pid);
    }
    puts("FIM\n");      // aperece duas vezes cada vez que o programa é corrido
}

void main()
{
    forkTest();
    
}
