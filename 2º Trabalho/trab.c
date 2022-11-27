#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "queue.c"

#define NEW -4
#define RUN 1
#define BLOCK -1
#define READY -2
#define EXIT 2
#define FREE -5
#define MAX_TIME 100
#define SIZE_LINE 10
#define THREAD 0
#define PROGRAM 1
#define UNKNOWN -8

typedef struct
{
    struct QueueRecord *Block;
    struct QueueRecord *Ready;
}QUEUE;

typedef struct
{
    int base_cod_prog;      // início do segmento com as instruções do programa
    int lim_cod_prog;       // dimensão do segmento as instruções do programa
    int base_var_prog;      // início do segmento com as variáveis do programa
    int lim_var_prog;       // dimensão do segmento as variáveis do programa
}PCB;

typedef struct 
{
    int pc;                       
    int start_time;
    int start_inst;
    int end_inst;
    int state;                      
    int id_process;
    int t_or_p;
    int indice;
    int times_run;
    int times_blocked;
    int count;
    int remove;
}TCB;

int instructions;
int mem[200];
int free_mem[200];
int CPU;
char input[MAX_TIME][SIZE_LINE];


int count_prog_and_thread(FILE *fp){

    int count=0, l=0;
    char line[SIZE_LINE];

    if (fp==NULL)
        printf("ERROR in openning file\n");

    while (fgets(line,sizeof(line),fp)!= NULL)
    {
        if (strstr(line,"LOAD") || strstr(line,"NWTH"))
            count+=1;

        strcpy(input[l],line);
        l++;
    }
    instructions=l+1;

    // Averiguar se o input é bem guardado
    /*
    for (int i = 0; i <= l; i++)
        printf("%s", input[i]);
    printf("\n");
    */
    return count;
}

void time_in_run_blocked(TCB tcb[], int number_of_prog_thread){

    for (int i = 0; i < number_of_prog_thread; i++)
    {
        int count_run=0, count_block=0, aux=0;
        for (int j = tcb[i].start_inst; j <= tcb[i].end_inst; j++)
        {
            if (tcb[i].t_or_p==THREAD && tcb[i].start_inst!=UNKNOWN && tcb[i].end_inst!=UNKNOWN)
            {
                if (strstr(input[j],"DISK")){
                    count_block+=4;
                }
                else if (strstr(input[j],"HALT"))
                    break;
                else if (strstr(input[j],"JFRW"))
                {
                    char *token;
                    char *space=" ";
                    //input[j][strcspn(input[j],"\n")]=0;
                    //token=strtok(input[j],space);
                    //token=strtok(NULL,space);
                    //j=j+atoi(token)-1;
                    count_run++;
                }
                else if (strstr(input[j],"JBCK"))
                {
                    char *token;
                    char *space=" ";
                    //input[j][strcspn(input[j],"\n")]=0;
                    //token=strtok(input[j],space);
                    //token=strtok(NULL,space);
                    //j=j-atoi(token)-1;
                    count_run++;
                }
                else if (strstr(input[j],"JIFZ"))
                {
                    //printf("%s", input[j]);   
                    char *token;
                    char *space=" ";
                    //input[j][strcspn(input[j],"\n")]=0;
                    //token=strtok(input[j],space);
                    //token=strtok(NULL,space);
                    /*
                    if (atoi(token)==0)
                        j=j+1;
                    */
                    count_run++;
                }
                else{
                    //printf("%s", input[j]);
                    count_run++;
                }
            }
            else if (tcb[i].t_or_p==PROGRAM && tcb[i].start_inst!=UNKNOWN && tcb[i].end_inst!=UNKNOWN)
            {
                if (strstr(input[j],"DISK")){
                    count_block+=4;
                }
                if (strstr(input[j],"THRD"))
                    aux=1;
                else if (strstr(input[j],"RETN") && aux)
                    aux=0;
                else if (strstr(input[j],"HALT"))
                    break;
                else if (strstr(input[j],"JFRW"))
                {
                    char *token;
                    char *space=" ";
                    //input[j][strcspn(input[j],"\n")]=0;
                    //token=strtok(input[j],space);
                    //token=strtok(NULL,space);
                    //j=j+atoi(token)-1;
                    count_run++;
                }
                else if (strstr(input[j],"JBCK"))
                {
                    char *token;
                    char *space=" ";
                    //input[j][strcspn(input[j],"\n")]=0;
                    //token=strtok(input[j],space);
                    //token=strtok(NULL,space);
                    //j=j-atoi(token)-1;
                    count_run++;
                }
                else if (strstr(input[j],"JIFZ"))
                {
                    char *token;
                    char *space=" ";
                    //input[j][strcspn(input[j],"\n")]=0;
                    //token=strtok(input[j],space);
                    //token=strtok(NULL,space);
                    /*
                    if (atoi(token)==0)
                        j=j+1;
                    */
                    count_run++;
                }
                else if (!aux)
                    count_run++;
            }
        }
        tcb[i].times_run=count_run;
        tcb[i].times_blocked=count_block;
    }
}

