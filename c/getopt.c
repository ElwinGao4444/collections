/*
 * =====================================================================================
 *
 *       Filename:  getopt.c
 *
 *    Description:  getopt函数的使用方法
 *
 *                  getopt只处理“-”开头的参数
 *
 *                  1、单个字符，表示选项
 *                  2、单个字符+一个冒号，后面必须跟一个参数，可以有空格也可以没有
 *                  3、单个字符+两个冒号，后面可以跟一个参数，不能加空格
 *
 *                  如果为找到符合的参数，则getopt会返回“?”
 *
 *        Version:  1.0
 *        Created:  12/30/2014 04:00:36 PM
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

static const char *option = "a:b::cde";

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  usage
 *  Description:   
 * =====================================================================================
 */
void usage()
{
	printf("Usage:\n");
}		/* -----  end of function usage  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int main(int argc, char *argv[])
{
	int opt;
	opterr=0;
	while ((opt = getopt(argc, argv, option)) != -1) {
		switch (opt) {
		case 'a' :
		case 'b' :
			printf("opt: %c: %s\n", opt, optarg);
			break;
		case '?' :
			usage();
			exit(-1);
		default :
			printf("%c: %s\n", opt, optarg);
			break;
		}
	}

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */

