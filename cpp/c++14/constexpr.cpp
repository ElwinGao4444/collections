/*
// =====================================================================================
// 
//       Filename:  constexpr.cpp
// 
//    Description:  constexpr关键词用法 
//                  constexpr用于描述编译时计算结果，且值不会改变的情况
//                  主要作用就是性能提升，将运行时计算提前到编译时计算（const允许在运行时确定）
//                  虽然该关键字在C++11中就已经出现，但直到C++14才有更大的使用价值
// 
//        Version:  1.0
//        Created:  04/21/2025 18:03:46
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

// C++11 constexpr 的函数，必须有且只有一行，通常配合三目运算符，所以使用场景很窄
constexpr int factorial_in_11(int n) { // C++14 和 C++11均可
    return n <= 1 ? 1 : (n * factorial_in_11(n - 1));
}

// C++14 允许constexpr函数可以有多行，但参数和返回值，依然要保持字面值，不允许使用auto
constexpr int factorial_in_14(int n) { // C++11中不可，C++14中可以
    int ret = 0;
    for (int i = 0; i < n; ++i) {
        ret *= i;
    }
    return ret;
}

/* 
// ===  FUNCTION  ======================================================================
//         Name:  main
//  Description:  
// =====================================================================================
*/
int main(int argc, char *argv[])
{
	constexpr int n1 = 20;
	constexpr int n2 = n1 + 1;
	std::cout << n1 << "," << n2 << std::endl;
	std::cout << factorial_in_11(10) << "," << factorial_in_11(10) << std::endl;
	return EXIT_SUCCESS;
}				// ----------  end of function main  ----------

