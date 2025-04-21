/*
// =====================================================================================
// 
//       Filename:  return_type_dedute.cpp
// 
//    Description:  函数返回值类型，可通过auto关键字进行推导 
// 
//        Version:  1.0
//        Created:  04/21/2025 17:03:08
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
#include <type_traits>

// 注意，普通函数不能像lambda一样，支持通过auto进行参数类型推导
auto add(const int& n) {
	return n + 1;
}

decltype(auto) ret_int() {return 1;}
decltype(auto) ret_float() {return 1.1;}

/* 
// ===  FUNCTION  ======================================================================
//         Name:  main
//  Description:  
// =====================================================================================
*/
int main(int argc, char *argv[])
{
	std::cout << add(1) << std::endl;
	std::cout << ret_int() << std::endl;
	std::cout << ret_float() << std::endl;
	return EXIT_SUCCESS;
}				// ----------  end of function main  ----------

