/*
 * =====================================================================================
 *
 *       Filename:  du.c
 *
 *    Description:  该du函数相比linux的du命令还有许多欠缺，例如：
 *                  1、只能计算总值，不能以list的形式展示每个文件的size
 *                  2、没有统计“目录”的磁盘占用
 *                  3、不能指定只遍历固定的深度
 *                  4、不能以MB，GB等更好的可视化展示
 *                  不过该函数实现了du的核心功能，方便后续追加更多的功能
 *
 *        Version:  1.0
 *        Created:  09/06/2016 07:54:13 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Elwin.Gao (elwin), elwin.gao4444@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

unsigned long du(char *name)
{
	unsigned long total_size = 0;

	struct stat statbuf;
	int ret = lstat(name,&statbuf);
	if (ret != 0) {
		return total_size;
	}

	if(S_ISDIR(statbuf.st_mode)) {

		DIR *dp;
		struct dirent *entry;

		if((dp = opendir(name)) == NULL) {
			fprintf(stderr,"cannot open directory: %s\n", name);
			return total_size;
		}
		chdir(name);
		while((entry = readdir(dp)) != NULL) {
			ret = lstat(entry->d_name,&statbuf);
			if (ret != 0) {
				return total_size;
			}
			if(S_ISDIR(statbuf.st_mode)) {

				if(strcmp(".",entry->d_name) == 0 || strcmp("..",entry->d_name) == 0) {
					continue;
				}
				total_size += du(entry->d_name);
			} else {
				total_size += statbuf.st_size;  
			}
		}
		chdir("..");
		closedir(dp);
	} else {
		total_size += statbuf.st_size;  
	}

	return total_size;
}

int main(int argc, char* argv[])
{
	char *topdir, pwd[2]=".";
	if (argc != 2)
		topdir=pwd;
	else
		topdir=argv[1];

	printf("Directory size is %lu\n", du(topdir));
	return EXIT_SUCCESS;
}

