/*
 * =====================================================================================
 *
 *       Filename:  tcp_server.c
 *
 *    Description:  tcp server demo with select mechanism, there is a bug when client
 *    			quit, but why???? and it's just a demo, a stupid demo!!!!
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

#define SERVPORT 65535
#define LISTENQ 7
#define N 4

void do_echo(int connfd)
{
	ssize_t recvlen;
	char buff[BUFSIZ];

//	while (1) {
	memset(buff, 0, sizeof(buff));
	if ((recvlen = recv(connfd, buff, BUFSIZ, 0)) == 0) {
		perror("recv quit");
//		break;
	}
	printf("recvlen = %lu\n", recvlen);
	buff[recvlen-1] = '\0';
	printf("recv: %s\n", buff);
	if (strcmp(buff, "quit") == 0) {
		printf("quit from command\n");
//		break;
	}
	if (send(connfd, buff, recvlen, 0) == -1) {
		perror("send");
//		break;
	}
//	}
//	close(connfd);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int main(int argc, char *argv[])
{
	int sockfd, connfd[N] = {0};
	struct sockaddr_in servaddr, cliaddr;
	struct timeval timeo = {5, 0};
	socklen_t addrlen;
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

	int i = 0;
	int n = 0;
	int ret;
	fd_set rdfds;
	int maxsocket = 0;
	struct timeval tv;
	while (1) {
		FD_ZERO(&rdfds);
		FD_SET(sockfd, &rdfds);
		for (i=0; i<n; ++i) {
			FD_SET(connfd[i], &rdfds);
		}

		tv.tv_sec = 30;
		tv.tv_usec = 0;

		maxsocket = maxsocket > sockfd ? maxsocket : sockfd;
		ret = select(maxsocket + 1, &rdfds, NULL, NULL, &tv);
		if (ret < 0) {
			perror("select");
			break;
		} else if (ret == 0) {
			puts("timeout");
			continue;
		}

		for (i=0; i<n; ++i) {
			if (FD_ISSET(connfd[i], &rdfds)) {
				do_echo(connfd[i]);
			}
		}

		if (FD_ISSET(sockfd, &rdfds)) {
			if (n == N) {
				puts("too much connfd...");
				exit(EXIT_FAILURE);
			}
			
			addrlen = sizeof(cliaddr);
			if ((connfd[n] = accept(sockfd, (struct sockaddr*)&cliaddr, &addrlen)) == -1) {
				perror("accept");
				exit(EXIT_SUCCESS);
			}
			FD_SET(connfd[n], &rdfds);
			maxsocket = maxsocket > connfd[n] ? maxsocket : connfd[n];
			++n;
			inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff));
			printf("connection from: %s, port: %d\n", buff, ntohs(cliaddr.sin_port));
		}
	}
	close(sockfd);

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
