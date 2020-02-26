#include <stdio.h>

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

//각 thread 별로 돌릴 수 있드록
//시간 측정에 관한거도 여기서 알아야 할듯..
int forwSequentialRead(void* start_address, void* end_address)
{
	register uint64_t val;
	volatile uint64_t* endptr = (uint64_t*)end_address;
	for(volatile uint64_t* curptr = (uint64_t*)start_address; curptr<endptr; )
	{
		UNROLL512(val = *curptr++;)
	}

	return 0;
}

int forwSequentialWrite(void* start_address, void* end_address)
{
	register uint64_t val = 0xFFFFFFFFFFFFFFFF;	
	volatile uint64_t* endptr = (uint64_t*)end_address;
	for(volatile uint64_t* curptr = (uint64_t*)start_address; curptr<endptr; )
	{
		UNROLL512(*curptr++ = val;)
	}

	return 0;
}	


//zipf 자료형 생각해보기
//longlong접근하는거 시간 들어가는거 영향 괜찮을지
//end_address 가 안필요할 수도!
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

}
