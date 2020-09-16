/*
 * =====================================================================================
 *
 *       Filename:  foo.c
 *
 *    Description:  compile with "cc -ftest-coverage -fprofile-arcs foo.c"
 *
 *        Version:  1.0
 *        Created:  02/27/2014 03:15:20 PM
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

void foo(int arg)
{
	int i;

	if (arg % 2) {
		for (i=0; i<arg; ++i) {
			if (i % 2) {
				printf("for loop %d\n", i);
			}
		}
	} else {
		puts("never run here...");
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
	foo(13);

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */

