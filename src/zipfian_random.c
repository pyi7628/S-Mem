#include <stdio.h>
#include <stdlib.h>
#include <time.h>//난수
#include <math.h>
#include "common.h"

double* zipf_arr=NULL;
int* weight_arr=NULL;

int weight_size=0;

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

}//현재 안쓰고 있음

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
//zipfian random 값 구할 때 사용, 0~1사이의 , 1포함x, 범위 문제 되나 check해보기
double random_value_gen()
{
	srand(clock());
	int random_value = rand();
	double output = (double)random_value/RAND_MAX;
	return output;
}

//0~stride만큼의 무작위 random값 뽑음
int real_random_address_gen()
{

	int output = random_value_gen()*(memory_alloc_size * GB) / (ZIPFCUMULNUM * sizeof(int64_t));
	return output;
}

//cur_address구할 때 사용, 여기에서 적분한 값으로 access값받아오기
//0~(k-1)/sizeof(access_size)
int random_address_value_gen()
{
	//output이 수식 최종 근이 됨
	double z = random_value_gen();
	int k = (memory_alloc_size * GB) / (ZIPFCUMULNUM * sizeof(int64_t));
		//(sqrt(pow(2/k,2) - (4*z / pow((double)k,2.0))) - (double)2/k) / (-2/pow(k,2));
	int output = k-(pow((double)k, 2.0)*sqrt(pow((double)2/k, 2.0)-(4*z/pow((double)k, 2.0))))/2;
	double temp= (double)2/k;
	//printf("z: %lf, 1: %lf 2: %lf output: %d\n", z, temp, pow((double)k, 2.0), output);
	return output;
}


//mini test
void mini_init_zipfian_cumul()
{
	get_zipfian_cumul("output/mini_cumul_40.txt", &zipf_arr);
}
int mini_search_cumul(double random_value)
{	
	int left = 0;
	int right = ZIPFCUMULNUM/8 - 1;
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

int mini_get_random_access_value()
{

	double gen = random_value_gen();
	int result = mini_search_cumul(gen);
//	printf("result: %d\n", result);

	return result;
}
int mini_random_address_value_gen()
{
	//output이 수식 최종 근이 됨
	double z = random_value_gen();
	int k = (memory_alloc_size * GB/8) / (ZIPFCUMULNUM/8 * sizeof(int64_t));
		//(sqrt(pow(2/k,2) - (4*z / pow((double)k,2.0))) - (double)2/k) / (-2/pow(k,2));
	int output = k-(pow((double)k, 2.0)*sqrt(pow((double)2/k, 2.0)-(4*z/pow((double)k, 2.0))))/2;
	double temp= (double)2/k;
	//printf("z: %lf, 1: %lf 2: %lf output: %d\n", z, temp, pow((double)k, 2.0), output);
	return output;
}

void mini_zipf_data_check()
{
	long long i = 0;
	int stride = memory_alloc_size*GB/ZIPFCUMULNUM;
	while(i<memory_alloc_size*GB)
	{
		int64_t cur_address_offset = (mini_get_random_access_value()*(int64_t)stride)+mini_random_address_value_gen();
		//long long a=(long long)128*GB;
		printf("%lld\n",cur_address_offset);
		i++;
	}

}


void init_zipfian_cumul()
{
	get_zipfian_cumul("output/cumul_90.txt",&zipf_arr);
}

//이걸로 주소값 zipfian offset설정해줌
//0~1M
int get_random_access_value()
{

	double gen = random_value_gen();
	int result = search_cumul(gen);
//	printf("result: %d\n", result);

	return result;
}

void init_weight_arr()
{
	int size = (32/2)*(32+1)*4;
	weight_size = size; 
	weight_arr = (int*)malloc(sizeof(int)*(size+1));
	int count = 0;
	for(int i = 31; i >= 0; i--)//i 값이 몇개의 access point를만들지
	{
		for(int j = 0; j < 4*(32-i); j++)
		{
			weight_arr[count++] = i;
		}
	}

}

int random_second_address_gen()
{
	int gen = random_value_gen()*(weight_size);
	int result = weight_arr[gen];

	return result;

}

void free_arr()
{
	free(zipf_arr);
	free(weight_arr);
}
