#include <stdio.h>
#include <stdlib.h>

int main()
{
	int64_t *arr=(int64_t*)malloc(sizeof(int64_t)*10);
	printf("%p %p\n", arr, &arr[1]);
	return 0;
}
