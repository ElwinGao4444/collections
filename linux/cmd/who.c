/*
 * =====================================================================================
 *
 *       Filename:  who.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/13/2012 04:13:00 AM
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
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define SHOWHOST                                /* include remote machine on output */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int main(int argc, char *argv[])
{
	struct utmp current_record;
	int utmpfd;
	int reclen = sizeof(current_record);
	void show_info(struct utmp *utbufp);

	if ((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1) {
		perror(UTMP_FILE);
		exit(1);
	}

	while (read(utmpfd, &current_record, reclen) == reclen)
		show_info(&current_record);
	close(utmpfd);
	return 0;
	printf("Hello World!\n");
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */

void show_info(struct utmp *utbufp)
{
	void showtime(long);
	if (utbufp->ut_type != USER_PROCESS)
		return;

	printf("%-8.8s", utbufp->ut_user);
	printf(" ");
	printf("%-8.8s", utbufp->ut_line);
	printf(" ");
/*	printf("%10ld", utbufp->ut_tv.tv_sec);	*/
	showtime(utbufp->ut_tv.tv_sec);
	printf(" ");
#ifdef SHOWHOST
	printf("(%s)", utbufp->ut_host);
#endif
	printf("\n");
}

void showtime(long timeval)
{
	char *cp;
	cp = ctime(&timeval);
	printf("%12.12s", cp+4);
}
