#include <cpuid.h>
#include <immintrin.h>
#include <x86intrin.h>
#include <stdio.h>
#include <time.h>

#include "common.h"

//extern 선언
int64_t g_ticks_per_ms;
double g_ns_per_tick;


int64_t start_time()
{
	volatile int32_t dc0 = 0;
	volatile int32_t dc1, dc2, dc3, dc4;
	__cpuid(dc0, dc1, dc2, dc3, dc4);
	return __rdtsc();
}

int64_t stop_time()
{	
	u_int64_t tick;
	u_int32_t filler;
	volatile int32_t dc0 = 0;
	volatile int32_t dc1, dc2, dc3, dc4;
	tick = __rdtscp(&filler);
	__cpuid(dc0, dc1, dc2, dc3, dc4);
	return tick;
}

void check_timer()
{
	int64_t start = start_time();
	struct timespec duration, remainder;
	duration.tv_sec = DURATION_MS / 1000;
	duration.tv_nsec = (DURATION_MS % 1000) * 1e6;
	nanosleep(&duration, &remainder);
	int64_t end = stop_time();

	g_ticks_per_ms = (end - start) / DURATION_MS;

	g_ns_per_tick =  1 / (double)g_ticks_per_ms * (double)1e6;
}	

int main()
{
	check_timer();
	printf("%d, %f\n", g_ticks_per_ms, g_ns_per_tick);

	return 0;
}
