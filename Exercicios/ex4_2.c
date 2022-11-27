#include <stdio.h>
#include <string.h>

#define NUM_STRINGS 10
#define MAX_TIME 100

int max_time(int input[][8],int sz1, int sz2){

    int max=0;
    for (int  program = 0; program < sz1; program++)
    {
        for (int y = 0; y < sz2; y++)       
            max+=input[program][y];
    }
    return max;   
}

int each_process_time(int input[][8],int program,int sz2){

    int process_time=0;

    for (int  t = 0; t < sz2; t++)
        process_time+=input[program][t];       
    
    return process_time;

}

void output(int input[][8], int sz1, int sz2, int tmax){       

    
    char *arr[MAX_TIME][NUM_STRINGS];

    // Modelo de 2 estados
    for (int process_id = 0; process_id < sz1; process_id++)
    {
        int start_time=0;
        int end_time=0;
        
        if (process_id!=0)
        {
            for (int prog=0; prog < process_id; prog++)    
                start_time+=each_process_time(input,prog,sz2); 
        }
        //printf("start_time:%d\n",start_time);  
        
        for (int prog=0; prog <= process_id; prog++)    
            end_time+=each_process_time(input,prog,sz2);
        //printf("end_time:%d\n",soma_t);

        for (int t = 0; t < tmax; t++)
        {
            if (t>=start_time && t<end_time)
                arr[t][process_id]="RUN";
            else if (t<start_time)
                arr[t][process_id]="READY";
            else
                arr[t][process_id]="   ";
        
        }
    }
    
    
    printf("instante ");
    for (int proc_num = 0; proc_num < sz1; proc_num++)
    {
        printf("|  proc%d  ",proc_num+1);            
    }
    
    
    printf("\n");
    
    for (int time = 0; time < tmax; time++)
    {
        
        printf("%d      ", time+1); 
        for (int process = 0; process < sz1; process++)
        {
            printf("      %s",arr[time][process]);
        }
        printf("\n");
    }
    
}

int main(){
    
    int max_t;
    
    int programas[3][8] = {
        {3, 1, 2, 2, 0, 0, 0, 0} ,
        {4, 2, 4, 0, 0, 0, 0, 0} ,
        {2, 1, 5, 0, 0, 0, 0, 0} };

    
    
    max_t=max_time(programas,3,8);
    
    output(programas,3,8,max_t);

    return 0;
}