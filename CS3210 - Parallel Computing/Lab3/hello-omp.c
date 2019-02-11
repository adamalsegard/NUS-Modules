#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main (int  argc,  char *argv[])
{
    int thread_id, no_threads;
    
    /* Fork slave  threads,  each with its own  unique thread id */
    #pragma  omp  parallel
    {
        /* Obtain thread id */
        thread_id =  omp_get_thread_num();
        printf("Hello World from thread  =  %d\n",  thread_id);

        /* Only master thread  does this.
        Master thread always has id  equal to  0  */
        if (thread_id ==  0)
        {
            no_threads = omp_get_num_threads();
            printf("Number  of threads =  %d\n",  no_threads);
        }
    }   /* All slave threads  join  master thread and are destroyed */
}
