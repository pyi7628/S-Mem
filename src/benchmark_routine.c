#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>

#include "common.h"
#include "benchmark_routine.h"


pthread_mutex_t mutex_lock;

int32_t test(int64_t* cur_address, int* random_arr)
{
	register int64_t val;
	volatile int64_t* curptr  = cur_address;	
	
	int index = 0;
	UNROLL512( val = *(curptr + random_arr[index++]);) 

	return 0;


}
void* latency_randomRead_test(void* start_address)
{
	int64_t start, end;
	int64_t elapsed = 0;
	double res;
	int64_t passes=0;
	int64_t* cur_address = (int64_t*)start_address;

	int zipf_temp_arr[257][513]={0,};
	int64_t cur_address_arr[257]={0,};

	int64_t target_ticks = g_ticks_per_ms * DURATION_MS;

	while(elapsed < target_ticks)
	{
		int zipf_index = 0;
		int cur_address_index = 0;
		for(int i=0;i<256;i++)
		{
			for(int j=0; j<512; j++)
			{
				zipf_temp_arr[i][j] = get_random_access_value();
			}
			cur_address_arr[i] = random_address_value_gen();
			//printf("%d\n ", cur_address_arr[i]);
		}
		start = start_time();
		UNROLL256(test(cur_address + (cur_address_arr[cur_address_index++]*ZIPFCUMULNUM), zipf_temp_arr[zipf_index++]);)
		end = stop_time();
		elapsed += (end - start);
		passes += 256;
		//printf("elapsed: %ld, target: %ld\n", elapsed, target_ticks);
	}

	res = (double)elapsed * g_ns_per_tick;
	printf(" elapsed: %ld clock: %.3f ns: %lf g_tick: %ld\n",elapsed,  res, g_ns_per_tick,g_ticks_per_ms);
	printf("ns/accesses: %lf %.3f %ld\n",res,  (double)(elapsed*g_ns_per_tick)/(double)(passes*512), passes);

//	pthread_muex_lock(&mutex_lock);
//	pthread_mutex_unlock(&mutex_lock);

	return (void*)(&res);//여기서 latency return

}
//각 thread 별로 돌릴 수 있드록
//시간 측정에 관한거도 여기서 알아야 할듯..
/*void* forwSequentialRead(void* start_address)
{
	int64_t start, end;
	int64_t elapsed = 0;
	double res;
	int64_t passes=0;
	unsigned int* next_address = (unsigned int*)start_address;// 이거 사실은 mem 의 처음 부분임!!

	int64_t target_ticks = g_ticks_per_ms * DURATION_MS;
	while(elapsed < target_ticks)
	{
		start = start_time();
		//UNROLL256(test(next_address, &next_address, 0);)
		end = stop_time();
		elapsed += (end - start);
		passes += 256;
	}
	
	res = (double)elapsed * g_ns_per_tick;
	printf(" elapsed: %ld clock: %.3f %lf\n",elapsed,  res, g_ns_per_tick);
	printf("ns/accesses: %lf %.3f %ld\n",res,  res/(passes*512), passes);
//	pthread_mutex_lock(&mutex_lock);
//	pthread_mutex_unlock(&mutex_lock);

	return (void*)(&res);

}*/
void* forwSequentialRead(void* start_address)
{
	register int64_t val;
	volatile int64_t* endptr = (int64_t*)start_address + (working_set_per_thread/sizeof(int64_t));
	//printf("valid?: %ld  %ld\n",start_address, endptr);
	int64_t start, end;
	int64_t elapsed = 0;
	double res;
	int64_t passes=0;

	int64_t target_ticks = g_ticks_per_ms * DURATION_MS;
	for(volatile int64_t* curptr = (int64_t*)start_address; curptr < endptr; )
	{
		start = start_time();

		UNROLL512(val = *curptr++;)

		end = stop_time();
		printf("%lf\n",(end-start)*g_ns_per_tick/512);
		elapsed += (end - start);
		passes++;
	}

	
	res = (double)elapsed * g_ns_per_tick;
	printf(" elapsed: %ld clock: %.3f %lf\n",elapsed,  res, g_ns_per_tick);
	printf("ns/accesses: %lf %.3f %ld\n",res,  res/(passes*512), passes);
//	pthread_mutex_lock(&mutex_lock);
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
	int64_t passes = 0;

	//printf("start time: %d\n", start);
	for(volatile int64_t* curptr = (int64_t*)start_address; curptr<endptr; )
	{
		start = start_time();

		UNROLL512(*curptr++ = val;)

		end = stop_time();

		elapsed += (end - start);
		passes++;
	}
	res = (double)elapsed * g_ns_per_tick;
	printf("ns/accesses: %lf %.3f\n", res, res/(512*passes));
//	pthread_mutex_lock(&mutex_lock);
	//total_run_time += res;
//	printf("total: %.3f\n", total_run_time);
//	pthread_mutex_unlock(&mutex_lock);

	return (void*)(&res);
}

