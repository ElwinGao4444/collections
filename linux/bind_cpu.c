/*
 * =====================================================================================
 *
 *       Filename:  bind_cpu.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/18/2016 04:36:37 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Elwin.Gao (elwin), elwin.gao4444@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sched.h>

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  dummy_run
 *  Description:   
 * =====================================================================================
 */
void dummy_run()
{
	int n = 1000*1000;
	int i = 0;
	for ( ; i < n; ++i) {
		int c = i * i / n;
	}
	sleep(1);
}		/* -----  end of function dummy_run  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int main(int argc, char *argv[])
{
	cpu_set_t mask;
	int core_num = sysconf(_SC_NPROCESSORS_CONF);	//获取当前的cpu总数  
	printf("core num: %d\n", core_num);

	printf("pid: %d\n", getpid());
	for ( ; ; ) {
		int i = 0;
		for ( ; i < core_num; ++i) {
			CPU_ZERO(&mask);	// mask初始化为空
			CPU_SET(i, &mask);	// mask设置CPU集合
			if (sched_setaffinity(0, sizeof(mask), &mask) < 0) {
				perror("sched_setaffinity");
			}
			printf("using core: %d\n", i);
			dummy_run();
		}
	}

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */

