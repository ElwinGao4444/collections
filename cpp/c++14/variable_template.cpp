/*
// =====================================================================================
// 
//       Filename:  variable_template.cpp
// 
//    Description:  将模版扩展到了变量上（不仅是函数/类模版） 
//                  语法：template<typename T> // 模版参数列表
//                        T variable_name = xxx; // 变量定义（初始化）
// 
//        Version:  1.0
//        Created:  04/21/2025 17:33:26
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

// 用法一：对于基础类型的模版
template <typename T>
constexpr T pi = T(3.14);

// 用法二：对于类类型的模版变量
template<typename T>
T default_value = T(); // 默认构造的默认值
template<> // 模版特化：
int default_value<int> = 42;

/* 
// ===  FUNCTION  ======================================================================
//         Name:  main
//  Description:  
// =====================================================================================
*/
int main(int argc, char *argv[])
{
	std::cout << "pin int: " << pi<int> << std::endl;
	std::cout << "pin float: " << pi<float> << std::endl;

	std::cout << "default int: " << default_value<int> << std::endl;
	std::cout << "default float: " << default_value<float> << std::endl;

	return EXIT_SUCCESS;
}				// ----------  end of function main  ----------

