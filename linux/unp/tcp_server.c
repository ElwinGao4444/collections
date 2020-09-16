/*
 * =====================================================================================
 *
 *       Filename:  tcp_server.c
 *
 *    Description:  basic tcp server demo
 *
 *        Version:  1.0
 *        Created:  06/21/2012 11:55:31 AM
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
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#define SERVPORT 65535
#define LISTENQ 7

void do_echo(int sockfd)
{
	ssize_t n;
	char line[BUFSIZ];

	while (1) {
		if ((n = recv(sockfd, line, BUFSIZ, 0)) == 0) {
			return ;
		}
		send(sockfd, line, n, 0);
	}
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int main(int argc, char *argv[])
{
	int sockfd, connfd;
	struct sockaddr_in servaddr, cliaddr;
	struct timeval timeo = {5, 0};
	socklen_t addrlen;
	ssize_t recvlen;
	char buff[BUFSIZ];

	signal(SIGPIPE, SIG_IGN);

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0))  == -1) {
		perror("create socket");
		exit(EXIT_FAILURE);
	}

	setsockopt(connfd, SOL_SOCKET, SO_SNDTIMEO, &timeo, sizeof(timeo));                           
	setsockopt(connfd, SOL_SOCKET, SO_RCVTIMEO, &timeo, sizeof(timeo));

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERVPORT);

	if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
		perror("bind");
		exit(EXIT_FAILURE);
	}

	if (listen(sockfd, LISTENQ) == -1) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	while (1) {
		addrlen = sizeof(cliaddr);
		if ((connfd = accept(sockfd, (struct sockaddr*)&cliaddr, &addrlen)) == -1) {
			perror("accept");
			exit(EXIT_SUCCESS);
		}
		inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff));
		printf("connection from: %s, port: %d\n", buff, ntohs(cliaddr.sin_port));
//		do_echo(connfd);
		while (1) {
			memset(buff, 0, sizeof(buff));
			if ((recvlen = recv(connfd, buff, BUFSIZ, 0)) == 0) {
				perror("recv quit");
				break;
			}
			printf("recvlen = %lu\n", recvlen);
			buff[recvlen-1] = '\0';
			printf("recv: %s\n", buff);
			if (strcmp(buff, "quit") == 0) {
				printf("quit from command\n");
				break;
			}
			if (send(connfd, buff, recvlen, 0) == -1) {
				perror("send");
				break;
			}
		}
		close(connfd);
	}
	close(sockfd);

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
