/*
 * =====================================================================================
 *
 *       Filename:  foo.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/16/2013 10:10:14 AM
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

struct list_head {
	struct list_head *next;
	struct list_head *prev;
};

struct fox {
	unsigned tail_len;
	unsigned weight;
	int is_fantastic;
	int kong;
	struct list_head *list;
};
#define list_entry(ptr, type, member) container_of(ptr, type, member)

#define container_of(ptr, type, member) ({			\
		        const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
		        (type *)( (char *)__mptr - offsetof(type,member) );})

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int main(int argc, char *argv[])
{
	struct fox f,f2;
	f.tail_len = 10;
	f.weight = 20;
	f.is_fantastic = 1;
	f.list->next = &f2;

	struct fox *p;
	p = list_entry(&f.weight, struct fox, weight);

	printf("%p\n%p\nHello World!\n", &f, p);
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */


xxx库是什么库？
静态链接时，顺序有关系 从右往左
比如a依赖b 那么要把b放在a的右边
gcc -static -la -lb ...
如果
gcc -static -lb -la ...
否则就出现...unresolved symbols...


编译器从左到右扫描，需要两个表A,B
A：里面记录了所有没有被resolve的symbol
B: 里面记录了所有需要链接的库

一开始A,B两个表都是空的
从左到右扫描的时候的过程如下
1.如果是一个obj文件，则将不能被resolve的symbol加入到A表里面
2.如果是一个库文件，则查找其中是否有A表中的symbol,如果有，则从A表中去掉该symbol(因为已经被当前库resolve了），同时将该库加入到B表中

扫描结束后如果A表非空，则会报错

现在的编译器默认都是进行一次扫描的，所以你需要将.o文件放在前面，否则你这个A表直到最后才生成，但这个时候所有的库已经扫描完了会造成A表非空


具体的可能会有些不一样，但是大体原理基本就是这样的
