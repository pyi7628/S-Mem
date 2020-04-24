#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#include "common.h"
#include "benchmark_routine.h"
#include "zipfian_random.h"

//common.h로 옮기기
#define TRUE 1
#define FALSE 0

//맨 처음 setup하는거 함수로 만들기
size_t working_set_size = 1;
long long  working_set_per_thread = 0;
double zipfian_number = 0;// common.h에 있을 전역 사용 변수 같은애들!
int number_of_threads = 1;
size_t memory_alloc_size = 1;

//random
long long total_access_size = 0;
int accesses_per_iter = 0;

void *mem;

///////////////////코드 정리 필요!
_Bool is_random = FALSE;
int n_value = 1024;
int number_of_zipfian = 4194304;

void parse_input(int argc, char *argv[]);

int main(int argc, char *argv[])
{
	//parse input passed at stdin
	if(argc>1) parse_input(argc, argv);
	//init time variable
	init_timer();
	//memory allocation
	long long size = memory_alloc_size*GB;
	mem = malloc(size);

	//memory init
	memset(mem, 0xFFFFFFFFFFFFFFFF, size);
	
	working_set_per_thread = (working_set_size*GB)/number_of_threads;//Is it possible with size_t?

	
//	printf("last address: %ld\n", &mem[size-1]);
	
	struct timespec tspec;
	long long start, end;
	double res;

	pthread_t *workers = (pthread_t*)malloc(sizeof(pthread_t)*number_of_threads);
	int *status = (int*)malloc(sizeof(int)*number_of_threads);
	int *zipf_arr = NULL;
	if(is_random)
	{
		//random value 값 받아오는 부분!!
		//get_zipfian_cumul("../output/cumul_90.txt");
		init_zipfian_cumul();
		init_weight_arr();
	
		//printf("random: %d\n", random_address_value_gen());
		for(int i=0;i<20;i++)
		{
			random_address_value_gen();
		}

		total_access_size = working_set_size*GB/sizeof(int64_t);
		//1M 으로 구간 나눴을 때 각각이 얼마인지!!
		accesses_per_iter = total_access_size/ZIPFCUMULNUM;
		//latency_randomRead_test(mem);
	}
	else
	{
		clock_gettime(CLOCK_REALTIME, &tspec);
		start = tspec.tv_sec * NANO + tspec.tv_nsec;
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
		end = tspec.tv_sec * NANO + tspec.tv_nsec;

		res = (double)(end - start)/NANO;
		
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
