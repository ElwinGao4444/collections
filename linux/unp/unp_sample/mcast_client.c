/*
 * =====================================================================================
 *
 *       Filename:  client.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/29/2012 04:31:28 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Elwin.Gao (elwin), elwin.gao4444@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>

#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>

#define  LOCAL_PORT      5350
#define  MCAST_ADDR      "224.0.0.100"
#define  MCAST_INTERVAL    5
#define  LOCAL_ADDR   "192.168.1.256"

/*
   static void mdns_mcast_group(struct sockaddr_in *mcast_addr){
   mcast_addr->sin_family = AF_INET;
   mcast_addr->sin_port = htons(MCAST_PORT);
   mcast_addr->sin_addr.s_addr = inet_addr(MCAST_ADDR);
   }

   int mdns_open_socket(void){
   struct ip_mreq mreq;
   struct sockaddr_in sa;
   int fd =-1,ttl,yes;//yes地址重用

   mdns_mcast_group(&sa);

   if((fd = socket(AF_INET,SOCK_DGRAG,0)) < 0){
   printf("socket() failed: %s\n",strerror(errno));
   }
   }*/

int main()
{
	int sock_fd;
	struct sockaddr_in local_addr;
	int err = -1;
	int loop = 1;
	sock_fd = socket(AF_INET,SOCK_DGRAM,0);/* 建立套接字*/
	if(sock_fd < 0){
		perror("socket()");
		exit(1);
	} else {
		printf("socket() sucessful\n");
	}

	memset(&local_addr,0,sizeof(local_addr));
	local_addr.sin_family = AF_INET;
	local_addr.sin_addr.s_addr = htonl(INADDR_ANY);//inet_addr(LOCAL_ADDR);htonl(INADDR_ANY);固定为local_addr则接受不到
	local_addr.sin_port = htons(LOCAL_PORT);
	err = bind(sock_fd,(struct sockaddr *)&local_addr,sizeof(local_addr));
	if(err < 0){
		perror("bind");
		exit(1);
	} else {
		printf("bind sucessful\n");
	}
	loop = 1;
	err = setsockopt(sock_fd,IPPROTO_IP,IP_MULTICAST_LOOP,&loop,sizeof(loop));
	if(err < 0){
		perror("setsocket():IP MULTICAST_LOOP");
		exit(1);
	} else {
		printf("IP_MULTICAST_LOOP SUCESSFUL\n");
	}

	struct ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = inet_addr(MCAST_ADDR);
	mreq.imr_interface.s_addr = inet_addr(LOCAL_ADDR);
	err = setsockopt(sock_fd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq));//
	if(err < 0){
		printf("%s setsockopt():IP ADD MEMBURSHIP\n",strerror(errno));
		exit(1);
	} else {
		printf("setsockopt() IP ADD MEMBURSHIP sucessful\n");
	}
	int  times = 0;
	int  addr_len = 0;
	char buff[1000];
	int  recv_num  =0;
	for(times = 0;times < 5;times++){
		addr_len = sizeof(local_addr);
		memset(buff,0,sizeof(buff));
		recv_num = recvfrom(sock_fd,buff,sizeof(buff),0,(struct sockaddr*)&local_addr,&addr_len);
		if(recv_num < 0){

			perror("recvfrom()");
			exit(1);
		} else {
			printf("Recv %dst message from server :%s\n",times,buff);
			sleep(MCAST_INTERVAL);
		}
	}
	err = setsockopt(sock_fd,IPPROTO_IP,IP_DROP_MEMBERSHIP,&mreq,sizeof(mreq));
	close(sock_fd);
	return 0;
}
