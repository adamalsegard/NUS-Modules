/**
 * CS3210 - Hello World in MPI
 **/

#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv)
{
	int rank, size;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	/* From here on, each process is free to execute its own code */

	printf("Hello world from process %d out of %d\n", rank, size);

	MPI_Finalize();

	return 0;
}
