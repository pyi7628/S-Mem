#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>

#include "common.h"
#include "benchmark_routine.h"


pthread_mutex_t mutex_lock;
//각 thread 별로 돌릴 수 있드록
//시간 측정에 관한거도 여기서 알아야 할듯..
void* forwSequentialRead(void* start_address)
{
	register int64_t val;
	volatile int64_t* endptr = (int64_t*)start_address + (working_set_per_thread/sizeof(int64_t));
	printf("valid?: %ld  %ld\n",start_address, endptr);
	int64_t start, end;
	int64_t elapsed;
	double res;
	int64_t passes=0;

	//printf("start time: %d\n", start);
	for(volatile int64_t* curptr = (int64_t*)start_address; curptr < endptr; )
	{
		start = start_time();

		UNROLL512(val = *curptr++;)

		end = stop_time();

		elapsed += (end - start);
		passes++;
	}
	res = (double)elapsed * g_ns_per_tick;
	printf(" elapsed: %ld clock: %.3f %lf\n",elapsed,  res, g_ns_per_tick);
	printf(" ns/accesses: %.3f %ld\n", res/(512*passes), passes);
//	pthread_mutex_lock(&mutex_lock);
	//total_run_time += res;
//	printf("total: %.3f\n", total_run_time);
//	pthread_mutex_unlock(&mutex_lock);

	return (void*)(&res);

}

void* forwSequentialWrite(void* start_address)
{
	register int64_t val = 0xFFFFFFFFFFFFFFFF;
	volatile int64_t* endptr = (int64_t*)start_address + (working_set_per_thread/sizeof(int64_t));
	//printf("valid?: %ld\n", endptr-(u_int64_t*)start_address);
	int64_t start, end;
	int64_t elapsed;
	double res;
	struct timespec ts;

	//printf("start time: %d\n", start);
	for(volatile int64_t* curptr = (int64_t*)start_address; curptr<endptr; )
	{
		start = start_time();

		UNROLL512(*curptr++ = val;)

		end = stop_time();

		elapsed += (end - start);

	}
	res = (double)elapsed * g_ns_per_tick;
	printf("clock: %.3f\n", res);
//	pthread_mutex_lock(&mutex_lock);
	//total_run_time += res;
//	printf("total: %.3f\n", total_run_time);
//	pthread_mutex_unlock(&mutex_lock);

	return (void*)(&res);
}
/*
void* randomReadTest(void* start_address, char* zipf_arr)
{
	register char val;
	volatile char* startptr = (char*)start_address;
	clock_t start, end;
	clock_t elapsed;
	double res;
	struct timespec ts;
	

	long long total_passes=0;
	int cur_pass=0;
	//int i=0;
	for(;  1024*1024*4	> total_passes; )
	{

		UNROLL512(val = *(startptr + zipf_arr[cur_pass++];)
		if(cur_pass>1)


	}

	return (void*)(&res);

}*/
/*a
int randomRead(void* start_address, void* end_address, long long* zipf_rv)
{
	register uint64_t val;
	volatile uint64_t* endptr = (uint64_t*)end_address;
	volatile uint64_t* curptr = (uint64_t*)start_address;
	for(long long offset_index=0; offset_index < working_set_per_thread; )
	{
		UNROLL512(val = *(curptr+zipf_rv[offset_index++]);)
	}

	return 0;

}

int randomWrite(void* start_address, void* end_address, long long* zipf_rv)
{
	register uint64_t val = 0xFFFFFFFFFFFFFFFF;
	volatile uint64_t* endptr = (uint64_t*)end_address;
	volatile uint64_t* curptr = (uint64_t*)start_address;
	for(long long offset_index=0; offset_index < working_set_per_thread; )
	{
		UNROLL512(*(curptr+zipf_rv[offset_index++]) = val;)
	}

	return 0;

}*/
