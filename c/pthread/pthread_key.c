/*
 * =====================================================================================
 *
 *       Filename:  pthread_key.c
 *
 *    Description:  线程私有全局变量
 *
 *        Version:  1.0
 *        Created:  01/14/2016 10:46:54 AM
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

pthread_key_t pgkey;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  key_destructor
 *  Description:  线程结束，join之前，释放pthread_key_t
 * =====================================================================================
 */
void key_destructor(void *key)
{
	printf("key_destructor: %d[%p]\n", *(int*)key, key);
}		/* -----  end of function key_destructor  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  pthread_fun
 *  Description:   
 * =====================================================================================
 */
void* pthread_fun(void *arg)
{
	int no = (int)(long)arg;
	int key = no + 100;
	int *p = 0;

	if (no != 4) {
		if (pthread_setspecific(pgkey, &key) != 0) {
			printf("no: %d, key set error.\n", no);
			return NULL;
		}
	}

	printf("pthread_fun: %d\n", no);
	p = pthread_getspecific(pgkey);
	if (p == NULL) {
		printf("no: %d, key not found.\n", no);
		return NULL;
	}
	printf("pthread_key: %d\n", *p);

	return NULL;
}		/* -----  end of function pthread_fun  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int main(int argc, char *argv[])
{
	pthread_t tid1, tid2, tid3, tid4;
	pthread_key_create(&pgkey, key_destructor);

	pthread_create(&tid1, NULL, pthread_fun, (void*)(long)1);
	pthread_create(&tid2, NULL, pthread_fun, (void*)(long)2);
	pthread_create(&tid3, NULL, pthread_fun, (void*)(long)3);
	pthread_create(&tid4, NULL, pthread_fun, (void*)(long)4);

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	pthread_join(tid3, NULL);
	pthread_join(tid4, NULL);

	pthread_key_delete(pgkey);

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */

