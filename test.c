#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

int main()
{
	u_int64_t* arr = malloc(GB*sizeof(u_int64_t));
	memset(arr, 0, GB*sizeof(u_int64_t));
	printf("size: %d %ld\n", sizeof(void), GB); 

	for(;;)
	{
		;
	}

	return 0;
}
