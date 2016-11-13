/* C Example */
#include <stdio.h>
#include <mpi.h>

int main (int argc, char *argv[])
{
        int rank, size;
        char msg[80];
        int length;

        /* starts MPI */
        MPI_Init (&argc, &argv);

        /* get current process id */
        MPI_Comm_rank (MPI_COMM_WORLD, &rank);

        /* get number of processes */
        MPI_Comm_size (MPI_COMM_WORLD, &size);

        /* Gets the name of the processor (computer name) */
        MPI_Get_processor_name(msg, &length);

        printf( "Hello world from process %d of %d running on %s \n", 
            rank, size, msg);

        /* exiting */
        MPI_Finalize();

        return 0;
}
