/*
 * =====================================================================================
 *
 *       Filename:  more.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/12/2012 11:20:44 PM
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

#define PAGELEN 24
#define LINELEN 512

void do_more(FILE*);
int see_more();

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int main(int argc, char *argv[])
{
	FILE *fp;
	if (argc == 1) {
	/* 	fprintf(stderr, "usage: more [-dflpcsu] [+linenum | +/pattern] name1 name2 ...\n");	*/
		do_more(stdin);
	} else {
		while (argc--)
			if ((fp = fopen(*++argv, "r")) != NULL) {
				do_more(fp);
				fclose(fp);
			} else {
				exit(1);
			}

		if ((fp = fopen(*++argv, "r")) != NULL) {
			do_more(fp);
			fclose(fp);
		} else {
			exit(1);
		}
	}				/* ----------  end of function main  ---------- */
	return EXIT_SUCCESS;
}

void do_more(FILE *fp)
{
	char line[LINELEN];
	int num_of_lines = 0;
	int see_more(FILE*), reply;
	FILE *fp_tty;

	if ((fp_tty = fopen("/dev/tty", "r")) == NULL) {
		puts("tty can't open");
		exit(1);
	}

	while (fgets(line, LINELEN, fp)) {
		if (num_of_lines == PAGELEN) {
			reply = see_more(fp_tty);
			if (reply == 0)
				break;
			num_of_lines -= reply;
		}
		if (fputs(line, stdout) == EOF)
			exit(1);
		num_of_lines++;
	}
}

int see_more(FILE *cmd)
{
	int c;
	printf("\033[7m--MORE--\033[m");
	while ((c = getc(cmd)) != EOF) {
		if (c == 'q')
			return 0;
		if (c == ' ')
			return PAGELEN;
		if (c == '\n')
			return 1;
	}
	return 0;
}
