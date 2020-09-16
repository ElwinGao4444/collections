/*
 * =====================================================================================
 *
 *       Filename:  tcp_client.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/30/2012 08:59:18 AM
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
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERV_PORT 65535
#define MAXLINE 1024

void do_cli(FILE *fp, int sockfd)
{
	char sendline[MAXLINE], recvline[MAXLINE];
	while (fgets(sendline, MAXLINE, fp) != NULL) {
		send(sockfd, sendline, strlen(sendline), 0);
		if (recv(sockfd, recvline, MAXLINE, 0) == 0) {
			fprintf(stderr, "do_cli: server terminated prematurely");
		}
		fputs(recvline, stdout);
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
	int sockfd;
	struct sockaddr_in servaddr;

	if (argc != 2) {
		fprintf(stderr, "usage: %s <IP address>\n", argv[0]);
		exit(1);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	servaddr.sin_port = htons(SERV_PORT);

	connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

	do_cli(stdin, sockfd);

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
