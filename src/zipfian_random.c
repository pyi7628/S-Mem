#include <stdio.h>
#include <stdlib.h>


//#include "common.h"
#define ZIPFVALUENUM 4194304
#define ZIPFCUMULNUM 1048576
void get_zipfian_values(char* file_name ,double** zipf_arr)
{
	*zipf_arr = (double*)malloc(sizeof(double) * (ZIPFVALUENUM+1));
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
		*(*zipf_arr + i)=atof(temp);
		printf("%d: %.8lf\n", i, *(*zipf_arr+i));
		i++;
	}
	fclose(fp);

}
void get_zipfian_cumul(char* file_name ,double** zipf_arr)
{
	*zipf_arr = (double*)malloc(sizeof(double) * (ZIPFCUMULNUM+1));
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
		*(*zipf_arr + i) = atof(temp);
		if(*(*zipf_arr + i) == 1) break;
		i++;
	}

	fclose(fp);
	
}

int search_cumul(double* cumul_arr, double random_value)
{	
	int left = 0;
	int right = ZIPFCUMULNUM - 1;
	int i = (left + right) / 2;
	while(left < right && random_value != cumul_arr[i])
	{
		if(random_value > cumul_arr[i])
		{
			right = i;
			i = (left + right) / 2;
		}
		else
		{
			left = i;
			i = (left + right) / 2;
		}
	}

	return i;
}

int main()
{
	double* zipf_arr=NULL;
	get_zipfian_cumul("../output/cumul_93.txt",&zipf_arr);

	printf("zipf: %.7lf %.7lf\n", zipf_arr[0], zipf_arr[5]);

	printf("search: %d\n",search_cumul(zipf_arr, 1));
	return 0;
}