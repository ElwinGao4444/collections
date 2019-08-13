/*
// =====================================================================================
// 
//       Filename:  traits.cpp
// 
//    Description:  traits编程技术
//
//                  通常类型萃取都包含一下内容
//                  template <class _Iterator>  
//                  struct iterator_traits {  
//                      typedef typename _Iterator::iterator_category iterator_category;    //迭代器类型  
//                      typedef typename _Iterator::value_type        value_type;           //迭代器所指对象的类型  
//                      typedef typename _Iterator::difference_type   difference_type;      //迭代器之间距离  
//                      typedef typename _Iterator::pointer           pointer;              //迭代器所指之物  
//                      typedef typename _Iterator::reference         reference;            //迭代器引用之物  
//                  };  
//
// 
//        Version:  1.0
//        Created:  01/05/2016 10:50:17 AM
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

// 当需要进行类型推导时，最好的方式是使用模板
// 无论参数还是返回值都可以使用模板类型
// 但是，模板使用最大的障碍在于无法进行类型推导
template <typename T, typename U>
U fun1(T arg1, U arg2)	// 如果想返回*U类型，直接使用模板类型是做不到的
{
	T t1 = arg1;
	U t2 = arg2;
	std::cout << t1 << std::endl;
	std::cout << t2 << std::endl;
	return t2;
}

// 内嵌类型声明解决了返回*T的问题
// 但是内嵌类型声明对系统内嵌类型无法支持
template <typename T>
struct MyIt
{
	typedef T value_type;	//内嵌类型声明

	T value;

	MyIt(T v)
	{
		value = v;
	}

	T operator*()
	{
		return value;
	}
};

template <typename ItType>
typename ItType::value_type fun2(ItType it)	// 内嵌类型声明限制了系统内置类型无法使用这种方法
{
	return *it;
}

// 类型萃取技术完美解决了以上两种方法存在的问题
template <typename ItType>
struct iterator_traits
{
	typedef typename ItType::value_type value_type;
};

template <typename ItType>
struct iterator_traits<ItType*>
{
	typedef ItType value_type;
};

template <typename ItType>
struct iterator_traits<const ItType*>
{
	typedef ItType value_type;
};

template <typename ItType>
typename iterator_traits<ItType>::value_type fun3(ItType it)
{
	return *it;
}

/* 
// ===  FUNCTION  ======================================================================
//         Name:  main
//  Description:  
// =====================================================================================
*/
int main(int argc, char *argv[])
{
	// 单纯模板使用
	int a = 0;
	int *p = &a;
	int b = fun1(p, *p);

	// 内嵌类型声明
	MyIt<int> it(1);
	std::cout << fun2(it) << std::endl;

	// 类型萃取技术
	std::cout << fun3(p) << std::endl;
	std::cout << fun3(it) << std::endl;

	return EXIT_SUCCESS;
}				// ----------  end of function main  ----------
