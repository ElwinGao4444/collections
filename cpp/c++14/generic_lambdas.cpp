/*
// =====================================================================================
// 
//       Filename:  generic_lambdas.cpp
// 
//    Description: 泛型Lambda，Lambda参数支持auto类型推导，实现类似模版的泛型操作
// 
//        Version:  1.0
//        Created:  04/21/2025 16:55:35
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

/* 
// ===  FUNCTION  ======================================================================
//         Name:  main
//  Description:  
// =====================================================================================
*/
int main(int argc, char *argv[])
{
	// lambda函数的参数和返回值，都可以通过auto进行推导，此前的C++11是不支持的
	auto add = [](const auto& n) -> auto {
		return n + 1;
	};
	std::cout << add(1) << std::endl;
	std::cout << add(1.1) << std::endl;
	return EXIT_SUCCESS;
}				// ----------  end of function main  ----------

