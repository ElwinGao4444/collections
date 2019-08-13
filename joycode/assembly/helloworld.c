/*
 * =====================================================================================
 *
 *       Filename:  foo.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/04/2015 04:05:34 PM
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

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int main(int argc, char *argv[])
{
	char const * MESSAGE = "hello world\n";  
	char const * MESSAGE2 = "hello word\n";  
	asm volatile ( "movl $4, %%eax;"  
			"movl $1, %%ebx;"  
			"movl %0, %%ecx;"  
			"movl $12 , %%edx;"  
			"int $0x80;"  
			: "=m"   (MESSAGE)  
			: "m"   (MESSAGE2)  
			: );  
	//printf("hello\n");  
	return 0; 

	printf("Hello World!\n");
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
