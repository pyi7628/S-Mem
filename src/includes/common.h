#define GB 1073741824
#define NANO 1000000000
#define ZIPFVALUENUM 4194304
#define ZIPFCUMULNUM 1048576
#define DURATION_MS 5000

extern int64_t g_ticks_per_ms;
extern double g_ns_per_tick;


extern size_t working_set_size;
extern long long  working_set_per_thread;
extern double zipfian_number;// common.h에 있을 전역 사용 변수 같은애들!
extern int number_of_threads;
extern size_t memory_alloc_size;
