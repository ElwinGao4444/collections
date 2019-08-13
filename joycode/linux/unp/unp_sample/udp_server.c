/*
 * =====================================================================================
 *
 *       Filename:  udp_server.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/29/2012 04:50:44 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Elwin.Gao (elwin), elwin.gao4444@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define SERV_PORT 65535
#define MAXLINE 1024

void do_echo(int sockfd, struct sockaddr* pcliaddr, socklen_t clilen)
{
	int n;
	socklen_t len;
	char mesg[MAXLINE];

	while (1) {
		len = clilen;
		n = recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len);
		sendto(sockfd, mesg, n, 0, pcliaddr, len);
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
	struct sockaddr_in servaddr, cliaddr;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

	do_echo(sockfd, (struct sockaddr*)&cliaddr, sizeof(cliaddr));

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
