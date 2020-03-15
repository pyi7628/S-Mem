#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#include "common.h"
#include "zipfianGenerator.h"
#include "benchmark_routine.h"

//common.h로 옮기기
#define TRUE 1
#define FALSE 0

size_t working_set_size = 1;
long long  working_set_per_thread = 0;
double zipfian_number = 0;// common.h에 있을 전역 사용 변수 같은애들!
int number_of_threads = 1;
size_t memory_alloc_size = 1;

void *mem;

///////////////////
_Bool is_random = FALSE;
int n_value = 1024;
int number_of_zipfian = 4194304;

void parse_input(int argc, char *argv[]);

int main(int argc, char *argv[])
{
	//parse input passed at stdin
	if(argc>1) parse_input(argc, argv);
	
	//memory allocation
	long long size = memory_alloc_size*GB;
	mem = malloc(size);

	//memory init
	memset(mem, 0, size);
	
	working_set_per_thread = (working_set_size*GB)/number_of_threads;//Is it possible with size_t?

	
	
	struct timespec tspec;
	long long start, end;
	double res;

	pthread_t *workers = (pthread_t*)malloc(sizeof(pthread_t)*number_of_threads);
	int *status = (int*)malloc(sizeof(int)*number_of_threads);
	int *zipf_arr = NULL;
	if(is_random)
	{
		//random value 값 받아오는 부분!!
		random_value_generator(zipfian_number, n_value, number_of_zipfian, &zipf_arr);
	}
	else
	{
		clock_gettime(CLOCK_REALTIME, &tspec);
		start = tspec.tv_nsec;
		for(int i=0;i<number_of_threads;i++)
		{
			void* start_address = mem + (i*working_set_per_thread);
			
			if(pthread_create(&workers[i], NULL, forwSequentialRead, (void*)start_address)<0)
			{
				perror("thread create error: ");
				exit(0);
			}
		}

		int result=0;
		for(int i=0;i<number_of_threads;i++)
		{
			pthread_join(workers[i],(void*)&result);
			//printf("status[%d]: %d\n",i, result);
		}

		clock_gettime(CLOCK_REALTIME, &tspec);
		end = tspec.tv_nsec;

		res = (double)(end - start)/1000000;
		
		printf("final result: %.3lf\n", res);
		
	}
	
	
 

	free(status);
	free(workers);
	free(mem);//필요하면 free하는 부분 함수로 만드는게 좋을 듯?
	return 0;
}










void parse_input(int argc, char *argv[])
{
	int opt;
	while((opt = getopt(argc, argv, "w:z:j:m:rn:")) != -1)
	{
		switch(opt)
		{
			case 'w':
			
				working_set_size = atoi(optarg);//it's always divide by 4(KB)
				break;
			
			case 'z':
			
				zipfian_number = atof(optarg);
				break;

			case 'j': 
				
				number_of_threads = atoi(optarg);
				break;
			case 'm':

				memory_alloc_size = atoi(optarg);
				break;

			case 'r':

				is_random = TRUE;
				break;

			case 'n':

				n_value = atoi(optarg);
				break;
			

			
			default :
				fprintf(stderr, "ERROR: Invalid option.\n");
		}

	}
}
