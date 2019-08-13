/*
// =====================================================================================
// 
//       Filename:  errno.cpp
// 
//    Description:  errno & errmsg define
// 
//        Version:  1.0
//        Created:  10/26/2015 10:20:29 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Elwin.Gao (elwin), elwin.gao4444@gmail.com
//        Company:  
// 
// =====================================================================================
*/

#include "errno.h"

hash_map<int, std::string> Error_Wrapper::error_map;

// 如果将该宏展开放到头文件中，则static是必须要写的，否则会造成链接阶段报重复定义的错误
// 之所以把该宏放到cpp文件中，主要是考虑到当程序存在大量.o文件时，程序载入内存，反复大量创建不必要的对象，延长程序加载时间
// 此时，static关键字可加可不加，加上更好
//
// 另外，此处也可以不用__LINE__，而使用name来生成惟一对象名，使用name的好处在于可以保证宏定义的惟一性
#define ERROR_DEFINE(name, num, msg) static Error_Wrapper TOKENPASTE2(Unique_, __LINE__)(num, msg);
#include "errdef.h"
#undef ERROR_DEFINE

