/*
 * =====================================================================================
 *
 *       Filename:  mcast_client.c
 *
 *    Description:  basic mcast client demo
 *
 *        Version:  1.0
 *        Created:  06/21/2012 11:56:30 AM
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
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVPORT 65535
#define MCASTADDR "224.0.0.44"
#define IFNAME "eth0"

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
	struct ip_mreq mreq;
	struct ifreq ifreq;
	socklen_t addrlen;
	ssize_t recvlen;
	char buff[BUFSIZ]; 
	char *ifname = IFNAME;

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("create socket");
		exit(EXIT_FAILURE);
	}

	memset(&cliaddr, 0, sizeof(cliaddr));
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	cliaddr.sin_port = htons(SERVPORT);

	if (bind(sockfd, (struct sockaddr*)&cliaddr, sizeof(cliaddr))) {
		perror("binding");
		exit(EXIT_FAILURE);
	}

	mreq.imr_multiaddr.s_addr = inet_addr(MCASTADDR);
	if (ifname != NULL) {
		strncpy(ifreq.ifr_name, ifname, IFNAMSIZ);
		if (ioctl(sockfd, SIOCGIFADDR, &ifreq) < 0) {
			perror("ioctl");
			exit(EXIT_FAILURE);
		}
		memcpy(&mreq.imr_interface, 
				&((struct sockaddr_in*)&ifreq.ifr_addr)->sin_addr, 
				sizeof(struct in_addr));
	} else {
		mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	}
	setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));

	while (1) {
		addrlen = sizeof(servaddr);
		memset(buff, 0, sizeof(buff));
		if ((recvlen = recvfrom(sockfd, buff, BUFSIZ, 0, (struct sockaddr*)&servaddr, &addrlen)) == -1) {
			perror("recvfrom");
			exit(EXIT_FAILURE);
		}
		printf("recv: %s\n", buff);
		if (sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
			perror("sendto");
			exit(EXIT_FAILURE);
		}
		inet_ntop(AF_INET, &servaddr.sin_addr, buff, sizeof(buff));
		printf("connection from: %s, port: %d\n", buff, ntohs(servaddr.sin_port));
	}
	close(sockfd);

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
