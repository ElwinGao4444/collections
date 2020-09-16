/*
// =====================================================================================
// 
//       Filename:  vector.cpp
// 
//    Description:  vector是一种数组结构
// 
//        Version:  1.0
//        Created:  09/01/2017 02:39:58 PM
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

#include <vector>

/* 
// ===  FUNCTION  ======================================================================
//         Name:  main
//  Description:  
// =====================================================================================
*/
int main(int argc, char *argv[])
{
	std::vector<int> vec;

	std::cout << "vec.size(): " << vec.size() << std::endl;
	std::cout << "vec.capacity(): " << vec.capacity() << std::endl;
	std::cout << "vec.max_size(): "<< vec.max_size() << std::endl;
	std::cout << "vec.empty(): "<< vec.empty() << std::endl;
	std::cout << std::endl;

	// set reserve
	// 当预期会有一次大量元素插入的时候，提前进行reserve会对写入性能有提升
	// 这是由vector的内存分配机制引起的
	// vector有两种内存分配方式：1、开辟申请内存的1.5倍；2、以最小2的n次幂进行内存分配
	// 无论哪种方式，一次大量写入，都会造成内存不足，从而需要不停的扩展空间，以及反复拷贝的问题
	std::cout << "vec.reserve(100): " << std::endl;
	vec.reserve(100);
	std::cout << "vec.size(): " << vec.size() << std::endl;
	std::cout << "vec.capacity(): " << vec.capacity() << std::endl;
	std::cout << "vec.empty(): "<< vec.empty() << std::endl;
	std::cout << std::endl;

	// resize
	// resize的含义是调整vector中数据的数量，而不会调整内容的使用量
	// 在vector中内存一般总之持续上涨而不会下降的
	std::cout << "vec.resize: " << std::endl;
	for (int i = 0; i < 10; ++i) {
		vec.push_back(i);
	}
	std::cout << "vec.size(): " << vec.size() << std::endl;
	std::cout << "vec.capacity(): " << vec.capacity() << std::endl;
	std::cout << "vec.empty(): "<< vec.empty() << std::endl;

	vec.resize(5);
	std::cout << "vec.size(): " << vec.size() << std::endl;
	std::cout << "vec.capacity(): " << vec.capacity() << std::endl;
	std::cout << "vec.empty(): "<< vec.empty() << std::endl;

	vec.resize(20);
	std::cout << "vec.size(): " << vec.size() << std::endl;
	std::cout << "vec.capacity(): " << vec.capacity() << std::endl;
	std::cout << "vec.empty(): "<< vec.empty() << std::endl;

	std::cout << std::endl;

	// clear
	// clear 和 vec.resize(0)表达的意思相同
	std::cout << "vec.clear(): " << std::endl;
	vec.clear();
	std::cout << "vec.size(): " << vec.size() << std::endl;
	std::cout << "vec.capacity(): " << vec.capacity() << std::endl;
	std::cout << "vec.empty(): "<< vec.empty() << std::endl;
	std::cout << std::endl;

	// push_back
	std::cout << "push_back 1000 element" << std::endl;
	for (int i = 0; i < 1000; ++i) {
		vec.push_back(i);
	}
	std::cout << std::endl;

	// pop_back
	std::cout << "pop_back" << std::endl;
	for (int i = 0; i < 1000; ++i) {
		// pop_back返回值为空
		// 获取last element需要使用back()方法
		vec.pop_back();
	}
	std::cout << std::endl;

	// insert
	// insert需要借住迭代器，好处是所有容器的代码都是一致的
	// insert返回指向新节点首元素的迭代器
	// insert除了可以插入单个元素，还可以重复插入及范围插入
	std::cout << "insert" << std::endl;
	for (int i = 0; i < 1000; ++i) {
		std::vector<int>::iterator it = vec.begin();
		it = vec.insert(it, i);
		assert(it == vec.begin());
	}
	std::cout << std::endl;

	// erase
	// erase需要借助迭代器，erase返回指向最后一个被删除元素的下一个元素的迭代器
	// 在遍历删除中，不看erase返回值，而是采用删除成功就对当前迭代器++的方法从原理角度上都是错的
	// 因为只要vector元素发生变化，原迭代器即失效，继续使用原迭代器是一种不符合规范的行为
	std::cout << "erase" << std::endl;
	for (int i = 0; i < 1000; ++i) {
		std::vector<int>::iterator it = vec.begin();
		it = vec.erase(it);
		assert(it == vec.begin());
	}
	std::cout << std::endl;

	// swap
	// swap并不是简单的交换数据，本质上是交换内存
	// 从这个例子中可以看到，vec虽然数据被清空，但是依旧占用1600个int的空间
	// 但是经过swap之后，数据元素涨到了10个，但是占用空间却缩减为16个
	// 我们可以使用swap交换内存的特性，对原vector中多余的内存进行释放
	// 直到C++11，出现了新函数shrink_to_fit()，可以直接对内存进行缩减
	std::cout << "swap" << std::endl;
	std::vector<int> vec2;
	for (int i = 0; i < 10; ++i) {
		vec2.push_back(i);
	}
	vec.swap(vec2);
	std::cout << "vec.size(): " << vec.size() << std::endl;
	std::cout << "vec.capacity(): " << vec.capacity() << std::endl;
	std::cout << std::endl;

	// element access
	std::cout << "element access" << std::endl;
	std::cout << "vec[0]: " << vec[0] << std::endl;
	std::cout << "vec.at(0): " << vec.at(0) << std::endl;
	std::cout << "vec.front(): " << vec.front() << std::endl;
	std::cout << "vec.back(): " << vec.back() << std::endl;
	std::cout << std::endl;

	// iterator
	std::cout << "iterator" << std::endl;
	for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;
	for (std::vector<int>::reverse_iterator it = vec.rbegin(); it != vec.rend(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;
	std::cout << std::endl;

	// 关于下标和迭代器
	// 由于vector下标访问非常方便，但是涉及到insert和erase操作，却必须要用迭代器
	// 这就使得我们必须可以在下标和迭代器之间快速转换
	// 这个转换的原理就是vector的iterator支持算数运算操作
	std::cout << "index and iterator" << std::endl;
	std::cout << "vec.size(): " << vec.size() << std::endl;
	std::cout << "vec.end() - vec.begin(): " << vec.end() - vec.begin() << std::endl;
	for (int i = 0; i < 10; ++i) {
		std::cout << vec[i] << " ";
		std::cout << *(vec.begin() + i) << std::endl;
	}

	return EXIT_SUCCESS;
}				// ----------  end of function main  ----------

