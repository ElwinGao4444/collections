/*
 * =====================================================================================
 *
 *       Filename:  get_host_ip.c
 *
 *    Description:  获取本机IP的三种方法
 *
 *        Version:  1.0
 *        Created:  01/13/2017 08:19:42 PM
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
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/types.h>
#include <ifaddrs.h>
#include <string.h> 

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  fun1
 *  Description:  通过gethostname()和gethostbyname()获取hostname和ip
 * =====================================================================================
 */
void fun1()
{
	char hname[128];
	struct hostent *hent;
	int i;

	gethostname(hname, sizeof(hname));

	//hent = gethostent();
	hent = gethostbyname(hname);

	printf("hostname: %s\naddress list:\n", hent->h_name);
	for(i = 0; hent->h_addr_list[i]; i++) {
		printf("%s\n", inet_ntoa(*(struct in_addr*)(hent->h_addr_list[i])));
	}
}		/* -----  end of function fun1  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  fun2
 *  Description:  通过netdevice枚举网卡
 * =====================================================================================
 */
void fun2()
{
	struct ifaddrs * ifAddrStruct=NULL;
	void * tmpAddrPtr=NULL;

	getifaddrs(&ifAddrStruct);

	while (ifAddrStruct!=NULL) {
		if (ifAddrStruct->ifa_addr->sa_family==AF_INET) { // check it is IP4
			// is a valid IP4 Address
			tmpAddrPtr=&((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
			char addressBuffer[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
			printf("%s IP Address %s\n", ifAddrStruct->ifa_name, addressBuffer); 
		} else if (ifAddrStruct->ifa_addr->sa_family==AF_INET6) { // check it is IP6
			// is a valid IP6 Address
			tmpAddrPtr=&((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
			char addressBuffer[INET6_ADDRSTRLEN];
			inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
			printf("%s IP Address %s\n", ifAddrStruct->ifa_name, addressBuffer); 
		} 
		ifAddrStruct=ifAddrStruct->ifa_next;
	}
	return;
}		/* -----  end of function fun2  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  fun3
 *  Description:  打开一个对外界服务器的网络连接，通过getsockname()反查自己的IP
 * =====================================================================================
 */
void fun3()
{
	struct hostent *host;
	int i,sock,err;
	const char *p;
	struct sockaddr_in serv;
	struct sockaddr_in local;
	const char *pLogServer = "baidu.com";
	socklen_t local_len = sizeof(local);

	if ((host = gethostbyname(pLogServer)) == NULL) {
		return;
	}

	for (i = 0; host->h_addr_list[i]; i++) {
		sock = socket(AF_INET, SOCK_STREAM, 0);

		if (sock < 0) continue;

		memset(&serv, 0, sizeof(serv));
		serv.sin_family = AF_INET;
		serv.sin_addr = *((struct in_addr *)host->h_addr_list[i]);
		serv.sin_port = htons(80);

		err = connect(sock, (struct sockaddr*)&serv, sizeof(serv));

		if (err < 0) {
			close(sock);
			continue;
		}

		err = getsockname(sock, (struct sockaddr*)&local, &local_len);

		if (err < 0) {
			close(sock);
			continue;
		}

		char szBuf[BUFSIZ];
		size_t nLen = BUFSIZ;
		p = inet_ntop(AF_INET, &local.sin_addr, szBuf, nLen);

		if (!p) {
			close(sock);
			continue;
		}

		close(sock);

		printf("IP: %s\n", szBuf); 

		return;
	}
}		/* -----  end of function fun3  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int main(int argc, char *argv[])
{
	fun1();
	putchar('\n');
	fun2();
	putchar('\n');
	fun3();
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */

