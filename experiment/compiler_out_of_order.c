/*
 * =====================================================================================
 *
 *       Filename:  compiler_out_of_order.c
 *
 *    Description:  the code of assembly is out of order when using -O2
 *                  using "gcc -S -O2 compiler_out_of_order.c" see compiler_out_of_order.s
 *
 *        Version:  1.0
 *        Created:  07/14/2013 06:01:03 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Elwin.Gao (elwin), elwin.gao4444@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int main(int argc, char *argv[])
{
	int a = 0;
	int b = 0;

	a = argc;
	a *= 7 * 13;
	a *= (argc+16);

	b += 99;

	printf("%d%d\n", a, b);

	return 0;
}				/* ----------  end of function main  ---------- */
