/*
// =====================================================================================
// 
//       Filename:  list.cpp
// 
//    Description:  list是一种链表结构
// 
//        Version:  1.0
//        Created:  09/01/2017 04:07:09 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Elwin.Gao (elwin), elwin.gao4444@gmail.com
//        Company:  
// 
// =====================================================================================
*/

#include <assert.h>

#include <cstdlib>
#include <iostream>

#include <list>

bool check(const int &value) {
	return value < 5 ? true : false;
}

bool int_equal(const int &x, const int &y) {
	return x == y;
}

bool int_compare(const int &x, const int &y) {
	return x < y;
}

/* 
// ===  FUNCTION  ======================================================================
//         Name:  main
//  Description:  
// =====================================================================================
*/
int main(int argc, char *argv[])
{
	std::list<int> list;

	std::cout << "list.size(): " << list.size() << std::endl;
	std::cout << "list.max_size(): "<< list.max_size() << std::endl;
	std::cout << "list.empty(): "<< list.empty() << std::endl;
	std::cout << std::endl;

	// resize
	// list不像vector需要预先内存分配，所以resize动作同时也会改变内存用量
	std::cout << "list.resize: " << std::endl;
	for (int i = 0; i < 10; ++i) {
		list.push_back(i);
	}
	std::cout << "list.size(): " << list.size() << std::endl;
	std::cout << "list.empty(): "<< list.empty() << std::endl;

	list.resize(5);
	std::cout << "list.size(): " << list.size() << std::endl;
	std::cout << "list.empty(): "<< list.empty() << std::endl;

	list.resize(20);
	std::cout << "list.size(): " << list.size() << std::endl;
	std::cout << "list.empty(): "<< list.empty() << std::endl;

	std::cout << std::endl;

	// clear
	std::cout << "list.clear(): " << std::endl;
	list.clear();
	std::cout << "list.size(): " << list.size() << std::endl;
	std::cout << "list.empty(): "<< list.empty() << std::endl;
	std::cout << std::endl;

	// push & pop
	std::cout << "push_back" << std::endl;
	for (int i = 0; i < 10; ++i) {
		list.push_back(i);
	}
	std::cout << "push_front" << std::endl;
	for (int i = 0; i < 10; ++i) {
		list.push_front(i);
	}
	std::cout << "pop_front" << std::endl;
	for (int i = 0; i < 10; ++i) {
		list.pop_front();
	}
	std::cout << "pop_front" << std::endl;
	for (int i = 0; i < 10; ++i) {
		list.pop_front();
	}
	std::cout << std::endl;

	// insert
	// insert需要借住迭代器，好处是所有容器的代码都是一致的
	// 只要容器size改变，迭代器即失效，需要通过返回值来获取有效迭代器
	std::cout << "insert" << std::endl;
	for (int i = 0; i < 1000; ++i) {
		std::list<int>::iterator it = list.begin();
		it = list.insert(it, i);
		assert(it == list.begin());
	}
	std::cout << std::endl;

	// erase
	// insert需要借住迭代器，好处是所有容器的代码都是一致的
	// 只要容器size改变，迭代器即失效，需要通过返回值来获取有效迭代器
	std::cout << "erase" << std::endl;
	for (int i = 0; i < 1000; ++i) {
		std::list<int>::iterator it = list.begin();
		it = list.erase(it);
		assert(it == list.begin());
	}
	std::cout << std::endl;

	// swap
	// list的swap除了交换list以外，并没有像vector一样释放内存的功能
	std::cout << "swap" << std::endl;
	std::list<int> list2;
	for (int i = 0; i < 10; ++i) {
		list2.push_back(i);
	}
	list.swap(list2);
	std::cout << "list.size(): " << list.size() << std::endl;
	std::cout << std::endl;

	// element access
	// 因为list是链表结构，所以无法下标访问，只能取首尾元素
	std::cout << "element access" << std::endl;
	std::cout << list.front() << std::endl;
	std::cout << list.back() << std::endl;
	std::cout << std::endl;

	// iterator
	std::cout << "iterator" << std::endl;
	for (std::list<int>::iterator it = list.begin(); it != list.end(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;
	for (std::list<int>::reverse_iterator it = list.rbegin(); it != list.rend(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;
	std::cout << std::endl;

	// 迭代器运算
	// list的迭代器不支持计算两迭代器之间距离，以及偏移n个元素的操作
	// std::cout << "list.end() - list.begin(): " << list.end() - list.begin() << std::endl;	// error
	// std::list<int>::iterator it = list.begin() + 1;	// error

	// splice
	// splice的作用有两个：list拼接，list插入（单个元素，数据区域）
	// 虽然insert也可以做相同的操作，但是insert是针对所有类型容器，插入效率比较低
	// 而splice直接针对list，插入效率非常高
	std::cout << "list.splice()" << std::endl;
	std::list<int> list3;
	for (int i = 0; i < 10; ++i) {
		list3.push_back(i * 10);
	}
	list.splice(list.begin(), list3, list3.begin());
	std::cout << "list.size(): " << list.size() << std::endl;
	std::cout << std::endl;

	// remove
	// remove会删除list中所有与给定值相等的节点
	std::cout << "list.remove()" << std::endl;
	list.remove(0);
	std::cout << "list.size(): " << list.size() << std::endl;
	std::cout << std::endl;

	// remove_if
	// remove_if比remove更灵活，允许用户给出任意判定条件
	std::cout << "list.remove_if()" << std::endl;
	list.remove_if(check);
	std::cout << "list.size(): " << list.size() << std::endl;
	std::cout << std::endl;

	// unique
	// unique一般要配合sort一起使用，用于去除连续相同数据
	// unique支持无参和单参两个重载，允许用户自定义判断条件
	std::cout << "list.unique()" << std::endl;
	list.push_back(9);
	list.unique();
	std::cout << "list.size(): " << list.size() << std::endl;
	list.push_front(5);
	list.unique(int_equal);
	std::cout << "list.size(): " << list.size() << std::endl;
	std::cout << std::endl;

	// merge
	// merge用于给两个有序数组做二路归并
	std::cout << "list.merge()" << std::endl;
	std::list<int> list4;
	list4.push_back(0);
	list4.push_back(2);
	list4.push_back(4);
	list4.push_back(6);
	list4.push_back(8);
	std::list<int> list5;
	list5.push_back(1);
	list5.push_back(3);
	list5.push_back(5);
	list5.push_back(7);
	list5.push_back(9);
	list4.merge(list5, int_compare);
	for (std::list<int>::iterator it = list4.begin(); it != list4.end(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;
	std::cout << std::endl;

	// sort
	std::list<int> list6;
	list6.push_back(4);
	list6.push_back(6);
	list6.push_back(5);
	list6.push_back(7);
	list6.sort();
	for (std::list<int>::iterator it = list6.begin(); it != list6.end(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;

	list6.push_back(3);
	list6.sort(int_compare);
	for (std::list<int>::iterator it = list6.begin(); it != list6.end(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;
	std::cout << std::endl;

	// reverse
	// list反转
	list6.reverse();
	for (std::list<int>::iterator it = list6.begin(); it != list6.end(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;

	return EXIT_SUCCESS;
}				// ----------  end of function main  ----------

