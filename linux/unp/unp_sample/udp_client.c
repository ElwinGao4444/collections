/*
 * =====================================================================================
 *
 *       Filename:  udp_client.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/29/2012 05:07:08 PM
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

void do_cli(FILE *fp, int sockfd, const	struct sockaddr *pservaddr, socklen_t servlen)
{
	int n;
	char sendline[MAXLINE], recvline[MAXLINE + 1];

	while (fgets(sendline, MAXLINE, fp) != NULL) {
		sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
		n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);
		recvline[n] = 0;
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
		fprintf(stderr, "usage %s <IP address>\n", argv[0]);
		exit(1);
	}

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	servaddr.sin_port = htons(SERV_PORT);

	do_cli(stdin, sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