void read_instructions_start(TCB tcb[], int num){

    int index=0, auxp=1, auxt=1;
    char *token;
    char *space=" ";

    for (int i = 0; i < instructions; i++)
    {
        if (strstr(input[i],"LOAD"))
        {
            token=strtok(input[i],space);
            token=strtok(NULL,space);
            token[strcspn(token,"\n")]=0;
            tcb[index].start_time=atoi(token);
            tcb[index].start_inst=i+1;
            tcb[index].pc=i+1;
            tcb[index].id_process=auxp;
            tcb[index].t_or_p=PROGRAM;
            tcb[index].indice=index;
            auxp++;
            index++;
        }
        else if (strstr(input[i],"NWTH"))
        {
            token=strtok(input[i],space);
            token=strtok(NULL,space);
            token[strcspn(token,"\n")]=0;
            tcb[index].start_time=i;
            tcb[index].start_inst=UNKNOWN;
            tcb[index].id_process=auxt;
            tcb[index].t_or_p=THREAD;
            tcb[index].indice=index;
            index++;
            auxt++;
        }
        else if (strstr(input[i],"THRD"))
        {
            for (int j = 0; j < num ; j++)
            {
                if (tcb[j].t_or_p==THREAD && tcb[j].start_inst==UNKNOWN)
                {
                    tcb[j].start_inst=i+1;
                    tcb[index].pc=i+1;
                    break;
                }
            }
        }
    }
}

void read_instructions_end(TCB tcb[], int num){

    for (int j = 0; j < instructions; j++)
    {
        for (int i = 0; i < num; i++)
        {
            if (strstr(input[j],"ENDP") && tcb[i].t_or_p==PROGRAM)
            {
                tcb[i].end_inst=j-1;
                break;
            }
            else if (strstr(input[j],"RETN") && tcb[i].t_or_p==THREAD)
            {
                tcb[i].end_inst=j-1;
                break;
            }
            
            if (tcb[i].t_or_p==THREAD && tcb[i].start_inst==UNKNOWN)
            {
                tcb[i].end_inst=UNKNOWN;
                break;
            }
        }
    }    
}

int memory_free(int mem[], TCB tcb, PCB pcb){

        
}

void NewProcess(int time, int number_of_prog_thread, TCB tcb[], Queue R){

    for (int i = 0; i < number_of_prog_thread; i++)
    {
        if (tcb[i].t_or_p==PROGRAM && tcb[i].start_time==time)
            tcb[i].state=NEW;
        
        else if (tcb[i].t_or_p==THREAD && tcb[i].start_time==time){
            tcb[i].state=READY;

            if (tcb[i].times_run!=0 || tcb[i].times_blocked!=0){
                Enqueue(tcb[i].indice,R);
            }
        }
    }
}

void New2ready(int number_of_prog_thread, TCB tcb[], Queue R){

    for (int i = 0; i < number_of_prog_thread; i++)
    {
        if (tcb[i].t_or_p==PROGRAM && tcb[i].state==NEW)
        {
            tcb[i].state=READY;
            Enqueue(tcb[i].indice,R);
        } 
    }
}

