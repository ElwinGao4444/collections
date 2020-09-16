/*
 * =====================================================================================
 *
 *       Filename:  reorder.c
 *
 *    Description:  CPU will be reorder when running
 *
 *        Version:  1.0
 *        Created:  11/21/2013 05:46:31 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Elwin.Gao (elwin), elwin.gao4444@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

static int x, y, a, b;

void* thread_one(void *arg)
{
	a = 1;
	x = b;
	return NULL;
}

void* thread_two(void *arg)
{
	b = 1;
	y = a;
	return NULL;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int main(int argc, char *argv[])
{
	int i;
	pthread_t one, two;
	for (i=0; i<10000; ++i) {
		x=y=a=b=0;
		pthread_create(&one, NULL, thread_one, NULL);
		pthread_create(&two, NULL, thread_two, NULL);
		pthread_join(one, NULL);
		pthread_join(two, NULL);
		if (x==0 && y==0)
			printf("x=%d, y=%d\n", x, y);
	}

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
