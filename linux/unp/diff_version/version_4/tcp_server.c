/*
 * =====================================================================================
 *
 *       Filename:  tcp_server.c
 *
 *    Description:  tcp server demo with epoll mechanism, there is a bug that it can not
 *    			support two client interacting in alternative mode, one will be
 *    			block. this is on purpose to compare with select way of tcp demo
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
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVPORT 65535
#define LISTENQ 7
#define N 4

void do_echo(int connfd)
{
	ssize_t recvlen;
	char buff[BUFSIZ];

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

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int main(int argc, char *argv[])
{
	int sockfd;
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

	int connfd, epfd, nfds;
	struct epoll_event ev, events[20];
	epfd = epoll_create(256);
	ev.data.fd = sockfd;
	ev.events = EPOLLIN | EPOLLET;
	epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);
	while (1) {
		nfds = epoll_wait(epfd, events, 20, 500);
		int i;
		for (i=0; i<nfds; ++i) {
			if (events[i].data.fd == sockfd) {
				addrlen = sizeof(cliaddr);
				if ((connfd = accept(sockfd, (struct sockaddr*)&cliaddr, &addrlen)) == -1) {
					perror("accept");
					exit(EXIT_SUCCESS);
				}
				inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff));
				printf("connection from: %s, port: %d\n", buff, ntohs(cliaddr.sin_port));
				ev.data.fd = connfd;
				ev.events = EPOLLIN | EPOLLET;
				epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);
			} else if (events[i].events & EPOLLIN) {
				connfd = events[i].data.fd;
				do_echo(connfd);
			}
		}

	}
	close(sockfd);

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
