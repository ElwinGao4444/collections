/*
// =====================================================================================
// 
//       Filename:  type.cpp
// 
//    Description:  auto变量的用法
// 
//        Version:  1.0
//        Created:  01/04/2016 03:21:31 PM
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
#include <string>

/* auto变量不能用于函数参数 */
/* auto 和 decltype 在模板上的应用十分广泛，可用于简化代码 */

/* 
// ===  FUNCTION  ======================================================================
//         Name:  main
//  Description:  
// =====================================================================================
*/
int main(int argc, char *argv[])
{
	// auto类型可以自动识别变量类型
	// 但是auto类型必须立即初始化，否则会报编译错误
	auto str = std::string("abc");
	for (auto it=str.begin(); it!=str.end(); ++it) {
		std::cout << *it << std::endl;
	}

	// auto会把const属性抛弃
	auto i1 = 1; i1 = 2;
	std::cout << i1 << std::endl;

	const int ci2 = 10;	// 尝试对ci2赋值会报编译错误
	auto i2 = ci2;		// 尝试催 i2赋值，可以正常执行
	i2 = 20;
	std::cout << i2 << std::endl;

	auto i3 = i1 + i2;
	std::cout << i3 << std::endl;

	// 当auto和引用（或指针）结合时，auto推导将保留表达式的const属性
	auto &i4 = ci2;	// i4为const引用
	// i4 = 0;	// 赋值会引起编译错误

	// decltype的作用和auto类似，区别在于auto根据初始化决定参数，decltype根据表达式确定类型
	// 二者最大的区别：auto必须初始化，decltype不必初始化
	std::string s1 = std::string("ijk");
	decltype(s1) s2;	// decltype在编译时会改写为表达式的返回类型
	s2 = "bje";
	for (decltype(s2.begin()) it = s2.begin(); it!=s2.end(); ++it) {
		std::cout << *it << std::endl;
	}

	return EXIT_SUCCESS;
}				// ----------  end of function main  ----------

