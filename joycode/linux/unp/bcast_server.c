/*
 * =====================================================================================
 *
 *       Filename:  bcast_server.c
 *
 *    Description:  basic bcast server demo(notice: firewall will prevent return-date 
 *    			from broadcast target)
 *
 *        Version:  1.0
 *        Created:  06/21/2012 11:56:20 AM
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
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVPORT 65535

int sockfd;

void* thread_recv(void *arg)
{
	char buff[BUFSIZ];
	ssize_t recvlen;

	puts("thread_recv created...");
	while ((recvlen = recvfrom(sockfd, buff, BUFSIZ, 0, NULL, NULL)) != -1) {
		printf("recv: %s\n", buff);
	}
	perror("recvfrom");
	exit(EXIT_FAILURE);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int main(int argc, char *argv[])
{
	int sockfd_cp, on = 1;
	struct sockaddr_in cliaddr;
//	socklen_t addrlen;
	pthread_t tid;
	
	if (argc != 2) {
		fprintf(stderr, "usage: %s <IP address>\n", argv[0]);
		exit(EXIT_FAILURE);
	}   

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("create socket");
		exit(EXIT_FAILURE);
	}

	memset(&cliaddr, 0, sizeof(cliaddr));
	cliaddr.sin_family = AF_INET;
	inet_pton(AF_INET, argv[1], &cliaddr.sin_addr);
	cliaddr.sin_port = htons(SERVPORT);

	setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));

	pthread_create(&tid, NULL, thread_recv, NULL);
//	pthread_join(tid, NULL);

	int num = 0;
	char str[16];
	while (1) {
		memset(str, 0, sizeof(str));
		sprintf(str, "%d", num++);
		printf("send: %s\n", str);
		if (sendto(sockfd, str, strlen(str), 0, (struct sockaddr*)&cliaddr, sizeof(cliaddr)) == -1) {
			perror("sendto");
			exit(EXIT_FAILURE);
		}
		sleep(1);
	}
	close(sockfd);

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
