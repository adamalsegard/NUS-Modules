/**
 * CS3210 - The program outputs the location of each process 
 * that is started together with the processor affinity mask.
 **/

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define _GNU_SOURCE
#define __USE_GNU
#include <sched.h>


int main(int argc, char ** argv)
{
	MPI_Status mpi_status; 
	int nprocs;
	int my_id;
	char hostname[256];
	int affinity; 
	int sc_status;
	pid_t my_pid;
	cpu_set_t my_set;
	unsigned short mask;
	
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
	
	memset(hostname, 0, sizeof(hostname));
	sc_status = gethostname(hostname, sizeof(hostname)-1);
	if (sc_status)
	{
		perror("gethostname");
		return sc_status;
	}
		
	memset(&my_set, 0, sizeof(my_set));
	CPU_ZERO(&my_set);
	sc_status = sched_getaffinity(0, sizeof(my_set), &my_set);
	if (sc_status)
	{
		perror("sched_getaffinity");
		return sc_status;
	}
	
	mask = 0;
	
	if (CPU_ISSET(0, &my_set)) mask |= 0x0001;
	if (CPU_ISSET(1, &my_set)) mask |= 0x0002;
	if (CPU_ISSET(2, &my_set)) mask |= 0x0004;
	if (CPU_ISSET(3, &my_set)) mask |= 0x0008;
	if (CPU_ISSET(4, &my_set)) mask |= 0x0010;
	if (CPU_ISSET(5, &my_set)) mask |= 0x0020;
	if (CPU_ISSET(6, &my_set)) mask |= 0x0040;
	if (CPU_ISSET(7, &my_set)) mask |= 0x0080;
	if (CPU_ISSET(8, &my_set)) mask |= 0x0100;
	if (CPU_ISSET(9, &my_set)) mask |= 0x0200;
	if (CPU_ISSET(10, &my_set)) mask |= 0x0400;
	if (CPU_ISSET(11, &my_set)) mask |= 0x0800;
	if (CPU_ISSET(12, &my_set)) mask |= 0x1000;
	if (CPU_ISSET(13, &my_set)) mask |= 0x2000;
	if (CPU_ISSET(14, &my_set)) mask |= 0x4000;
	if (CPU_ISSET(15, &my_set)) mask |= 0x8000;
	 
	printf("Process %d is on hostname %s on processor mask 0x%x\n", 
            my_id, hostname, mask);
	
	MPI_Finalize();
	return 0;
}

