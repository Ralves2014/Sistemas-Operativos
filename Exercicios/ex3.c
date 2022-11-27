#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *print_message_function( void *ptr );

int main()
{
     pthread_t thread1, thread2;
     int  x1,x2;
     int  iret1, iret2;
     

    /* Create independent threads each of which will execute function */
     x1=1; 
     iret1 = pthread_create( &thread1, NULL, print_message_function, (void *) &x1 );
     x2=2;
     iret2 = pthread_create( &thread2, NULL, print_message_function, (void *) &x2);
     
     pthread_join( thread1, NULL);
     printf("Join Thread 1 \n");
      
     pthread_join( thread2, NULL);
     printf("Join Thread 2 \n");

     exit(0);
     return 0;
}

void *print_message_function(void *input)
{
     int* z = (int*) input;
     int  w = *z;
     printf("Thread %d is printing w ...\n", w);
     printf("Thread %d is printing *(int *)input \n", *(int *)input);
     printf("The thread does  something ..... and is terminated \n ");
     
     
     pthread_exit(NULL);
     
}