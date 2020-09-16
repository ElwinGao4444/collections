/*
 * =====================================================================================
 *
 *       Filename:  foo.c
 *
 *    Description:  关于进行地址空间的研究
 *
 *        Version:  1.0
 *        Created:  12/18/2013 09:42:57 AM
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

long g1, g2, g3;
long gi1 = 1, gi2 = 2, gi3 = 3;

void foo()
{
	long f1, f2, f3;
	void *fh1, *fh2, *fh3;
	void *fm1, *fm2, *fm3;

	fh1 = malloc(8);
	fh2 = malloc(8);
	fh3 = malloc(8);

	fm1 = malloc(1<<20);
	fm2 = malloc(1<<20);
	fm3 = malloc(1<<20);

	printf("foo:\t%p\n", (void*)foo);

	printf("f1:\t%p\n", (void*)&f1);
	printf("f2:\t%p\n", (void*)&f2);
	printf("f3:\t%p\n", (void*)&f3);

	printf("fh1:\t%p\n", fh1);
	printf("fh2:\t%p\n", fh2);
	printf("fh3:\t%p\n", fh3);

	printf("fm1:\t%p\n", fm1);
	printf("fm2:\t%p\n", fm2);
	printf("fm3:\t%p\n", fm3);

}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int main(int argc, char *argv[])
{
	long m1, m2, m3;
	void *mh1, *mh2, *mh3;
	void *mm1, *mm2, *mm3;

	mh1 = malloc(8);
	mh2 = malloc(8);
	mh3 = malloc(8);

	mm1 = malloc(1<<20);
	mm2 = malloc(1<<20);
	mm3 = malloc(1<<20);

	printf("g1:\t%p\n", (void*)&g1);
	printf("g2:\t%p\n", (void*)&g2);
	printf("g3:\t%p\n", (void*)&g3);

	printf("gi1:\t%p\n", (void*)&gi1);
	printf("gi2:\t%p\n", (void*)&gi2);
	printf("gi3:\t%p\n", (void*)&gi3);

	puts("--------------------------------");

	printf("main:\t%p\n", (void*)main);

	printf("m1:\t%p\n", (void*)&m1);
	printf("m2:\t%p\n", (void*)&m2);
	printf("m3:\t%p\n", (void*)&m3);

	printf("mh1:\t%p\n", mh1);
	printf("mh2:\t%p\n", mh2);
	printf("mh3:\t%p\n", mh3);

	printf("mm1:\t%p\n", mm1);
	printf("mm2:\t%p\n", mm2);
	printf("mm3:\t%p\n", mm3);

	puts("--------------------------------");

	foo();

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
