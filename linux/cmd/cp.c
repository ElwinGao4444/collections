/*
 * =====================================================================================
 *
 *       Filename:  cp.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/13/2012 04:49:27 AM
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
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define BUFFERSIZE 4096
#define COPYMODE 0640

void oops(char*, char*);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int main(int argc, char *argv[])
{
	int in_fd=-1, out_fd=-1, n_chars=0;
	char buf[BUFFERSIZE];

	if (argc != 3) {
		fprintf(stderr, "usage: %s source destination\n", *argv);
		exit(EXIT_FAILURE);
	}

	if ((in_fd = open(argv[1], O_RDONLY)) == -1) {
		fprintf(stderr, "Cannot open %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	if ((out_fd = creat(argv[2], COPYMODE)) == -1) {
		fprintf(stderr, "Cannot creat %s\n", argv[2]);
		exit(EXIT_FAILURE);
	}
	while ((n_chars = read(in_fd, buf, BUFFERSIZE)) > 0)
		if (write(out_fd, buf, n_chars) != n_chars) {
			fprintf(stderr, "Write error to %s\n", argv[2]);
			fprintf(stderr, "errno = %d\n", errno);
			fprintf(stderr, "%s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
	if (n_chars == -1) {
		fprintf(stderr, "Read error from %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	if (close(in_fd) == -1 || close(out_fd) == -1) {
		fprintf(stderr, "Error closing files\n");
		exit(EXIT_FAILURE);
	}
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
