/*
 * =====================================================================================
 *
 *       Filename:  mcast_server.c
 *
 *    Description:  basic mcast server demo
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
#define MCASTADDR "224.0.0.44"

void* thread_recv(void *arg)
{
	int *psockfd = arg;
	char buff[BUFSIZ];
	ssize_t recvlen;

	puts("thread_recv created...");
	while ((recvlen = recvfrom(*psockfd, buff, BUFSIZ, 0, NULL, NULL)) != -1) {
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
	int sockfd;
	struct sockaddr_in cliaddr;
//      socklen_t addrlen;
	pthread_t tid;

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("create socket");
		exit(EXIT_FAILURE);
	}

	memset(&cliaddr, 0, sizeof(cliaddr));
	cliaddr.sin_family = AF_INET;
	inet_pton(AF_INET, MCASTADDR, &cliaddr.sin_addr);
	cliaddr.sin_port = htons(SERVPORT);

//	setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));

	pthread_create(&tid, NULL, thread_recv, &sockfd);
//      pthread_join(tid, NULL);

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
