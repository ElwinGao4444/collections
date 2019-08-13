/*
 * =====================================================================================
 *
 *       Filename:  udp_client.c
 *
 *    Description:  basic udp client demo, change data size and send this data as quick as possiable
 *
 *        Version:  2.0
 *        Created:  06/21/2012 11:56:05 AM
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
#define N 1000

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
//	socklen_t addrlen;
	ssize_t recvlen;
	char buff[BUFSIZ];

	strcpy(buff, "-"
//			"----------------------------------------------------------------------------------------------------"
//			"----------------------------------------------------------------------------------------------------"
//			"----------------------------------------------------------------------------------------------------"
//			"----------------------------------------------------------------------------------------------------"
//			"----------------------------------------------------------------------------------------------------"
//			"----------------------------------------------------------------------------------------------------"
//			"----------------------------------------------------------------------------------------------------"
//			"----------------------------------------------------------------------------------------------------"
//			"----------------------------------------------------------------------------------------------------"
//			"----------------------------------------------------------------------------------------------------"
	      		);

	if (argc != 2) {
		fprintf(stderr, "usage: %s <IP address>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("create socket");
		exit(EXIT_FAILURE);
	}

//	int buffsize = 1 * 1024;
//	if (setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &buffsize, sizeof(buffsize)) == -1) {
//		perror("set socket option");
//		exit(EXIT_FAILURE);
//	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	servaddr.sin_port = htons(SERVPORT);

	int i = 0;
	while (i++ != N) {
		if (sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
			perror("sendto");
			exit(EXIT_FAILURE);
		}
	//		memset(buff, 0, sizeof(buff));
	//		if ((recvlen = recvfrom(sockfd, buff, BUFSIZ, 0, NULL, NULL)) == -1) {
	//			perror("recvfrom");
	//			exit(EXIT_FAILURE);
	//		}
	//		printf("return back: %s\n", buff);
	}
	close(sockfd);

	return(EXIT_SUCCESS);
}				/* ----------  end of function main  ---------- */
