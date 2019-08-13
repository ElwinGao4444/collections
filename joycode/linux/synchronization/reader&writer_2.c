/*
 * =====================================================================================
 *
 *       Filename:  reader&writer_2.c
 *
 *    Description:  说明：读写锁的使用需要gnu标准的支持，单纯的C标准将导致读写锁无法使用
 *
 *        Version:  1.0
 *        Created:  07/05/2012 03:46:16 PM
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
#include <unistd.h>
#include <pthread.h>

unsigned short buff = 0;
unsigned short reader_count = 0;
unsigned short control = 1;
pthread_rwlock_t mutex;

void* thread_writer(void *arg)
{
	puts("thread_writer created");
	while (control) {
		pthread_rwlock_wrlock(&mutex);
		printf("writer have changed the content: %d\n", ++buff);
		pthread_rwlock_unlock(&mutex);

		sleep(1);
	}

	return EXIT_SUCCESS;
}

void* thread_reader(void *arg)
{
	puts("thread_reader created");
	while (control) {
		pthread_rwlock_rdlock(&mutex);
		++reader_count;
		pthread_rwlock_unlock(&mutex);

		/* read buff */
		printf("read content: %d, %d reader(s)\n", buff, reader_count);

		pthread_rwlock_rdlock(&mutex);
		--reader_count;
		pthread_rwlock_unlock(&mutex);

		sleep(1);
	}

	return EXIT_SUCCESS;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int main(int argc, char *argv[])
{
	long i;

#define WRITER_COUNT 2
#define READER_COUNT 2

	pthread_t tid_writer[WRITER_COUNT];
	pthread_t tid_reader[READER_COUNT];

	/* initialize the mutex */
	pthread_rwlock_init(&mutex, NULL);

	/* create writer thread(s) and reader thread(s) */
	for (i=0; i<WRITER_COUNT; ++i) {
		pthread_create(&tid_writer[i], NULL, thread_writer, (long*)i);
	}
	for (i=0; i<READER_COUNT; ++i) {
		pthread_create(&tid_reader[i], NULL, thread_reader, (long*)i);
	}

	/* termainate program when press 'enter' */
	while (control) {
		if (getchar()) {
			control = 0;
		}
	}

	/* join writer thread(s) and reader thread(s) */
	for (i=0; i<WRITER_COUNT; ++i) {
		pthread_join(tid_writer[i], NULL);
	}
	for (i=0; i<READER_COUNT; ++i) {
		pthread_join(tid_reader[i], NULL);
	}

	/* destroy each mutex */
	pthread_rwlock_destroy(&mutex);

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