void Ready2run(int number_of_prog_thread, TCB tcb[], Queue R){

    if (!IsEmpty(R) && CPU==FREE)
    {
        for (int i = 0; i < number_of_prog_thread; i++)
        {
            if (tcb[i].indice==Front(R))
            {
                CPU=Front(R);
                tcb[i].state=RUN;
            }   
        }
    }
}

void Blocked2ready(int Q, int number_of_prog_thread,TCB tcb[], Queue R, Queue B){

    for (int i = 0; i < number_of_prog_thread; i++)
    {
        if (tcb[i].state==BLOCK && tcb[i].count==Q && tcb[i].times_run>0)
        {
            Dequeue(B);
            Enqueue(tcb[i].indice,R);
           
            tcb[i].count=0;
            tcb[i].state=READY;
        }
        else if (tcb[i].state==BLOCK && tcb[i].times_blocked==0 && tcb[i].times_run>0)
        {
            Dequeue(B);
            Enqueue(tcb[i].indice,R);
            
            tcb[i].count=0;
            tcb[i].state=READY;
        }
    }
}

void Remove_others(int number_of_prog_thread, TCB tcb[]){

    int aux=0;
    for (int i = 0; i < number_of_prog_thread; i++)
    {
        if (tcb[i].t_or_p==PROGRAM && tcb[i].state==EXIT && tcb[i].times_run==0 && tcb[i].times_blocked==0 && aux==0)
            aux=1;

        else if (tcb[i].t_or_p!=PROGRAM && tcb[i].state!=EXIT && aux==1)
            tcb[i].state=EXIT;
        
        else if (tcb[i].t_or_p==PROGRAM && aux==1)
            break;
    }
}

void Run2exit_blocked_run(int Q, int number_of_prog_thread,TCB tcb[], Queue R, Queue B){

    for (int i = 0; i < number_of_prog_thread; i++)
    {
        if (tcb[i].times_run>=Q)
        {
            if (tcb[i].state==RUN && tcb[i].times_blocked>0 && tcb[i].count==Q)
            {
                // block
                CPU=FREE;
                Dequeue(R);
                Enqueue(tcb[i].indice,B);
                tcb[i].count=0;
                tcb[i].state=BLOCK;
            }
            else if (tcb[i].state==RUN && tcb[i].times_blocked==0 && tcb[i].count==Q)
            {
                // ready
                CPU=FREE;
                tcb[i].count=0;
                Dequeue(R);
                Enqueue(tcb[i].indice,R);

                if (Front(R)==tcb[i].indice)
                    tcb[i].state=RUN;
                else
                    tcb[i].state=READY;
            }
        }
        else
        {
            if (tcb[i].state==RUN && tcb[i].times_run==0 && tcb[i].times_blocked>0)
            {
                // block
                CPU=FREE;
                Dequeue(R);
                Enqueue(tcb[i].indice,B);
                tcb[i].count=0;
                tcb[i].state=BLOCK;
            }
            else if (tcb[i].state==RUN && tcb[i].times_run==0 && tcb[i].times_blocked==0)
            {
                // exit
                CPU=FREE;
                Dequeue(R);
                tcb[i].count=0;
                tcb[i].state=EXIT;
            }
        }
    }
}

void Execute(int number_of_prog_thread,TCB tcb[]){

    for (int i = 0; i < number_of_prog_thread; i++)
    {
        if (CPU==tcb[i].indice && tcb[i].state==RUN){
            tcb[i].count++;
            tcb[i].times_run--;
        }
    }
}

void Execute_blocked(int number_of_prog_thread,TCB tcb[], Queue B){

    for (int i = 0; i < number_of_prog_thread; i++)
    {
        if (tcb[i].state==BLOCK){
            tcb[i].count++;
            tcb[i].times_blocked--;
        }
    }
}

void RemoveProcess(int pos, TCB tcb[]){
    tcb[pos].remove=1;
}

void print_NEW(int number_of_prog_thread, TCB tcb[]){

    int count=0;
    for (int i = 0; i < number_of_prog_thread; i++)
    {
        if (tcb[i].state==NEW)
            count++;    
    }

    if (count==0)
        printf("      |");
    else
    {
        int aux=0;
        for (int i = number_of_prog_thread; i >=0; i--)
        {
            if (tcb[i].t_or_p==PROGRAM && tcb[i].state==NEW){
                aux++;
                printf("P%d ", tcb[i].id_process);
            }
            if (aux==count){

                if (count==1)
                    printf("   |");
                else if (count==2)
                    printf("|");
            }
        }
    }
}

void print_READY(int number_of_prog_thread, TCB tcb[]){

    int count=0;
    for (int i = 0; i < number_of_prog_thread; i++)
    {
        if (tcb[i].state==READY)
            count++;    
    }

    if (count==0)
        printf("                    |");
    else{

        int aux=0;
        for (int i = number_of_prog_thread; i >=0; i--)
        {
            if (tcb[i].t_or_p==PROGRAM && tcb[i].state==READY){
                aux++;
                printf(" P%d  ", tcb[i].id_process);
            }    
            else if (tcb[i].t_or_p==THREAD && tcb[i].state==READY){
                aux++;
                printf(" TH%d ", tcb[i].id_process);
            }
            if (aux==count){

                if (count==1)
                    printf("               |");
                else if (count==2)
                    printf("          |");
                else if (count==3)
                    printf("       |");
                
                break;
            }        
        }
    }
}

void print_RUN(int number_of_prog_thread, TCB tcb[]){

    int count=0;
    for (int i = 0; i < number_of_prog_thread; i++)
    {
        if (tcb[i].state==RUN)
            count++;    
    }

    if (count==0)
        printf("      |");
    else
    {
        int aux=0;
        for (int i = number_of_prog_thread; i >=0; i--)
        {
            if (tcb[i].t_or_p==PROGRAM && tcb[i].state==RUN){
                printf("  P%d", tcb[i].id_process);
                aux++;
            }
            else if (tcb[i].t_or_p==THREAD && tcb[i].state==RUN){
                printf(" TH%d", tcb[i].id_process);
                aux++;
            }
            
            if (aux==count){
                printf("  |");
                break;
            }
        }
    } 
}

void print_BLOCKED(int number_of_prog_thread, TCB tcb[]){

    int count=0;
    for (int i = 0; i < number_of_prog_thread; i++)
    {
        if (tcb[i].state==BLOCK)
            count++;    
    }

    if (!count)
    {
        printf("               |");
    }
    else{
        int aux=0;
        for (int i = number_of_prog_thread; i >=0; i--)
        {
            if (tcb[i].t_or_p==PROGRAM && tcb[i].state==BLOCK){
                printf("  P%d ", tcb[i].id_process);
                aux++;
            }
            else if (tcb[i].t_or_p==THREAD && tcb[i].state==BLOCK){
                printf("TH%d  ", tcb[i].id_process);
                aux;
            }
                
            if (aux==count){

                if (count==1)
                    printf("          |");
                else if (count==2)
                    printf("          |");
            }        
        }
        
    }

}

void print_EXIT(int number_of_prog_thread, TCB tcb[]){

    int count=0;
    for (int i = 0; i < number_of_prog_thread; i++)
    {
        if (tcb[i].state==EXIT && tcb[i].remove==0)
            count++;    
    }

    if (count==0)
    {
        printf("         |");
    }
    else{

        int aux=0;
        for (int i = number_of_prog_thread; i >=0; i--)
        {
            if (tcb[i].t_or_p==PROGRAM && tcb[i].state==EXIT && tcb[i].remove==0){
                aux++;
                printf(" P%d ", tcb[i].id_process);
                RemoveProcess(i,tcb);
            }
            else if (tcb[i].t_or_p==THREAD && tcb[i].state==EXIT && tcb[i].remove==0){
                aux++;
                printf(" TH%d", tcb[i].id_process);
                RemoveProcess(i,tcb);
            }
        }
        if (aux==count){

            if (count==1)
                printf("     |");
            else if (count==2)
                printf(" |");
        }        
    }
}

