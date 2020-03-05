#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#include "common.h"
#include "zipfianGenerator.h"
#include "benchmark_routine.h"
//header로 옮기기


void *mem;

int testnum = 0;//test
double total_run_time = 0;

pthread_mutex_t mutex_lock;

void parse_input(int argc, char *argv[]);

size_t	working_set_size = 1;
long long working_set_per_thread = 0;
int	zipfian_number = 0;
int	number_of_threads = 1;
size_t	memory_alloc_size = 1;// 1GB default	



int main(int argc, char *argv[])
{
	//parse input passed at stdin
	if(argc>1) parse_input(argc, argv);
	
	//memory allocation
	long long size = memory_alloc_size*GB;
	mem = malloc(size);

	//memory init
	memset(mem, 0, size);
	
	
	//calculate working set size per thread
	working_set_per_thread = (working_set_size*GB)/number_of_threads;//Is it possible with size_t?

	printf("working_set_per_thread: %lld\n", working_set_per_thread);
	

	
	struct timespec tspec;
	long long start, end;
	double res;

	pthread_t *workers = (pthread_t*)malloc(sizeof(pthread_t)*number_of_threads);
	int *status = (int*)malloc(sizeof(int)*number_of_threads);
	
	clock_gettime(CLOCK_REALTIME, &tspec);
	start = tspec.tv_sec * NANO + tspec.tv_nsec;
	for(int i=0;i<number_of_threads;i++)
	{
		void* start_address = mem + (i*working_set_per_thread);
		//printf("start address: %p mem arr: %lld\n", start_address, mem);
		if(pthread_create(&workers[i], NULL, forwSequentialRead, (void*)start_address)<0)
		{
			perror("thread create error: ");
			exit(0);
		}
	}

	double result;
	for(int i=0;i<number_of_threads;i++)
	{
		pthread_join(workers[i],(void*)&result);
		printf("status[%d]: %.3lf\n",i, result);
	}

	clock_gettime(CLOCK_REALTIME, &tspec);
	end = tspec.tv_sec * NANO + tspec.tv_nsec;

	res = (double)(end - start)/NANO;
	
	printf("read result: %.3lf\n", res);
	printf("read result: %.3lf\n", (res*NANO)/(working_set_size*GB));//?? ns/access
 

	clock_gettime(CLOCK_REALTIME, &tspec);
	start = tspec.tv_sec * NANO + tspec.tv_nsec;
	for(int i=0;i<number_of_threads;i++)
	{
		void* start_address = mem + (i*working_set_per_thread);
	//	printf("start address: %p mem arr: %lld\n", start_address, mem);
		if(pthread_create(&workers[i], NULL, forwSequentialWrite, (void*)start_address)<0)
		{
			perror("thread create error: ");
			exit(0);
		}
	}

	//int result=0;
	for(int i=0;i<number_of_threads;i++)
	{
		pthread_join(workers[i],(void*)&result);
		//printf("status[%d]: %d\n",i, result);
	}

	clock_gettime(CLOCK_REALTIME, &tspec);
	end = tspec.tv_sec * NANO + tspec.tv_nsec;

	res = (double)(end - start)/NANO;
	
	printf("write result: %.3lf\n", res);



	free(status);
	free(workers);
	free(mem);//필요하면 free하는 부분 함수로 만드는게 좋을 듯?
	return 0;
}




void parse_input(int argc, char *argv[])
{
	int opt;
	while((opt = getopt(argc, argv, "w:z:j:m:")) != -1)
	{
		switch(opt)
		{
			case 'w':
			
				working_set_size = atoi(optarg);//it's always divide by 4(KB)
				break;
			
			case 'z':
			
				zipfian_number = atoi(optarg);
				break;

			case 'j': 
				
				number_of_threads = atoi(optarg);
				break;
			case 'm':

				memory_alloc_size = atoi(optarg);
				break;

			
			default :
				fprintf(stderr, "ERROR: Invalid option.\n");
		}

	}
}