int randomRead(void* start_address)
{
	register int64_t val;
	//딱히 안필요할듯??
//	volatile int64_t* endptr = (int64_t*)start_address + (working_set_per_thread/sizeof(int64_t));
	volatile int64_t* curptr = (int64_t*)start_address;

	int64_t total_passes = 0;//총 몇개 pass까지 할지! 전체 메모리에 byte크기 나눈거
	int32_t cur_passes = 0;//총 1M 범위의 access니까 이거 얼마나 발생시킬지! 위에꺼 1M으로나눈거
	int zipf_temp_arr[520] = {0,};
	
	int64_t start, end;
	int64_t elapsed = 0;
	double res;

	for(;total_passes < total_access_size ;)
	{
		for(cur_passes = 0; cur_passes < ZIPFCUMULNUM; )
		{
			int zipf_index = 0;
			//512access단위로 접근할 때 random값 생성하는 시간을 빼기 위함
			for(int i=0; i<512; i++)
			{
				zipf_temp_arr[i] = get_random_access_value();
			}
			//int64_t check=curptr + 1;
			//printf("%ld %ld\n", curptr, curptr+1);
			start = start_time();	
			UNROLL512(val = *(curptr+zipf_temp_arr[zipf_index++]);)//여기에 zipf_temp접근하는 시간은 빼야하지 않나? dummy?
			end = stop_time();
			
			elapsed += (end - start);
			cur_passes += 512;
		}
		total_passes += ZIPFCUMULNUM;
		curptr = curptr + ZIPFCUMULNUM;
	}
	
	int64_t dummy_elapsed = 0;
	for(total_passes = 0; total_passes < total_access_size ;)
	{
		for(cur_passes = 0; cur_passes < ZIPFCUMULNUM; )
		{
			int zipf_index = 0;
			//512access단위로 접근할 때 random값 생성하는 시간을 빼기 위함
			for(int i=0; i<512; i++)
			{
				zipf_temp_arr[i] = get_random_access_value();
			}
			//int64_t check=curptr + 1;
			//printf("%ld %ld\n", curptr, curptr+1);
			start = start_time();	
			UNROLL512(zipf_temp_arr[zipf_index++];)//여기에 zipf_temp접근하는 시간은 빼야하지 않나? dummy?
			end = stop_time();
			
			dummy_elapsed += (end - start);
			cur_passes += 512;
		}
		total_passes += ZIPFCUMULNUM;
		curptr = curptr + ZIPFCUMULNUM;
	}


	res = (double)(elapsed - dummy_elapsed) * g_ns_per_tick;

	printf("ns/accesses: %lf %.3f\n", res,res/(total_passes));

	return (void*) &res;

}

void* randomWrite(void* start_address)
{
	register int64_t val = 0xFFFFFFFFFFFFFFFF;
	volatile int64_t* curptr = (int64_t*)start_address;
		//UNROLL512(*(curptr+zipf_rv[offset_index++]) = val;)
	
	int64_t total_passes = 0;//총 몇개 pass까지 할지! 전체 메모리에 byte크기 나눈거
	int32_t cur_passes = 0;//총 1M 범위의 access니까 이거 얼마나 발생시킬지! 위에꺼 1M으로나눈거
	int zipf_temp_arr[520] = {0,};
	
	int64_t start, end;
	int64_t elapsed = 0;
	double res;



	for(;total_passes < total_access_size ;)
	{
		for(cur_passes = 0; cur_passes < ZIPFCUMULNUM; )
		{
			int zipf_index = 0;
			//512access단위로 접근할 때 random값 생성하는 시간을 빼기 위함
			for(int i=0; i<512; i++)
			{
				zipf_temp_arr[i] = get_random_access_value();
			}
			//int64_t check=curptr + 1;
			//printf("%ld %ld\n", curptr, curptr+1);
			start = start_time();	
			UNROLL512( *(curptr+zipf_temp_arr[zipf_index++]) = val;)//여기에 zipf_temp접근하는 시간은 빼야하지 않나? dummy?
			end = stop_time();
			
			elapsed += (end - start);
			cur_passes += 512;
		}
		total_passes += ZIPFCUMULNUM;
		curptr = curptr + ZIPFCUMULNUM;
	}


	int64_t dummy_elapsed = 0;
	for(total_passes = 0; total_passes < total_access_size ;)
	{
		for(cur_passes = 0; cur_passes < ZIPFCUMULNUM; )
		{
			int zipf_index = 0;
			//512access단위로 접근할 때 random값 생성하는 시간을 빼기 위함
			for(int i=0; i<512; i++)
			{
				zipf_temp_arr[i] = get_random_access_value();
			}
			//int64_t check=curptr + 1;
			//printf("%ld %ld\n", curptr, curptr+1);
			start = start_time();	
			UNROLL512(zipf_temp_arr[zipf_index++];)//여기에 zipf_temp접근하는 시간은 빼야하지 않나? dummy?
			end = stop_time();
			
			dummy_elapsed += (end - start);
			cur_passes += 512;
		}
		total_passes += ZIPFCUMULNUM;
		curptr = curptr + ZIPFCUMULNUM;
	}


	res = (double)(elapsed - dummy_elapsed) * g_ns_per_tick;

	printf("ns/accesses: %lf %.3f\n", res,res/(total_passes));



	return (void*) &res;

}
