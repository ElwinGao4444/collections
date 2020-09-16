/*
 * =====================================================================================
 *
 *       Filename:  tcp_client.c
 *
 *    Description:  basic tcp client demo
 *
 *        Version:  1.0
 *        Created:  06/21/2012 11:55:39 AM
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

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int main(int argc, char *argv[])
{
	int sockfd;
	struct sockaddr_in servaddr;
	char buff[BUFSIZ];
	ssize_t recvlen;

	if (argc != 2) {
		fprintf(stderr, "usage: %s <IP address>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("connect");
		exit(EXIT_FAILURE);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERVPORT);
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) {
		perror("connect");
		exit(EXIT_FAILURE);
	}

	while (fgets(buff, BUFSIZ, stdin)) {
		if (send(sockfd, buff, strlen(buff), 0) == -1) {
			perror("sent");
			exit(EXIT_FAILURE);
		}
		memset(buff, 0, sizeof(buff));
		if ((recvlen = recv(sockfd, buff, BUFSIZ, 0)) == -1) {
			perror("recv");
			exit(EXIT_FAILURE);
		}
		buff[recvlen] = '\0';
		printf("result: %s\n", buff);
	}
	close(sockfd);

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
