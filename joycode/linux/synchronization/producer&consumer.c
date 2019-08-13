/*
 * =====================================================================================
 *
 *       Filename:  producer&consumer.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/05/2012 08:58:29 AM
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
#include <semaphore.h>

#define SIZE_OF_BUFF 10
/* const unsigned short SIZE_OF_BUFF = 10; */
unsigned short buff[SIZE_OF_BUFF];
unsigned short product_id, consume_id, in, out;
unsigned short  control = 1;
pthread_mutex_t mutex;
sem_t empty, full;

void produce()
{
	printf("producing: %d ...\n", ++product_id);
	puts("succeed");
}

void append()
{
	puts("appending a product...");
	buff[in] = product_id;
	in = (in + 1) % SIZE_OF_BUFF;
	puts("succeed");
}

void take()
{
	puts("taking a product...");
	consume_id = buff[out];
	buff[out] = 0;
	out = (out + 1) % SIZE_OF_BUFF;
	puts("succeed");
}

void consume()
{
	printf("consuming: %d ...\n", consume_id);
	puts("succeed");
}

void print_status()
{
	int i = 0;
	for ( ; i<SIZE_OF_BUFF; ++i) {
		printf("%d: %d", i, buff[i]);
		if (i == in) printf("  <--produce");
		if (i == out) printf("  <--consume");
		putchar('\n');
	}
}

void* thread_producer(void *arg)
{
/*	puts("thread_producer created"); */
	while (control) {
		puts("thread_producer running");
		printf("pthread_id = %ld\n", pthread_self());
		sem_wait(&empty);
		pthread_mutex_lock(&mutex);
		produce();
		append();
		print_status();
		sleep(1);
		pthread_mutex_unlock(&mutex);
		sem_post(&full);
	}
	return EXIT_SUCCESS;
}

void* thread_consumer(void *arg)
{
/*	puts("thread_consumer created"); */
	while (control) {
		puts("thread_consumer running");
		printf("pthread_id = %ld\n", pthread_self());
		sem_wait(&full);
		pthread_mutex_lock(&mutex);
		take();
		consume();
		print_status();
		sleep(1);
		pthread_mutex_unlock(&mutex);
		sem_post(&empty);
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

#define PRODUCER_COUNT 1
#define CONSUMER_COUNT 1

	pthread_t tid_producer[PRODUCER_COUNT];
	pthread_t tid_consumer[CONSUMER_COUNT];

	/* initialize each mutex and semaphore */
	pthread_mutex_init(&mutex, NULL);
	sem_init(&empty, 0, SIZE_OF_BUFF);
	sem_init(&full, 0, 0);

	/* create producer thread(s) and consumer(s) thread */
	for (i=0; i<PRODUCER_COUNT; ++i) {
		pthread_create(&tid_producer[i], NULL, thread_producer, (long*)i);
	}
	for (i=0; i<CONSUMER_COUNT; ++i) {
		pthread_create(&tid_consumer[i], NULL, thread_consumer, (long*)i);
	}

	/* terminate program when press 'enter' */
	while (control) {
		if (getchar()) {
			control = 0;
		}
	}

	/* join producer thread(s) and consumer(s) thread */
	for (i=0; i<PRODUCER_COUNT; ++i) {
		pthread_join(tid_producer[i], NULL);
	}
	for (i=0; i<CONSUMER_COUNT; ++i) {
		pthread_join(tid_consumer[i], NULL);
	}

	/* destroy each mutex */
	pthread_mutex_destroy(&mutex);
	sem_destroy(&empty);
	sem_destroy(&full);

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
