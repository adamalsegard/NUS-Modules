/** 
 * 
 * Matrix Multiplication - CUDA for GPUs
 *
 * CS3210
 *
 * Modified by Adam Alsegard, A0153097B, Assignment 1
 * Optimization v.2 - best version!
 * 
 **/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <assert.h>

int size;
// Use block size 32 because of warp size (32), maximum threads per block: 1024 (32x32)
#define BLOCK_SIZE 32

typedef struct
{
	float ** element;
} matrix;


long long wall_clock_time()
{
#ifdef __linux__
	struct timespec tp;
	clock_gettime(CLOCK_REALTIME, &tp);
	return (long long)(tp.tv_nsec + (long long)tp.tv_sec * 1000000000ll);
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (long long)(tv.tv_usec * 1000 + (long long)tv.tv_sec * 1000000000ll);
#endif
}

/**
 * Allocates memory for a matrix of size SIZE
 * The memory is allocated row-major order, i.e. 
 *  elements from the same row are allocated at contiguous 
 *  memory addresses.
 **/
__host__ void allocate_matrix(matrix* m)
{
	int i;
	cudaError_t rc;
	
	// allocate array for all the rows
	rc = cudaMallocManaged((void**)&(m->element), sizeof(float*) * size);
	if (rc != cudaSuccess)
	{
		fprintf(stderr, "CUDA error: %s\n", cudaGetErrorString(rc));
		exit(1);
	}
	
	// allocate an array for each row of the matrix
	for (i = 0; i < size; i++)
	{
		rc = cudaMallocManaged((void**)&(m->element[i]), sizeof(float) * size);
		if (rc != cudaSuccess)
		{
			fprintf(stderr, "CUDA error: %s\n", cudaGetErrorString(rc));
			exit(1);
		}
	}
}

/**
 * Free the memory allocated for a matrix.
 **/
__host__ void free_matrix(matrix* m) {
	int i;
	for (i = 0; i < size; i++)
		cudaFree(m->element[i]);
	cudaFree(m->element);
}

/**
 * Initializes the elements of the matrix with
 * random values between 0 and 9
 **/
__host__ void init_matrix(matrix m)
{
	int i, j;
	
	for (i = 0; i < size; i++)
		for (j = 0; j < size; j++)
		{
			m.element[i][j] = rand() % 10;
		}
}

/**
 * Initializes the elements of the matrix with
 * element 0.
 **/
__host__ void init_matrix_zero(matrix m)
{
	int i, j;
	
	for (i = 0; i < size; i++)
		for (j = 0; j < size; j++)
		{
			m.element[i][j] = 0.0;
		}
}


/**
 * Multiplies matrix @a with matrix @b storing
 * the result in matrix @result
 * 
 * The multiplication algorithm is the O(n^3) 
 * algorithm
 */
void mm(matrix a, matrix b, matrix result)
{
	int i, j, k;
	
	// Do the multiplication
	for (i = 0; i < size; i++)
		for (j = 0; j < size; j++)
			for(k = 0; k < size; k++)
				result.element[i][j] += a.element[i][k] * b.element[k][j];
}

/**
 * Each kernel computes the result element (i,j).
 */
