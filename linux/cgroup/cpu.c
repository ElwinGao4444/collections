/*
 * =====================================================================================
 *
 *       Filename:  cpu.c
 *
 *    Description:  此服务通过配置cpu.cfs_period_us与cpu.cfs_quota_us两个cgroup参数来观察
 *                  cpu的实际使用情况
 *
 *                  实验预期：
 *                  限制cpu使用率为10%，设置cpu.cfs_period_us从
 *                  最小值 1000 到最大值 1000000 范围内的波动，虽然都是10%的使用率，
 *                  但预期cpu.cfs_period_us值越大，cpu使用的分布越不均匀
 *
 *                  实验过程：
 *                  执行以下代码，模拟cpu密集型任务运行，观察cpu.stat统计信息
 *                  对 cpu.cfs_period_us 和 cpu.cfs_quota_us 设置以下3个对照组：
 *                  	1、cpu.cfs_period_us = 10000   && cpu.cfs_quota_us = 1000
 *                  	2、cpu.cfs_period_us = 100000  && cpu.cfs_quota_us = 10000
 *                  	3、cpu.cfs_period_us = 1000000 && cpu.cfs_quota_us = 100000
 *
 *                  实验数据：
 *                  实验基准：指定每次循环次数10000000次，无cgrpu限制时，耗时为0:29:522（单位：s:ms:us）
 *                  case 1：
 *                  	耗时：0:274:149
 *                  	nr_periods diff:   30
 *                  	nr_throttled diff: 28
 *                  	throttled_time:    245562148
 *                  case 2：
 *                  	耗时：0:186:838
 *                  	nr_periods diff:   4
 *                  	nr_throttled diff: 2
 *                  	throttled_time:    157251963
 *                  case 3：
 *                  	耗时：0:28:926
 *                  	nr_periods diff:   0
 *                  	nr_throttled diff: 0
 *                  	throttled_time:    0
 *
 *                  实验结论：
 *                  	cgrpu对于cpu的限制在相同受限比例的情况下会受periods的影响很大
 *                  	当periods的粒度越细，调度越公平，cpu的使用平均的分布在各个时间点
 *                  	当periods的粒度越粗，调度越不公平，cpu的使用容易出现毛刺
 *                  
 *                  实验扩展：
 *                  	本实验并研究超额使用cpu的情况，例如同时启动64个线程，在极短的时间内并发消耗
 *                  	cpu，但根据相关资料表明，一旦出现这种情况，cfs会在后期极大的限制该进程后续
 *                  	cpu的使用，造成进程饥饿
 *                  
 *                  cpu.stat信息说明：
 *                  	cpu.cfs_period_us：指定在多长的时间周期内进行cpu使用率进行控制
 *                  	cpu.cfs_quota_us：指定在单个period内可以使用多少cpu时间
 *                  	cpu.stat -> nr_periods：经过的周期
 *                  	cpu.stat -> nr_throttled：受限周期
 *                  	cpu.stat -> throttled_time：受限cpu时间(ns)
 *
 *        Version:  1.0
 *        Created:  09/25/2020 11:03:08 AM
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
#include <string.h>
#include <sys/time.h>

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int main(int argc, char *argv[])
{
	printf("pid: %d\n", getpid());

	int n = 0;
	if (argc == 2) {
		n = atoi(argv[1]);
	}

	char buf[32];
	for ( ; ; ) {
		printf("press a loop-count[default:%d]: ", n);
		gets(buf);
		if (strlen(buf) != 0) {
			n = atoi(buf);
		}

		struct timeval start, end;
		gettimeofday(&start, NULL);
		for (int i = 0 ; i < n; ++i) {
		}
		gettimeofday(&end, NULL);
		if (end.tv_sec > start.tv_sec && end.tv_usec < start.tv_usec) {
			--end.tv_sec;
			end.tv_usec += 1000000;
		}
		printf("spend time(s:ms:us): %ld:%ld:%ld\n",
				end.tv_sec - start.tv_sec,
				(end.tv_usec - start.tv_usec) / 1000,
				(end.tv_usec - start.tv_usec) % 1000);
	}

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */

