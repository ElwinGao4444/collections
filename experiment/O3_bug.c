/*
 * =====================================================================================
 *
 *       Filename:  O3_bug.c
 *
 *    Description:  O1 O2 & O3 bug when no volatile (only O0 is right)
 *
 *        Version:  1.0
 *        Created:  07/14/2013 04:29:32 PM
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

/* two ways: volatile or memory barrier */
int sign = 0;
int wait = 0;

#define barrier() __asm__ __volatile__("": : :"memory")

void* thread_check(void *arg)
{
	for ( ; ; ) {
		if (sign == 1)
			break;
		barrier();
		wait = 1;
	}
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
	pthread_t tid;
	pthread_create(&tid, NULL, thread_check, NULL);

	while (wait == 0)
		barrier();
	sign = 1;

	pthread_join(tid,NULL);

	printf("Hello World!\n");
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
