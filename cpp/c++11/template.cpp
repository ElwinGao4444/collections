/*
// =====================================================================================
// 
//       Filename:  template.cpp
// 
//    Description:  变长参数模板的用法
//                  虽然这里介绍了一些“书本”方法，但是变长模板最实用的场景是配合
//                  std::function将函数与参数进行绑定中使用，尤其是配合lambda表达式
// 
//        Version:  1.0
//        Created:  06/21/2016 03:24:26 PM
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

// 标准的定长模板形式
template <typename T>
void fixed(T i)
{
	std::cout << "fixed: " << i << std::endl;
}		// -----  end of template function fixed  -----

// =====================================================================================

// C++11新特型，变更长模板形式

// 非递归方式对参数列表解包
template <typename ... T>
void dummy_wrapper(T... t){};

template <typename T>
void print(T i)
{
	std::cout << i << std::endl;
}		// -----  end of template function print  -----

template <typename... T>
void variable(T... args)
{
	std::cout << "sizeof variable: " << sizeof...(args) << std::endl;
	dummy_wrapper((print(args), 0)...);	// 很别扭的用法，而且为什么反向打印？
}		// -----  end of template function variable  -----

// =====================================================================================

// 递归方式对参数列表解包
template <typename T, typename... Args>
void print_r(T i)
{
	std::cout << i << std::endl;
}		// -----  end of template function print  -----

template <typename T, typename... Args>
void print_r(T i, Args... args)
{
	std::cout << i << std::endl;
	print_r(args...);
}		// -----  end of template function print  -----

template <typename T, typename... Args>
void variable_r(T i, Args... args)
{
	std::cout << "sizeof variable_r: " << sizeof...(args) << std::endl;
	print_r(i, args...);
}		// -----  end of template function variable  -----

/* 
// ===  FUNCTION  ======================================================================
//         Name:  main
//  Description:  
// =====================================================================================
*/
int main(int argc, char *argv[])
{
	fixed(0);
	variable(1, 2, 3, 4);
	variable_r(1, 2, 3, 4);
	std::cout << "Hello World" << std::endl;
	return EXIT_SUCCESS;
}				// ----------  end of function main  ----------