void line_time_RR(int Q, int time, int number_of_prog_thread, TCB tcb[], Queue B, Queue R){

    // Transições de estados
    Blocked2ready(Q,number_of_prog_thread,tcb,R,B);           // ----
    Run2exit_blocked_run(Q,number_of_prog_thread,tcb,R,B);
    Remove_others(number_of_prog_thread,tcb);
    New2ready(number_of_prog_thread,tcb,R);
    Ready2run(number_of_prog_thread,tcb,R);                      
    NewProcess(time,number_of_prog_thread,tcb,R);
    Execute(number_of_prog_thread,tcb);                     
    Execute_blocked(number_of_prog_thread,tcb,B);
    
    /*
    for (int j = 0; j < number_of_prog_thread; j++){
        printf("indice: %d state: %d    \n",tcb[j].indice,tcb[j].state);
        //printf("indice: %d count: %d    \n",tcb[j].indice,tcb[j].count);
    }
    */

    // Output 
    print_NEW(number_of_prog_thread,tcb);
    print_READY(number_of_prog_thread,tcb);
    print_RUN(number_of_prog_thread,tcb);
    print_BLOCKED(number_of_prog_thread,tcb);
    print_EXIT(number_of_prog_thread,tcb);
}

int all_exit(int number_of_prog_thread, TCB tcb[]){

    int value=1;
    for (int i = 0; i < number_of_prog_thread; i++)
    {
        if (tcb[i].state!=EXIT)
            value=0;
    }

    return value;
}

int main(){
    
    int time=0, number_of_prog_thread;
    int Q = 3;
    CPU=FREE;

    FILE *pointer = fopen("exemplo4.txt", "r");
    number_of_prog_thread = count_prog_and_thread(pointer);
    //printf("num de proc: %d\n",number_of_prog_thread);
    
    PCB pcb[number_of_prog_thread];
    TCB tcb[number_of_prog_thread];
    Queue B = CreateQueue(50);
    Queue R = CreateQueue(50);

    for (int j = 0; j < 200; j++)
        free_mem[j]=0;
    
    read_instructions_start(tcb,number_of_prog_thread);
    read_instructions_end(tcb,number_of_prog_thread);
    time_in_run_blocked(tcb,number_of_prog_thread);

    for (int t = 0; t < number_of_prog_thread; t++){
        tcb[t].count=0;
        tcb[t].remove=0;
    }

    printf("\n");
    // Testar valores
    /*
    for (int i = 0; i < number_of_prog_thread; i++){
        if (tcb[i].t_or_p==PROGRAM){
            printf("P%d : %d %d\n",tcb[i].id_process,tcb[i].start_inst,tcb[i].end_inst);
            printf("P%d time start: %d\n",tcb[i].id_process,tcb[i].start_time);
            printf("P%d times run: %d\n",tcb[i].id_process,tcb[i].times_run);
            printf("P%d times blocked: %d\n",tcb[i].id_process,tcb[i].times_blocked);
        }
        else if (tcb[i].t_or_p==THREAD){
            printf("TH%d : %d %d\n",tcb[i].id_process,tcb[i].start_inst,tcb[i].end_inst);
            printf("TH%d time start: %d\n",tcb[i].id_process,tcb[i].start_time);
            printf("TH%d times run: %d\n",tcb[i].id_process,tcb[i].times_run);
            printf("TH%d times blocked: %d\n",tcb[i].id_process,tcb[i].times_blocked);
        }
    }
    */

    fclose(pointer);

    //int verify_mem = memory_free(free_mem,tcb,);
    //printf("mem free:%d\n", verify_mem);
    

    printf("\n");
    printf("                              SIMULADOR DE ESCALONAMENTO Round Robin Q=%d                                            \n", Q);
    printf("_____________________________________________________________________________________________________________________\n");
    printf("\n");


    printf("T   NEW   | READY              | RUN  | BLOCKED       | EXIT    |\n");

    while (!all_exit(number_of_prog_thread,tcb) && time<=100)
    {
        if (time<9)
        {
            printf("%d   ",time+1);
            line_time_RR(Q,time,number_of_prog_thread,tcb, B,R);
            printf("\n");
        }
        else
        {
            printf("%d  ",time+1);
            line_time_RR(Q,time,number_of_prog_thread,tcb, B,R);
            printf("\n");
        }
        time++;
    }
    
    return 0;
}