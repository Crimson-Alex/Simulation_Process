// Author: Alexander Schwarz
// Description: Simulation software for my master thesis
// 		Registers itself via /proc/planned_processes and performs some read/write operations on an array
//		A (legacy) matrix variant exists but was not used for conducting final experiments


// IMPORTS
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <omp.h>
#include <string.h>
#include <fcntl.h>



// MAIN
int main(int argc, char **argv)
{

	// Get mode from main.sh
	int mode = atoi(argv[1]);

	// Register process
	int fd = open("/proc/planned_processes", O_WRONLY);
	char buf[32];
	int task_id = 0; // Not pid!
	pid_t pid = getpid();
	
	if(fd < 0)
	{
		printf("Could not open /proc/planned_processes, proceeding without custom allocation. Is the alloc_pages_probe module running?\n");
	}
	else
	{
		snprintf(buf, sizeof(buf), "%d,%d\n", task_id, pid);
		if(write(fd, buf, strlen(buf)) < 0)
		{
			perror("Could not write to /proc_planned_processes. Do I have write permission?\n");
			close(fd);
		}
		else
		{
			printf("Successfully added %d to /proc/planned_processes\n", pid);
		}
	}



	// VARIANTS
	
	// Matrix variant
	// NOTE: Depricated
	if(mode == -1)
	{
		int size = 1024 * 256; // 1 GB
		int** array = (int**)malloc(size * sizeof(int*));

		if(array)
		{printf("Allocated %lu bytes for an array\n", size*sizeof(int));}
		else
		{printf("Failed to allocate array\n"); return 1;}

		for(int i = 0; i < size; i++)
		{
			array[i] = (int*)malloc(1024 * sizeof(int)); // 4096b == 1 page
			
			if(!array[i])
			{
				printf("Failed to allocated sub-array\n");
				for(int a = 0; a < i; a++)
				{
					free(array[a]);
				}
				free(array);
				return 1;
			}

		}

		// Writing data
		printf("Writing data for for some time\n");
		//clock_t start = clock();
		int iterations = 0;
		#pragma omp parallel num_threads(6)
		//while((((double)(clock() - start)) / CLOCKS_PER_SEC) < 60.0*6.0)
		while(iterations < 512*2)
		{
			for(int i = 0; i < size; i++)
			{
				for(int j = 0; j < 1024; j++)
				{
					array[i][j] = (i+1)*j;
				}
			}
			iterations++;
			printf("iterations=%d\n", iterations);
		}

		for(int i = 0; i < size; i++)
		{free(array[i]);}
		free(array);
	}

	// Repeated variant
	if(mode == 0)
	{
		int size = 1024 * 256; // 1 MB
		int* array = (int*)malloc(size * sizeof(int));

		if(array)
		{printf("Allocated %lu bytes for an array\n", size*sizeof(int));}
		else
		{printf("Failed to allocate array\n"); return 1;}

		// Writing data
		printf("Writing data for for some time\n");
		//clock_t start = clock();
		int iterations = 0;
		#pragma omp parallel num_threads(6)
		//while((((double)(clock() - start)) / CLOCKS_PER_SEC) < 60.0*6.0)
		while(iterations < 512)
		{
			for(int i = 0; i < size; i++)
			{
				array[iterations] += iterations; // Only accessing the first 512 values but more than once
			}
			iterations++;
			printf("iterations=%d\n", iterations);
		}
		free(array);
	}
	
	// Contiguous variant
	if(mode == 1)
	{
		int size = 1024 * 1024 * 256; // 1 GB
		int* array = (int*)malloc(size * sizeof(int));

		if(array)
		{printf("Allocated %lu bytes for an array\n", size*sizeof(int));}
		else
		{printf("Failed to allocate array\n"); return 1;}

		// Writing data
		printf("Writing data for for some time\n");
		//clock_t start = clock();
		int iterations = 0;
		#pragma omp parallel num_threads(6)
		//while((((double)(clock() - start)) / CLOCKS_PER_SEC) < 60.0*6.0)
		while(iterations < 512)
		{
			for(int i = 0; i < size; i++)
			{
				array[i] += iterations;
			}
			iterations++;
			printf("iterations=%d\n", iterations);
		}
		free(array);
	}

	// Strided variant
	if(mode == 2)
	{
		int size = 1024 * 1024 * 256; // 1 GB
		int* array = (int*)malloc(size * sizeof(int));

		if(array)
		{printf("Allocated %lu bytes for an array\n", size*sizeof(int));}
		else
		{printf("Failed to allocate array\n"); return 1;}

		// Writing data
		printf("Writing data for for some time\n");
		//clock_t start = clock();
		int iterations = 0;
		#pragma omp parallel num_threads(6)
		//while((((double)(clock() - start)) / CLOCKS_PER_SEC) < 60.0*6.0)
		while(iterations < 512)
		{
			for(int i = 0; i < size; i+=128)
			{
				array[i] += iterations;
			}
			iterations++;
			printf("iterations=%d\n", iterations);
		}
		free(array);
	}

	// Random Variant
	if(mode == 3)
	{
		int size = 1024 * 1024 * 256; // 1 GB
		int* array = (int*)malloc(size * sizeof(int));

		if(array)
		{printf("Allocated %lu bytes for an array\n", size*sizeof(int));}
		else
		{printf("Failed to allocate array\n"); return 1;}

		// Writing data
		printf("Writing data for for some time\n");
		//clock_t start = clock();
		int iterations = 0;
		#pragma omp parallel num_threads(6)
		//while((((double)(clock() - start)) / CLOCKS_PER_SEC) < 60.0*6.0)
		while(iterations < 6)
		{
			for(int i = 0; i < size; i++)
			{
				array[rand() % size] += iterations;
			}
			iterations++;
			printf("iterations=%d\n", iterations);
		}
		free(array);
	}



	// END
	
	// Unregister process
	if(fd < 0 || write(fd, buf, strlen(buf)) < 0)
	{
		printf("Could not write to /proc_planned_processes. Process might still be registered!\n");
	}
	else
	{
		snprintf(buf, sizeof(buf), "%d,%d\n", -task_id, pid);
		printf("Successfully unregistered process %d\n", pid);
		close(fd);
	}

	return 0;
}
