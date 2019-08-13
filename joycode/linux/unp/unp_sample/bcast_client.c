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
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERV_PORT 65535
#define MAXLINE 1024

static void recvfrom_alarm(int signo)
{
	return;                                 /* just interrupt the recvfrom */
}

void do_cli(FILE *fp, int sockfd, const	struct sockaddr *pservaddr, socklen_t servlen)
{
	int n;
	char sendline[MAXLINE], recvline[MAXLINE + 1];
	const int on = 1;

	setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
	signal(SIGALRM, recvfrom_alarm);

	while (fgets(sendline, MAXLINE, fp) != NULL) {
		socklen_t len;
		struct sockaddr_in reply_addr;
		sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
//		alarm(1);
		while (1) {
			len = servlen;
			n = recvfrom(sockfd, recvline, MAXLINE, 0, (struct sockaddr*)&reply_addr, &len);
			if (n < 0) {
				if (errno == EINTR) {
					break;
				} else {
					fprintf(stderr, "recvfrom error\n");
				}
			} else {
				recvline[n] = 0;
				char tmp[128];
				inet_ntop(AF_INET, (struct sockaddr*)&reply_addr.sin_addr, tmp, sizeof(tmp));
				printf("from %s: %s", tmp, recvline);
			}
			recvline[n] = 0;
			fputs(recvline, stdout);
		}
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
