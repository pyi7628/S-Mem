#include <stdio.h>
#include <stdlib.h>
#include <time.h>//난수

#include "common.h"

double* zipf_arr=NULL;

void get_zipfian_values(char* file_name ,double** arr)
{
	*arr = (double*)malloc(sizeof(double) * (ZIPFVALUENUM+1));
	char temp[20] = {0,};
	FILE *fp;
	
	fp = fopen(file_name, "r");
	if(fp == NULL)
	{
		printf("ERROR in file: %s\n", file_name);
		exit(1);
	}
	int i=0;
	while(fgets(temp, 20, fp) != NULL)
	{
		*(*arr + i)=atof(temp);
		printf("%d: %.8lf\n", i, *(*arr+i));
		i++;
	}
	fclose(fp);

}

void get_zipfian_cumul(char* file_name ,double** arr)
{
	*arr = (double*)malloc(sizeof(double) * (ZIPFCUMULNUM+1));
	char temp[20] = {0,};
	FILE *fp;
	
	fp = fopen(file_name, "r");
	if(fp == NULL)
	{
		printf("ERROR in file: %s\n", file_name);
		exit(1);
	}
	int i=0;
	while(fgets(temp, 20, fp) != NULL)
	{
		if(temp[0] == 'a') continue;
		*(*arr + i) = atof(temp);
		if(*(*arr + i) == 1) break;
		i++;
	}

	fclose(fp);
	
}

int search_cumul(double random_value)
{	
	int left = 0;
	int right = ZIPFCUMULNUM - 1;
	int i = (left + right) / 2;
	while(left < right && random_value != zipf_arr[i])
	{
		if(random_value < zipf_arr[i])
		{
			right = i-1;
			i = (left + right) / 2;
		}
		else
		{
			left = i+1;
			i = (left + right) / 2;
		}
	}

	return i;//실제 value보다 1작음, 실제 offset값으로 맞는 값
}

double random_value_gen()
{
	srand(clock());
	int random_value = rand();
	double output = (double)random_value/RAND_MAX;
	return output;
}

int main()
{

	get_zipfian_cumul("../output/cumul_90.txt",&zipf_arr);

	double gen = random_value_gen();
	int result = search_cumul(1);
	printf("result: %d\n", result);

	return 0;
}
