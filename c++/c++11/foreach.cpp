/*
// =====================================================================================
// 
//       Filename:  foreach.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  01/04/2016 04:15:02 PM
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
#include <map>

/* 
// ===  FUNCTION  ======================================================================
//         Name:  get_map
//  Description:  模拟获取容器的方法，用于验证foreach的特型
// =====================================================================================
*/
static std::map<int, std::string>& get_map(std::map<int, std::string> &info)
{
	std::cout << "get_map()" << std::endl;
	return info;
}		// -----  end of static function get_map  -----

/* 
// ===  FUNCTION  ======================================================================
//         Name:  main
//  Description:  
// =====================================================================================
*/
int main(int argc, char *argv[])
{
	// c++ 支持的foreach循环，循环变量与迭代器用法中的迭代器变量*it等效
	std::string str = std::string("abc");
	for (const auto &c : str) {	// 即使没有定义const，使用foreach语法时，元素变量默认也是const的
		// 在循环过程中，对元素进行修改，将会导致迭代器失效，出现迭代异常
		std::cout << c << std::endl;
	}

	// c++ 支持了变长初始化列表，相应的std中的各种容器都可以进行变长初始化
	std::map<int, std::string> info = {
		{1, "a"}, {2, "b"}, {3, "c"}
	};
	for (const auto &i : get_map(info)) {	// 这里的get_map()只会在循环开始调用1次
		std::cout << i.first << " : " << i.second << std::endl;
	}

	return EXIT_SUCCESS;
}				// ----------  end of function main  ----------

