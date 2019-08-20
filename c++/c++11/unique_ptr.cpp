/*
// =====================================================================================
// 
//       Filename:  unique_ptr.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  01/04/2016 08:04:26 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Elwin.Gao (elwin), elwin.gao4444@gmail.com
//        Company:  
// 
// =====================================================================================
*/

#include <cstdlib>
#include <iostream>
#include <memory>

std::unique_ptr<int> fun1()
{
	return std::unique_ptr<int>(new int(2));
}

void fun2(std::unique_ptr<int> &up)
{
	std::cout << "fun2: " << *up << std::endl;
}

void fun3(std::unique_ptr<int> up)
{
	std::cout << "fun3: " << *up << std::endl;
}

// unique_ptr对于deleter的定义要求有两个：
// 1、必须有默认构造函数
// 2、必须有“()”的运算符重载，即仿函数实现
// lambda表达式产生的类，不含有默认构造函数、默认西沟函数 和 赋值运算符重载
// 所以lambda表达式不能直接作为deleter使用
class simple_deleter {
public:
	simple_deleter() {
		std::cout << "simple_deleter default constructor" << std::endl;
	}
	template<typename T>
	void operator()(T *p) {
		std::cout << "simple_deleter ()operator" << std::endl;
		delete p;
	}
};

// my_deleter模拟了default_delete的官方实现方法
template<typename _Tp>
class my_deleter {
public:
	my_deleter() noexcept {
		std::cout << "my_delete default constructor" << std::endl;
	}

	template<typename _Up>
	my_deleter(const my_deleter<_Up>&) noexcept {
		std::cout << "my_delete conversion constructor" << std::endl;
	}

	void operator() (_Tp *p) const {
		std::cout << "my_delete by class deleter" << std::endl;
		delete p;
	}
};

// 定义一个删除函数
void delete_fun(int *p) {
	std::cout << "delete_fun() call" << std::endl;
	delete p;
}

/* 
// ===  FUNCTION  ======================================================================
//         Name:  main
//  Description:  
// =====================================================================================
*/
int main(int argc, char *argv[])
{
	// unique_ptr可以看作是单例shared_ptr
	// c++11中并没有类似make_shared<T>()函数类似的make_unique<T>()函数
	// make_unique<T>()函数在c++14中正式加入到std中
	std::unique_ptr<int> p1(new int(1));
	std::unique_ptr<int> p2 = std::move(p1);
	// std::unique_ptr<int> p3 = p1	// error，只能move，不能copy

	std::cout << "pointer data:" << *p2 << std::endl;
	// 传递unique_ptr的引用相当于传递指针的指针，unique_ptr所有权并不转移
	fun2(p2);
	std::cout << "pointer data:" << *p2 << std::endl;
	// unique_ptr无法直接作为参数传递，需要release后重新构造
	fun3(std::unique_ptr<int>(p2.release()));
	if (p2) {
		std::cout << "pointer exist:" << *p2 << std::endl;
	} else {
		std::cout << "pointer not exist:" << std::endl;
	}

	// unique_ptr另一个非常实用的用法，就是获取函数的内部返回值
	// 指针在函数内new，传递到函数外，不会造成内存泄漏
	std::unique_ptr<int> p3 = fun1();
	std::cout << *p3 << std::endl;
	// unique_ptr可以作为返回值，但是必须有对应的外部unique_ptr接收
	int *p4 = p3.get();
	std::cout << "pure pointer:" << *p4 << std::endl;
	int *p5 = fun1().get();
	std::cout << "pure pointer:" << *p5 << std::endl;

	// unique_ptr是天然支持c++动态数组的，而shared_ptr无法做到
	std::unique_ptr<int[]> parray(new int[4]);
	parray[0] = 0; parray[1] = 1; parray[2] = 2; parray[3] = 3;
	std::cout << parray[0] << std::endl;
	std::cout << parray[1] << std::endl;
	std::cout << parray[2] << std::endl;
	std::cout << parray[3] << std::endl;

	// unique_ptr允许传入预定义delete方式
	// default_delete有一项特权就是无须指定模版类型的第二个参数
	// 因为unique_ptr的模版定义将default_delete作为默认类型
	// 即p6的定义使用了默认类型，p8的定义显示的指定了默认类型，二者完全等效
	std::unique_ptr<int> p6 (new int, std::default_delete<int>());
	std::unique_ptr<int, std::default_delete<int>> p7 (new int);
	std::unique_ptr<int, std::default_delete<int>> p8 (new int, std::default_delete<int>());
	// 也可以传入diy delete方式
	// diy delete的方式必须制定第二个模版类型，但可以不写第二个参数
	std::unique_ptr<int, simple_deleter> p9 (new int);
	std::unique_ptr<int, simple_deleter> p10 (new int, simple_deleter());
	std::unique_ptr<int, my_deleter<int>> p11 (new int);
	std::unique_ptr<int, my_deleter<int>> p12 (new int, my_deleter<int>());

	// 可以直接使用函数来指定deleter
	std::unique_ptr<int, decltype(delete_fun)*> upi(new int, delete_fun);

	// lambda表达式不能直接作为构造函数参数使用
	// 可以使用函数指针作为模版类型，来使用lambda表达式
	std::unique_ptr<int, void(*)(int*)> p14 (new int,
			[](int *p) {
			std::cout << "delete by lambda" << std::endl;
			delete p;
			});
	// 也可以使用std::function作为模版类型，来使用lambda表达式
	std::unique_ptr<int, std::function<void(int*)>> p15 (new int,
			[&](int *p) {
			std::cout << "delete by lambda" << std::endl;
			delete p;
			});

	return EXIT_SUCCESS;
}				// ----------  end of function main  ----------

