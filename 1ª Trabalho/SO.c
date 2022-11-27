#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "queue.c"

typedef struct
{
    struct QueueRecord *BLCK;
    struct QueueRecord *READY;
}SO;

typedef struct
{
    int state;
    int position;
    int start;
    int all_times[9];
    int CPU;
    int contador;

}PROCESS;

#define RUN 1
#define BLCK -1
#define READY -2
#define EXIT 2

void all_times_each_process(int input[][10], int num_of_proc, PROCESS *p){

    int proc=0;
    while (proc<num_of_proc)
    {
        int pos=1;
        while (input[proc][pos]!=0)
        {
            p[proc].all_times[pos-1]=input[proc][pos];
            pos++;
        }
        for (int i = pos-1; i < 9; i++)
            p[proc].all_times[i]=-1;

        proc++;
    }
    
}

void start_process(int input[][10], int sz1, PROCESS *p){

    for (int proc = 0; proc < sz1; proc++)
        p[proc].start=input[proc][0];         
}

int STATE_NEW(int time, int num_proc, PROCESS *p){
    
    if (p[num_proc].start==time)
        return 1;
    else
        return 0;
}

int READY_TO_RUN(Queue Ready, PROCESS *p){

    if (p->CPU == -5)
        return 1;
    else
        return 0;
}

int ALL_EXIT(PROCESS *p, int number_of_process){

    int value=1;

    for (int proc = 0; proc < number_of_process; ++proc)
    {
        if (p[proc].state!=EXIT)
            value=0;
    }

    return value;

}

char *line_time_RR(int time, int number_of_process, Queue Block, Queue Ready, PROCESS *p , int Q){

    char *line=malloc(sizeof(char)*100);

    for (int proc = 0; proc < number_of_process; proc++)
    {
        if (STATE_NEW(time,proc,p)){
            strcat(line,"NEW     ");
            p[proc].state=READY;
            Enqueue(proc,Ready);
        }
        else if(time!=p[proc].start && p[proc].start>time){
            strcat(line,"          ");
        }
        else{

            if (p[proc].contador<Q)
            {
                
                if (p[proc].all_times[p[proc].position]==0)
                {
                    
                    if (p[proc].position%2==0)
                    {
                        Dequeue(Ready);
                        Enqueue(proc,Block);
                        p[proc].state=BLCK;
                        p->CPU=-5;
                    }
                    else
                    {
                        Dequeue(Block);
                        Enqueue(proc,Ready);
                        p[proc].state=READY;
                    }
                    p[proc].position+=1;
                    p[proc].contador=0;
                }
                
                
                if (p[proc].all_times[p[proc].position]!=-1)
                {
                    if (IsEmpty(Ready) )
                    {
                        strcat(line,"BLCK      ");
                        p[proc].state==BLCK;
                        p[proc].all_times[p[proc].position]-=1;
                    }
                    else if (READY_TO_RUN(Ready,p) && Front(Ready)==proc)
                    {
                        p->CPU=proc;    
                        strcat(line,"RUN       ");
                        p[proc].state=RUN;
                        p[proc].all_times[p[proc].position]-=1;
                        p[proc].contador+=1;
                    }
                    else if (p->CPU==proc)
                    {
                        strcat(line,"RUN       ");
                        p[proc].state=RUN;
                        p[proc].all_times[p[proc].position]-=1;
                        p[proc].contador+=1;
                    }
                    else if ((p[proc].state==READY && p->CPU!=proc) || (p[proc].state==BLCK && Front(Block)!=proc))
                    {
                        strcat(line,"READY     ");
                    }
                    else if (Front(Block)==proc)
                    {
                        strcat(line,"BLCK      ");
                        p[proc].state==BLCK;
                        p[proc].all_times[p[proc].position]-=1;
                        p[proc].contador+=1;
                    }
                    
                }
                else if (p[proc].state==EXIT)
                {
                    strcat(line,"          ");
                }
                else if (p[proc].state!=EXIT)
                {
                    p[proc].state=EXIT;
                    strcat(line,"EXIT      ");
                }

                if (p[proc].contador==Q && p[proc].all_times[p[proc].position]==0)
                {
                    p[proc].contador=0;
                }

            }
            else
            {
                if (p[proc].state=RUN)
                {
                    p[proc].contador=0;
                    p[proc].state=READY;
                    p->CPU=-5;  
                    Dequeue(Ready);
                    Enqueue(proc,Ready);
                    strcat(line,"READY     ");
                }
                else
                {
                    p[proc].contador=0;
                    p[proc].state=BLCK;  
                    Dequeue(Block);
                    Enqueue(proc,Block);
                    strcat(line,"READY     ");
                }
                
            }
            
            
            
        }

    }
    
    return line;
}

