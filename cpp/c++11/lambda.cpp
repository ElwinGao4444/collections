/*
// =====================================================================================
// 
//       Filename:  lambda.cpp
// 
//    Description:  lambda表达式的用法
//                  语法：[capture]{params} opt ->ret {body;}
// 
//        Version:  1.0
//        Created:  01/04/2016 04:59:36 PM
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

int global = 0;

class Foo
{
public:
	Foo(int i) : value(i)
	{
	}

	int fun()
	{
		// 捕获this指针
		return [this] {return this->value;}();
	}
private:
	int value;
};

/* 
// ===  FUNCTION  ======================================================================
//         Name:  main
//  Description:  
// =====================================================================================
*/
int main(int argc, char *argv[])
{
	auto f1 = [](int i) -> int {return ++i;};
	std::cout << f1(1) << std::endl;

	auto f2 = [](int i) {return ++i;};	// 如果返回值可推倒，那么返回值类型可以省略
	std::cout << f2(2) << std::endl;

	// 捕获(capture)的用法，捕获的含义为直接使用上下文中的外部变量
	int i1 = 1, i2 = 2, i3 = 3;

	auto f3 = [] {return 999;};
	std::cout << f3() << std::endl;	// 不捕获任何外部变量

	auto f4 = [=] {return ++global;};
	std::cout << f4() << std::endl;	// 以赋值的形式捕获所有外部变量
	std::cout << global << std::endl;

	auto f5 = [&] {return ++global;};
	std::cout << f5() << std::endl;	// 以引用的形式捕获所有外部变量
	std::cout << global << std::endl;

	auto f6 = [i1, &i2] {return i1 + ++i2;};
	std::cout << f6() << std::endl;	// 捕获i1和i2的引用
	std::cout << i1 << " " << i2 << std::endl;

	auto f7 = [=, &i3] {return i1 + i2 + ++i3;};	// 非引用的对象，都是read-only类型，不可修改
	std::cout << f7() << std::endl;	// 捕获所有外部变量和i3的引用
	std::cout << i1 << " " << i2 << " " << i3 << std::endl;

	Foo foo(123);
	std::cout << foo.fun() << std::endl;

	return EXIT_SUCCESS;
}				// ----------  end of function main  ----------

