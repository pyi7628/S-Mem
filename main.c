#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>


#define GB 1073741824

#define UNROLL2(x) x x
#define UNROLL4(x) UNROLL2(x) UNROLL2(x)
#define UNROLL8(x) UNROLL4(x) UNROLL4(x)
#define UNROLL16(x) UNROLL8(x) UNROLL8(x)
#define UNROLL32(x) UNROLL16(x) UNROLL16(x)
#define UNROLL64(x) UNROLL32(x) UNROLL32(x)
#define UNROLL128(x) UNROLL64(x) UNROLL64(x)
#define UNROLL256(x) UNROLL128(x) UNROLL128(x)
#define UNROLL512(x) UNROLL256(x) UNROLL256(x)
#define UNROLL1024(x) UNROLL512(x) UNROLL512(x)


size_t working_set_size = 1;
long long  working_set_per_thread = 0;
int zipfian_number = 0;// common.h에 있을 전역 사용 변수 같은애들!
int number_of_threads = 1;
size_t memory_alloc_size = 1;

void *mem;

int testnum = 0;//test
double total_run_time = 0;

pthread_mutex_t mutex_lock;

void parse_input(int argc, char *argv[]);


void *t_func(void *data);
void* forwSequentialRead(void* start_address);



int main(int argc, char *argv[])
{
	//parse input passed at stdin
	if(argc>1) parse_input(argc, argv);
	
	//memory allocation
	long long size = memory_alloc_size*GB;
	mem = malloc(size);

	//memory init
	memset(mem, 0, size);
	
	
	//mem[GB-1]=1;
	//print first of memory address
	//printf("mem address: %p, %p, %ld\n", mem,&mem[1], &mem[GB-1]-mem);

	//calculate working set size per thread
	working_set_per_thread = (working_set_size*GB)/number_of_threads;//Is it possible with size_t?

	printf("working_set_per_thread: %lld\n", working_set_per_thread);
	
	//printf("test val: %d %d %d\n", mem[working_set_per_thread-1], mem[GB-1],*(mem+working_set_per_thread-1));

	//int data=0;//test
	
	struct timespec tspec;
	long long start, end;
	double res;

	pthread_t *workers = (pthread_t*)malloc(sizeof(pthread_t)*number_of_threads);
	int *status = (int*)malloc(sizeof(int)*number_of_threads);
	
	clock_gettime(CLOCK_REALTIME, &tspec);
	start = tspec.tv_nsec;
	for(int i=0;i<number_of_threads;i++)
	{
		void* start_address = mem + (i*working_set_per_thread);
		printf("start address: %p mem arr: %lld\n", start_address, mem);
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
 

	free(status);
	free(workers);
	free(mem);//필요하면 free하는 부분 함수로 만드는게 좋을 듯?
	return 0;
}






void *t_func(void *data)
{
	int i=0;
	int a=(*((int *)data))++;
	printf("i=%d\n",i++);
	pthread_mutex_lock(&mutex_lock);
	//printf("%d\n",testnum);
	testnum++;
	pthread_mutex_unlock(&mutex_lock);
	return (void *)(a);
	
}

void* forwSequentialRead(void* start_address)
{
	register char val;
	volatile char* endptr = (char*)start_address + (working_set_per_thread);
	//printf("valid?: %ld\n", endptr-(u_int64_t*)start_address);
	clock_t start, end;
	clock_t elapsed;
	double res;

	//printf("start time: %d\n", start);
	for(volatile char* curptr = (char*)start_address; curptr<endptr; )
	{
		start = clock();
		UNROLL512(val = *curptr++;)
		end = clock();

		elapsed += (end - start);

	}
	res = (double)elapsed / CLOCKS_PER_SEC;
	printf("clock: %.3f\n", res);
	pthread_mutex_lock(&mutex_lock);
	total_run_time += res;
	printf("total: %.3f\n", total_run_time);
	pthread_mutex_unlock(&mutex_lock);

	return (void*) &res;
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
