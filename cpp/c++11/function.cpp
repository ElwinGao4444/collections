/*
// =====================================================================================
// 
//       Filename:  function.cpp
// 
//    Description:  function的作用主要是用于取代函数指针
// 
//        Version:  1.0
//        Created:  01/04/2016 04:22:58 PM
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
#include <functional>

class Foo
{
public:
	Foo(const std::function<void()> &fun) : callback(fun) {}
	void notify()
	{
		callback();
	}
private:
	std::function <void()> callback;
};

class Bar
{
public:
	void operator()(void)
	{
		std::cout << "Bar: " << __FUNCTION__ << std::endl;
	}
};

/* 
// ===  FUNCTION  ======================================================================
//         Name:  main
//  Description:  
// =====================================================================================
*/
int main(int argc, char *argv[])
{
	Bar bar;
	Foo foo(bar);
	foo.notify();

	return EXIT_SUCCESS;
}				// ----------  end of function main  ----------

