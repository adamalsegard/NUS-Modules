/**
*	Code for Tutorial 4
*/
#include <mpi.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[])
{

	int rank, np, size=8, tag=1;
	int left, right;
	char send_buffer1[size], recv_buffer1[size];
	char send_buffer2[size], recv_buffer2[size];
	MPI_Status Stat;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &np);
	
 
	//Exercise 1
	left = (rank-1+np)%np;
	right = (rank+1)%np;
	
	gethostname(send_buffer1, size);
	gethostname(send_buffer2, size);

	//sprintf(send_buffer1, "%d", rank);
	//sprintf(send_buffer2, "%d", rank);

	// This ordering doesn't avoid deadlock
	// Could divide into two segments with odd and even ranks (even start send, odd start recv...)
	// or we could use MPI_Sendrecv instead, or Non-blocking...
	if (rank%2 == 0){
		MPI_Send(send_buffer1, size, MPI_CHAR, left, tag, MPI_COMM_WORLD);
		MPI_Recv(recv_buffer1, size, MPI_CHAR, right, tag, MPI_COMM_WORLD, &Stat);
	
		MPI_Send(send_buffer2, size, MPI_CHAR, right, tag, MPI_COMM_WORLD);
		MPI_Recv(recv_buffer2, size, MPI_CHAR, left, tag, MPI_COMM_WORLD, &Stat);
	}
	else{
		MPI_Recv(recv_buffer1, size, MPI_CHAR, right, tag, MPI_COMM_WORLD, &Stat);
		MPI_Send(send_buffer1, size, MPI_CHAR, left, tag, MPI_COMM_WORLD);
		
		MPI_Recv(recv_buffer2, size, MPI_CHAR, left, tag, MPI_COMM_WORLD, &Stat);
		MPI_Send(send_buffer2, size, MPI_CHAR, right, tag, MPI_COMM_WORLD);
	}

	printf("My name: %s \n", send_buffer1);
	printf("Left neighbour: %s \n", recv_buffer2);
	printf("Right neighbour: %s \n", recv_buffer1);



	// Exercise 2
	int a[np];
	int x = 2;
	size = 1;
	float send_buf = 0;
	float recv_buf = 0;
	int dest = rank-1;
	int src = rank+1;

	if(rank == np-1){ // a_n-1
		a[rank] = rank;
		send_buf = a[rank]*pow(x, rank);

		MPI_Send(&send_buf, size, MPI_FLOAT, dest, tag, MPI_COMM_WORLD);

	}
	else if (rank == 0){ // root, a_0
		MPI_Recv(&recv_buf, size, MPI_FLOAT, src, tag, MPI_COMM_WORLD, &Stat);
		a[rank] = rank;	
		send_buf = a[rank]+recv_buf;
		printf("Result: P(x) =  %f \n", send_buf); 

	}
	else { // everyone else

		MPI_Recv(&recv_buf, size, MPI_FLOAT, src, tag, MPI_COMM_WORLD, &Stat);

		a[rank] = rank;
		send_buf = a[rank]*pow(x, rank) + recv_buf;

		MPI_Send(&send_buf, size, MPI_FLOAT, dest, tag, MPI_COMM_WORLD);
	}


	MPI_Finalize();

}





