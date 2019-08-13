/*
 * =====================================================================================
 *
 *       Filename:  udp_server.c
 *
 *    Description:  basic udp client demo, receive through multi-thread
 *
 *        Version:  1.0
 *        Created:  06/21/2012 11:55:49 AM
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
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVPORT 65535
#define N 2
#define M 10000

int sockfd = 0;
int count[N] = {0};
int saved[M] = {0};

void sig_handler(int signo)
{
	int i;
	if (signo == SIGALRM) {
		for (i=0; i<N; ++i) {
 /* :BUG:07/10/2012 03:23:40 PM:elwin: it's possiable to receive less package than it should be in multiple thread */
			if (count[i] != 0) {
				printf("count[%d] = %d\n", i, count[i]);
				count[i] = 0;
			} else {
				int j, c = 0;
				for (j=0; j<M; ++j)
					if (saved[j] == 0)
						++c;
				if (c < M) {
					printf("%d data(s) have lost\n", c);
					memset(saved, 0, sizeof(saved));
				}
			}
		}
		alarm(1);
	}
}

void* thread_recv(void *arg)
{
	long index = (long) arg;
	int num;
	struct sockaddr_in cliaddr;
	socklen_t addrlen;
	ssize_t recvlen;
	char buff[BUFSIZ];

	printf("thread %ld created\n", index);
	while (1) {
		addrlen = sizeof(cliaddr);
		memset(buff, 0, sizeof(buff));
		if ((recvlen = recvfrom(sockfd, buff, BUFSIZ, 0, (struct sockaddr*)&cliaddr, &addrlen)) == -1) {
			perror("recvfrom");
			exit(EXIT_FAILURE);
		}
		num = atoi(buff);
		if (num < M)
			saved[num] = 1;
		else
			printf("'saved' array overflowed, data: '%d' is too large\n", num);
		++count[index];
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
	struct sockaddr_in servaddr;
	pthread_t tid[N];

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("create socket");
		exit(EXIT_FAILURE);
	}
	
//	int buffsize = 128 * 1024;
//	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &buffsize, sizeof(buffsize)) == -1) {
//		perror("set socket option");
//		exit(EXIT_FAILURE);
//	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERVPORT);

	if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
		perror("binding");
		exit(EXIT_FAILURE);
	}
//	sleep(3);
//	puts("sleep over");

	long i;
	for (i=0; i<N; ++i) {
		pthread_create(&tid[i], NULL, thread_recv, (void*)i);
	}

	signal(SIGALRM, sig_handler);
	alarm(1);

	for (i=0; i<N; ++i) {
		pthread_join(tid[i], NULL);
	}

	close(sockfd);

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
