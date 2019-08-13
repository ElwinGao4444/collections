/*
 * =====================================================================================
 *
 *       Filename:  connect_timeout.c
 *
 *    Description:  设置阻塞模式下的connect超时时间
 *                  用法：cc -o client connect_timeout.c
 *                        ./client IP PORT [time(s)] （可以用网站IP作为测试IP）
 *
 *        Version:  1.0
 *        Created:  03/08/2016 04:45:11 PM
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
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>


int main(int argc, char *argv[])
{
    int fd;
    struct sockaddr_in addr;
    struct timeval timeo = {3, 0};
    socklen_t len = sizeof(timeo);

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (argc == 4)
        timeo.tv_sec = atoi(argv[3]);
    setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &timeo, len);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));
    if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        fprintf(stderr, "errno: %d\n", errno);
        perror("errmsg");
        if (errno == EINPROGRESS) {
            fprintf(stderr, "timeout\n");
            return -1;
        }
        perror("connect");
        return 0;
    }
    printf("connected\n");

    return 0;
}