__global__ void mm_kernel(matrix a, matrix b, matrix result, int size)
{

	// Find out which row and column we are on
	int row = blockIdx.y * blockDim.y + threadIdx.y; 
	int col = blockIdx.x * blockDim.x + threadIdx.x;
	int newSize = size;

	// If the size is not a multiple of blkSize, add one iteration to the for-loop
	if ( newSize%BLOCK_SIZE != 0 )
	{
		newSize += BLOCK_SIZE;
	}
	
	// Each thread will calculate one value for the resulting matrix
	float tmpRes = 0;

	// Find out which thread we are on inside the block
	int threadRow = threadIdx.y;
	int threadCol = threadIdx.x;	
		
	// Gather all subMatrices we need and copy them to shared memory
	for(int i=0; i < newSize; i+=BLOCK_SIZE)
	{			
		
		__shared__ float shA[BLOCK_SIZE][BLOCK_SIZE];
		__shared__ float shB[BLOCK_SIZE][BLOCK_SIZE];

		// Blocks from A moves horizontally, B vertically. If we are outside of matrices, set to zero.
		if( row < size && (i+threadCol) < size)
	 		shA[threadRow][threadCol] = a.element[row][i+threadCol];		
		else
			shA[threadRow][threadCol] = 0;

		if (col < size && (i+threadRow) < size)
			shB[threadRow][threadCol] = b.element[i+threadRow][col];
		else
			shB[threadRow][threadCol] = 0;

		// Sync all threads (block level) to make sure all needed elements are loaded
        __syncthreads();

		// Calculate the new value
		for(int k=0; k < BLOCK_SIZE; k++)
			tmpRes += shA[threadRow][k] * shB[k][threadCol];

		// Sync threads again to make sure the computation is done before loading the next matrices
		__syncthreads();
		
	}

	// Every thread copies one value, ignore the zero values
	if(row < size && col < size)
		result.element[row][col] = tmpRes;
}

void print_matrix(matrix m)
{
	int i, j;
	
	for (i = 0; i < size; i++)
	{
		printf("row %4d: ", i);
		for (j = 0; j < size; j++)
			printf("%6.2f  ", m.element[i][j]);
		printf("\n");
	}
}



void work()
{
	matrix a, b, result1, result2;
	long long before, after;
	int correct, i, j, dim;
	cudaError_t rc;

	// Allocate memory for matrices
	allocate_matrix(&a);
	allocate_matrix(&b);
	allocate_matrix(&result1);
	allocate_matrix(&result2);	

	// Initialize matrix elements
	init_matrix(a);
	init_matrix(b);

	// Perform sequential matrix multiplication
	before = wall_clock_time();
	mm(a, b, result1);
	after = wall_clock_time();
        fprintf(stderr, "Matrix multiplication on CPU took %1.2f seconds\n", ((float)(after - before))/1000000000);
		
	// Perform CUDA matrix  multiplication
	dim3 threadsPerBlock(BLOCK_SIZE, BLOCK_SIZE);	// a block of blkSize X blkSize CUDA threads
	dim = (size%BLOCK_SIZE==0) ? size/BLOCK_SIZE : size/BLOCK_SIZE + 1;
	dim3 blocksPerGrid(dim, dim);					// a grid of CUDA thread blocks, covers all elements in matrices
	before = wall_clock_time();
	mm_kernel<<<blocksPerGrid, threadsPerBlock>>>(a, b, result2, size);
	cudaDeviceSynchronize();
	after = wall_clock_time();
	fprintf(stderr, "Matrix multiplication on GPU took %1.2f seconds\n", ((float)(after - before))/1000000000);

	// was there any error?
        rc = cudaGetLastError();
        if (rc != cudaSuccess)
                printf("Last CUDA error %s\n", cudaGetErrorString(rc));

	// Compare the results
	correct = 1;
	for (i = 0; correct && i < size; i++)
		for (j = 0; j < size; j++)
			if (result1.element[i][j] != result2.element[i][j]) {
				correct = 0;
				break;
			}

	if (correct)
		printf("The result matrices are identical!\n");
	else
		printf("Difference in result matrices at element (%d, %d)!\n", i, j);

	free_matrix(&a);
	free_matrix(&b);
	free_matrix(&result1);
	free_matrix(&result2);
}


int main(int argc, char ** argv)
{
	srand(0); 

	printf("Usage: %s <size>\n", argv[0]);
    
	if (argc >= 2)
		size = atoi(argv[1]);
	else
		size = 1024;
		
	fprintf(stderr,"Sequential matrix multiplication of size %d\n", size);
    
	// Multiply the matrices
	work();

	return 0;
}
