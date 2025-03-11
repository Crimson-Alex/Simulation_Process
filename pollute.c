#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>



// MAIN
int main(int argc, char **argv)
{
	// MAIN
	int size = 1024 * 1024 * 256; // 1 GB
	int* array = (int*)malloc(size * sizeof(int));

	if(array)
	{printf("Allocated %ld bytes for an array\n", size*sizeof(int));}
	else
	{printf("Failed to allocate array\n"); return 1;}

	clock_t start = clock();
	int r;
	printf("Performing random stuff for 300 seconds\n");
	#pragma omp parallel num_threads(6)
	while((((double)(clock() - start)) / CLOCKS_PER_SEC) < 300.0*6.0)
	{
		r = rand() % size;	
		array[r] = rand();
	}

	// Cleanup
	free(array);
	printf("Done\n");

	return 0;
}
