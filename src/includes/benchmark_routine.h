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


void* forwSequentialRead(void* start_address);

void* forwSequentialWrite(void* start_address);
