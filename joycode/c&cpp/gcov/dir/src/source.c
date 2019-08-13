/*
 * =====================================================================================
 *
 *       Filename:  source.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/27/2014 03:25:26 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Elwin.Gao (elwin), elwin.gao4444@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

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

