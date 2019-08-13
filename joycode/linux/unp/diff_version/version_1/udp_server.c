/*
 * =====================================================================================
 *
 *       Filename:  udp_server.c
 *
 *    Description:  basic udp client demo
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
	struct sockaddr_in servaddr, cliaddr;
	socklen_t addrlen;
	ssize_t recvlen;
	char buff[BUFSIZ];

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("create socket");
		exit(EXIT_FAILURE);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERVPORT);

	if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
		perror("binding");
		exit(EXIT_FAILURE);
	}

	while (1) {
		addrlen = sizeof(cliaddr);
		memset(buff, 0, sizeof(buff));
		if ((recvlen = recvfrom(sockfd, buff, BUFSIZ, 0, (struct sockaddr*)&cliaddr, &addrlen)) == -1) {
			perror("recvfrom");
			exit(EXIT_FAILURE);
		}
		buff[--recvlen] = '\0';
		printf("recv: %s\n", buff);
		if (sendto(sockfd, buff, recvlen, 0, (struct sockaddr*)&cliaddr, sizeof(cliaddr)) == -1) {
			perror("sendto");
			exit(EXIT_FAILURE);
		}
		inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff));
		printf("connection from: %s, port: %d\n", buff, ntohs(cliaddr.sin_port));
	}
	close(sockfd);

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
