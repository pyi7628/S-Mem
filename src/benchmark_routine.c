#include <stdio.h>
#include <time.h>
#include <pthread.h>

#include "common.h"
#include "benchmark_routine.h"


pthread_mutex_t mutex_lock;
//각 thread 별로 돌릴 수 있드록
//시간 측정에 관한거도 여기서 알아야 할듯..
void* forwSequentialRead(void* start_address)
{
	register char val;
	volatile char* endptr = (char*)start_address + (working_set_per_thread);
	//printf("valid?: %ld\n", endptr-(u_int64_t*)start_address);
	clock_t start, end;
	clock_t elapsed;
	double res;
	struct timespec ts;

	//printf("start time: %d\n", start);
	for(volatile char* curptr = (char*)start_address; curptr<endptr; )
	{
		clock_gettime(CLOCK_REALTIME, &ts);
		start = ts.tv_sec * NANO + ts.tv_nsec;

		UNROLL512(val = *curptr++;)

		clock_gettime(CLOCK_REALTIME, &ts);
		end = ts.tv_sec * NANO + ts.tv_nsec;

		elapsed += (end - start);

	}
	res = (double)elapsed / NANO;
	printf("clock: %.3f\n", res);
	pthread_mutex_lock(&mutex_lock);
	//total_run_time += res;
//	printf("total: %.3f\n", total_run_time);
	pthread_mutex_unlock(&mutex_lock);

	return (void*)(&res);
}

void* forwSequentialWrite(void* start_address)
{
	register char val = 0xFF;
	volatile char* endptr = (char*)start_address + (working_set_per_thread);
	//printf("valid?: %ld\n", endptr-(u_int64_t*)start_address);
	clock_t start, end;
	clock_t elapsed;
	double res;
	struct timespec ts;

	//printf("start time: %d\n", start);
	for(volatile char* curptr = (char*)start_address; curptr<endptr; )
	{
		clock_gettime(CLOCK_REALTIME, &ts);
		start = ts.tv_sec * NANO + ts.tv_nsec;

		UNROLL512(*curptr++ = val;)

		clock_gettime(CLOCK_REALTIME, &ts);
		end = ts.tv_sec * NANO + ts.tv_nsec;

		elapsed += (end - start);

	}
	res = (double)elapsed / NANO;
//	printf("clock: %.3f\n", res);
	pthread_mutex_lock(&mutex_lock);
	//total_run_time += res;
//	printf("total: %.3f\n", total_run_time);
	pthread_mutex_unlock(&mutex_lock);

	return (void*)(&res);
}


//zipf 자료형 생각해보기
//longlong접근하는거 시간 들어가는거 영향 괜찮을지
//end_address 가 안필요할 수도!
/*int randomRead(void* start_address, void* end_address, long long* zipf_rv)
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
