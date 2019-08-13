/*
 * =====================================================================================
 *
 *       Filename:  foo.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/27/2014 04:10:44 PM
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

void b()
{
	int i;
	for (i=0; i<100; ++i) {
		printf("b(): %d\n", i);
	}
}

void a_b()
{
	int i;
	for (i=0; i<100; ++i) {
		printf("a_b(): %d\n", i);
		b();
	}
}

void c_b()
{
	int i;
	for (i=0; i<100; ++i) {
		printf("c(): %d\n", i);
		b();
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
	a_b();
	c_b();
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */

