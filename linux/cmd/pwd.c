/*
 * =====================================================================================
 *
 *       Filename:  pwd.c
 *
 *    Description: 
 *
 *        Version:  1.0
 *        Created:  06/19/2012 11:17:38 AM
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
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

ino_t get_inode(char *);
void printpathto(ino_t);
void inum_to_name(ino_t, char *, int);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int main(int argc, char *argv[])
{
	printpathto(get_inode("."));	/* print path to here */
	putchar('\n');
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */

void printpathto(ino_t this_inode)
{
	ino_t my_inode;
	char its_name[BUFSIZ];
	if (get_inode("..") != this_inode) {
		chdir("..");
		inum_to_name(this_inode, its_name, BUFSIZ);
		my_inode = get_inode(".");
		printpathto(my_inode);
		printf("/%s", its_name);
	}
}

void inum_to_name(ino_t inode_to_find, char *namebuf, int buflen)
{
	DIR *dir_ptr;
	struct dirent *direntp;
	dir_ptr = opendir(".");
	if (dir_ptr == NULL) {
		perror(".");
		exit(EXIT_FAILURE);
	}

	while ((direntp = readdir(dir_ptr)) != NULL)
		if (direntp->d_ino == inode_to_find) {
			strncpy(namebuf, direntp->d_name, buflen);
			namebuf[buflen - 1] = '\0';
			closedir(dir_ptr);
			return;
		}
}

ino_t get_inode(char *fname)
{
	struct stat info;
	if (stat(fname, &info) == -1) {
		fprintf(stderr, "Cannot stat");
		perror(fname);
		exit(1);
	}
	return info.st_ino;
}
