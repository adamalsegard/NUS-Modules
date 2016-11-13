/**
 * CS3210 - Blocking communication in MPI.
 */

#include <mpi.h>
#include <stdio.h>

int main(int argc,char *argv[])
{
	int numtasks, rank, dest, source, rc, count, tag=1;  
	char inmsg1, outmsg1='x';
	float inmsg2[10] = {0}; 
	float outmsg2[10] = {11, 21, 3, 4, 5, 6, 7, 8, 9, 10};
	MPI_Status Stat;

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0){
		dest = 1;
		source = 1;
		
		rc = MPI_Send(&outmsg1, 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);	
		rc = MPI_Recv(inmsg2, 10, MPI_FLOAT, source, tag, MPI_COMM_WORLD, &Stat);	
		rc = MPI_Get_count(&Stat, MPI_FLOAT, &count);
		
		printf("Recieved floats: ");
		for(int i=0; i<10; i++){
			printf("%f, ", inmsg2[i]);
		}
		printf("\n");
		

	} 	
	else if (rank == 1){
		dest = 0;
		source = 0;
		rc = MPI_Recv(&inmsg1, 1, MPI_CHAR, source, tag, MPI_COMM_WORLD, &Stat);
		rc = MPI_Send(outmsg2, 10, MPI_FLOAT, dest, tag, MPI_COMM_WORLD);
		rc = MPI_Get_count(&Stat, MPI_CHAR, &count);
	}
	

	printf("Task %d: Received %d char(s) from task %d with tag %d \n", rank, count, Stat.MPI_SOURCE, Stat.MPI_TAG);

	MPI_Finalize();
	
	return 0;
}

