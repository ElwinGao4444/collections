/*
// =====================================================================================
// 
//       Filename:  likely.cpp
// 
//    Description:  测试likely和unlikely的性能效果
// 
//        Version:  1.0
//        Created:  07/13/2013 11:31:39 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Elwin.Gao (elwin), elwin.gao4444@gmail.com
//        Company:  
// 
// =====================================================================================
*/
#include "../gzlib_util.hpp"

#define TIMES (10000000)
#define BILLION (1000000000)
#define MILLION 1000000

int main(int argc,char *argv[])
{
	int i;
	int j;
	int *array = NULL;
	struct timespec tpstart;
	struct timespec tpend;
	unsigned long long timeif;
	unsigned long long timeif_func;
	struct timeval tv;
	if(argc != 2)
	{
		printf("usage: ./test times\n");
		return -1;
	}

	int times = atoi(argv[1]);

	if(clock_gettime(CLOCK_MONOTONIC,&tpstart))
	{
		fprintf(stderr,"Fail to get start time for NULL\n");
		return -1;
	}
	for(i = 0;i<times;i++)
	{
		;
	}

	if(clock_gettime(CLOCK_MONOTONIC,&tpend))
	{
		fprintf(stderr,"Fail to get start time for NULL\n");
		return -1;
	}

	timeif = MILLION*(tpend.tv_sec - tpstart.tv_sec)+
		(tpend.tv_nsec-tpstart.tv_nsec)/1000;

	printf("%d BLANK OP 's total time is %llu us\n",times,(timeif));


	if(clock_gettime(CLOCK_MONOTONIC,&tpstart))
	{
		fprintf(stderr,"Fail to get start time for branch \n");
		return -1;
	}
	for(i = 0;i<times;i++)
	{
		j = rand();
		if(likely(j >1))
		{
			array = (int*)malloc(1000*sizeof(int));
			if(array)
				free(array);
		}
		else
		{
			gettimeofday(&tv,NULL);
		}
	}

	if(clock_gettime(CLOCK_MONOTONIC,&tpend))
	{
		fprintf(stderr,"Fail to get start time for branch\n");
		return -1;
	}

	timeif_func =MILLION*(tpend.tv_sec - tpstart.tv_sec)+
		(tpend.tv_nsec-tpstart.tv_nsec)/1000;

	printf("%d FUNCTION's with right prediction cost time is %llu us\n",
			times,(timeif_func));	
}