char *line_time_FCFS(int time, int number_of_process, Queue Block, Queue Ready, PROCESS *p){

    char *line=malloc(sizeof(char)*100);

    for (int proc = 0; proc < number_of_process; proc++)
    {
        if (STATE_NEW(time,proc,p)){
            strcat(line,"NEW     ");
            p[proc].state=READY;
            Enqueue(proc,Ready);
        }
        else if(time!=p[proc].start && p[proc].start>time){
            strcat(line,"             ");
        }
        else{

            if (p[proc].all_times[p[proc].position]==0)
            {
                
                if (p[proc].position%2==0)
                {
                    Dequeue(Ready);
                    Enqueue(proc,Block);
                    p[proc].state=BLCK;
                    p->CPU=-5;
                }
                else
                {
                    Dequeue(Block);
                    Enqueue(proc,Ready);
                    p[proc].state=READY;
                }
                p[proc].position+=1;
            }

            if (p[proc].all_times[p[proc].position]!=-1)
            {
                if (IsEmpty(Ready) )
                {
                    strcat(line,"BLCK      ");
                    p[proc].state==BLCK;
                    p[proc].all_times[p[proc].position]-=1;
                }
                else if (READY_TO_RUN(Ready,p) && Front(Ready)==proc)
                {
                    p->CPU=proc;    
                    strcat(line,"RUN       ");
                    p[proc].state=RUN;
                    p[proc].all_times[p[proc].position]-=1;
                }
                else if (p->CPU==proc)
                {
                    strcat(line,"RUN       ");
                    p[proc].state=RUN;
                    p[proc].all_times[p[proc].position]-=1;
                }
                else if ((p[proc].state==READY && p->CPU!=proc) || (p[proc].state==BLCK && Front(Block)!=proc))
                {
                    strcat(line,"READY     ");
                }
                else if (Front(Block)==proc)
                {
                    strcat(line,"BLCK      ");
                    p[proc].state==BLCK;
                    p[proc].all_times[p[proc].position]-=1;
                }
                
            }
            else if (p[proc].state==EXIT)
            {
                strcat(line,"          ");
            }
            else if (p[proc].state!=EXIT)
            {
                p[proc].state=EXIT;
                strcat(line,"EXIT      ");
            }
            
        }

    }
    
    return line;
}

int main(){

    int programas[3][10] = {
        {1, 4, 2, 2, 2, 4, 0, 0, 0, 0 } ,
        {0, 3, 3, 4, 1, 1, 0, 0, 0, 0 } ,
        {6, 2, 1, 6, 1, 3, 1, 1, 0, 0 } };
    
    int time=1;
    int number_of_process=(int)(sizeof(programas) / sizeof(programas[0]));
    //printf("%d\n",process_num);

    PROCESS* p=malloc(number_of_process*sizeof*p);
    p->CPU=-5;     // indica que nenhum processo esta a ser corrido

    Queue B = CreateQueue(number_of_process);
    Queue R = CreateQueue(number_of_process);

    start_process(programas,number_of_process,p);
    all_times_each_process(programas,number_of_process,p);                       

    for (int i = 0; i < number_of_process; i++)
        p[i].position=0;
    
    printf("\n");
    printf("                                          SIMULADOR DE ESCALONAMENTO FCFS                                            \n");
    printf("_____________________________________________________________________________________________________________________\n");
    printf("\n");
    
    printf("instante ");
    for (int proc_num = 1; proc_num <= number_of_process; proc_num++)
        printf("|  proc%d  ",proc_num);            

    printf("\n");

    while (!ALL_EXIT(p,number_of_process))         //time!=37 
    {
        printf("%d",time);
        if (time<10)
        {
            printf("            %s",line_time_FCFS(time-1,number_of_process,B,R,p));
        }
        else
            printf("           %s",line_time_FCFS(time-1,number_of_process,B,R,p));
        
        printf("\n");
        
        time++;    
    }


    int Q=3;
    time=1;
    
    PROCESS* p2=malloc(number_of_process*sizeof*p2);
    p2->CPU=-5;     // indica que nenhum processo esta a ser corrido

    MakeEmpty(B);
    MakeEmpty(R);
    
    start_process(programas,number_of_process,p2);
    all_times_each_process(programas,number_of_process,p2);                       

    for (int i = 0; i < number_of_process; i++){
        p2[i].position=0;
        p2[i].contador=0;
    }
        
    printf("\n");
    printf("                              SIMULADOR DE ESCALONAMENTO Round Robin Q=%d                                            \n", Q);
    printf("_____________________________________________________________________________________________________________________\n");
    printf("\n");

    printf("instante ");
    for (int proc_num = 1; proc_num <= number_of_process; proc_num++)
        printf("|  proc%d  ",proc_num);            

    printf("\n");

    while (!ALL_EXIT(p2,number_of_process))
    {
        printf("%d",time);

        if (time<10)
        {
            printf("            %s",line_time_RR(time-1,number_of_process,B,R,p2,Q));
        }
        else
            printf("           %s",line_time_RR(time-1,number_of_process,B,R,p2,Q));
        
        printf("\n");
            
        time++;    
    }
    

    return 0;
}